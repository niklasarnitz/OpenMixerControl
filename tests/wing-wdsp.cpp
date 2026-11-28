// SPDX-License-Identifier: GPL-3.0-only
#include "wing-wdsp.h"
#include "spibone_uapi.h"
#include <linux/magic.h>
#include <linux/spi/spidev.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>

#include <array>
#include <cassert>
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
std::string dir;
int control, dsp, dspOpens, transfers, controlOpens;
bool offline, noEntropy, nonBootLocal;
unsigned expectedCore, resetMask, changedCsr;
constexpr uint32_t nonce = 0x19274690;
constexpr std::array overdrive{2.5f, -0.25f, 0.125f, 0.0625f, 0.75f, 0.5f};
// Expected wire bits are fixed independently of the production encoder.
constexpr std::array overdriveWords{0x40200000u, 0xbe800000u, 0x3e000000u, 0x3d800000u, 0x3f400000u, 0x3f000000u};
unsigned updateOp, randomReads, faultOnTransfer = 1;
bool loseFabricAfterIdentify;
enum BadReply { Good, Empty, Short, Early, Magic, Abi, Core, Nonce, RequestCrc, ReplyCrc, Status, Flags, Reserved, Replay,
                StatusCrc, SportFault, Legacy };
BadReply bad;
uint32_t coreClock = 400000000;
OMC::WingDspIdentity identity;

void Put(uint8_t* p, uint32_t v)
{
    for (int i = 3; i >= 0; --i, v >>= 8) p[i] = v & 255;
}

// Rewrites the shell's manifest with other capabilities or board profile; returns its SHA-256.
std::string Manifest(const std::string& image, unsigned flags, const std::string& profile)
{
    {
        std::ofstream out(dir + "/manifest");
        out << R"({"schema_version":2,"abi_version":131072,"magic":1464421959,"build_id":305419896,"feature_flags":)"
            << flags << R"(,"target":"wing-compact-t55f484-fork","device":"T55F484",)"
            << R"("bitstream":{"file":"wing.bin","sha256":")" << image << R"("},"dsp_board_profile_sha256":")" << profile
            << R"(","transport":{"device":"/dev/spibone0","compatible":"openmixer,wing-spibone","chip_select":1,)"
            << R"("mode":0,"max_hz":2000000},"dsp_runtime":{"device":"/dev/spidev1.0","chip_select":0,"abi":1}})";
    }
    FILE* command = popen(("sha256sum " + dir + "/manifest").c_str(), "r");
    char hex[65]{};
    assert(command && fscanf(command, "%64[0-9a-f]", hex) == 1 && pclose(command) == 0);
    return hex;
}

void Receipt(const std::string& digest, const std::string& image, unsigned mask)
{
    std::ofstream out(dir + "/receipt");
    out << R"({"manifest":"/usr/share/wing-t55/manifest.json","build_id":305419896,"chip_select":1,)"
        << R"("spi_device":"/dev/spibone0","dsp_runtime_mask":)" << mask
        << R"(,"manifest_sha256":")" << digest << R"(","bitstream_sha256":")" << image << "\"}\n";
}

void Try(unsigned core, bool success, int count, int opens, bool allowControl = true,
         const std::array<float, 6>* values = &overdrive)
{
    expectedCore = core;
    transfers = dspOpens = controlOpens = 0;
    randomReads = 0;
    try {
        if (updateOp) OMC::SetWingOverdrive(values);
        else identity = OMC::IdentifyWingDsp(core);
        assert(success);
    } catch (const std::runtime_error& e) {
        if (success || transfers != count || dspOpens != opens) std::cerr << e.what() << '\n';
        assert(!success);
    }
    assert(transfers == count && dspOpens == opens);
    if (!allowControl) assert(controlOpens == 0);
}
} // namespace

extern "C" int __real_open(const char*, int, ...);
extern "C" int __real_fstat(int, struct stat*);
extern "C" int __wrap_fstat(int fd, struct stat* st)
{
    const int ret = __real_fstat(fd, st);
    if (!ret && st->st_uid == geteuid() && st->st_gid == getegid()) st->st_uid = st->st_gid = 0;
    return ret;
}

extern "C" int __wrap_lstat(const char* path, struct stat* st)
{
    assert(path == std::string("/run") || path == std::string("/usr/share/wing-t55"));
    *st = {};
    st->st_mode = S_IFDIR | 0755;
    return 0;
}

extern "C" int __wrap_statfs(const char* path, struct statfs* fs)
{
    assert(path == std::string("/run"));
    fs->f_type = nonBootLocal ? 0 : TMPFS_MAGIC;
    return 0;
}

extern "C" int __wrap_open(const char* path, int flags, ...)
{
    assert((flags & (O_NOFOLLOW | O_CLOEXEC)) == (O_NOFOLLOW | O_CLOEXEC));
    const std::string p(path);
    if (p == "/run/wing-t55.lock") return __real_open((dir + "/lock").c_str(), flags);
    if (p == "/run/omc-wing-fpga.json") return __real_open((dir + "/receipt").c_str(), flags);
    if (p == "/usr/share/wing-t55/manifest.json") return __real_open((dir + "/manifest").c_str(), flags);
    if (p == "/dev/spibone0") {
        ++controlOpens;
        return control = __real_open("/dev/null", flags);
    }
    assert(p == "/dev/spidev1.0");
    ++dspOpens;
    return dsp = __real_open("/dev/null", flags);
}

extern "C" ssize_t __wrap_getrandom(void* out, size_t size, unsigned flags)
{
    assert(size == sizeof nonce && flags == 1);
    const uint32_t challenge = nonce + randomReads++;
    std::memcpy(out, &challenge, size);
    return noEntropy ? -1 : size;
}

extern "C" int __wrap_ioctl(int fd, unsigned long op, ...)
{
    va_list args;
    va_start(args, op);
    if (fd == control) {
        if (op == SPIBONE_IOC_GET_STATE) {
            *va_arg(args, uint32_t*) = offline || (loseFabricAfterIdentify && transfers > 0)
                ? SPIBONE_STATE_OFFLINE : SPIBONE_STATE_ONLINE;
        } else {
            assert(op == SPIBONE_IOC_PEEK); // No write ioctl may reach the FPGA.
            auto* reg = va_arg(args, spibone_wb_xfer*);
            const std::array values{0x57494e47u, 0x20000u, 0x12345678u, 0x7fu, 0x101u, resetMask, 0u, 0u};
            assert(reg->addr >= 0x1800 && reg->addr <= 0x181c && reg->addr % 4 == 0);
            reg->val = values[(reg->addr - 0x1800) / 4] ^ (reg->addr == changedCsr ? 1 : 0);
        }
        va_end(args);
        return 0;
    }
    assert(fd == dsp);
    if (op == SPI_IOC_WR_MODE32) assert(*va_arg(args, uint32_t*) == 0);
    else if (op == SPI_IOC_WR_BITS_PER_WORD) assert(*va_arg(args, uint8_t*) == 8);
    else if (op == SPI_IOC_WR_MAX_SPEED_HZ) assert(*va_arg(args, uint32_t*) == 10000);
    else {
        assert(op == SPI_IOC_MESSAGE(1));
        ++transfers;
        const auto replyFault = transfers == int(faultOnTransfer) ? bad : Good;
        const auto& xfer = *va_arg(args, spi_ioc_transfer*);
        va_end(args);
        // Identify clocks the 44-byte v1.1 reply, updates the 24-byte v1.0 ACK.
        assert(xfer.len == (transfers == 1 ? 93u : 73u) && xfer.bits_per_word == 8 && xfer.cs_change == 0 &&
               xfer.speed_hz == 10000);
        const auto* tx = reinterpret_cast<const uint8_t*>(xfer.tx_buf);
        auto* rx = reinterpret_cast<uint8_t*>(xfer.rx_buf);
        std::memset(rx, 0xff, xfer.len); // the DSP sends 0xff whenever it has no reply byte
        for (size_t i = 73; i < xfer.len; ++i) assert(tx[i] == 0);
        std::array<uint8_t, 73> expected{};
        expected[0] = expected[12] = expectedCore;
        std::memcpy(expected.data() + 1, "WDSP", 4);
        const uint32_t challenge = nonce + transfers - 1;
        Put(expected.data() + 5, challenge);
        if (transfers == 2) {
            assert(updateOp && expectedCore == 4);
            expected[9] = updateOp;
            expected[10] = 5;
            if (updateOp == 2) {
                for (size_t i = 0; i < overdriveWords.size(); ++i)
                    Put(expected.data() + 13 + 4 * i, overdriveWords[i]);
            }
        }
        const uint32_t crc = crc32(0, expected.data() + 1, 36);
        Put(expected.data() + 37, crc);
        assert(std::memcmp(tx, expected.data(), expected.size()) == 0);
        const int exclusive = __real_open((dir + "/lock").c_str(), O_RDWR);
        assert(exclusive >= 0 && flock(exclusive, LOCK_EX | LOCK_NB) < 0);
        close(exclusive);
        static std::array<uint8_t, 24> identifyReply;
        if (replyFault != Empty) {
            auto* reply = rx + (replyFault == Early ? 40 : 49);
            std::memcpy(reply, replyFault == Magic ? "WDSP" : "WDSR", 4);
            Put(reply + 4, replyFault == Abi ? 2 : 1);
            Put(reply + 8, challenge ^ (replyFault == Nonce));
            reply[12] = replyFault == Core ? 3 : expectedCore;
            reply[13] = replyFault == Status ? 2 : 0;
            reply[14] = replyFault == Flags ? 3 : replyFault == SportFault ? 2 : 1;
            reply[15] = replyFault == Reserved ? 1 : 0;
            Put(reply + 16, crc ^ (replyFault == RequestCrc));
            Put(reply + 20, crc32(0, reply, 20) ^ (replyFault == ReplyCrc));
            if (transfers == 1 && replyFault != Legacy) {
                Put(reply + 24, 1); // ABI minor
                Put(reply + 28, replyFault == SportFault ? 3 : 0);
                Put(reply + 32, 1234);
                Put(reply + 36, coreClock);
                Put(reply + 40, crc32(0, reply, 40) ^ (replyFault == StatusCrc));
            }
            if (transfers == 1) std::memcpy(identifyReply.data(), reply, identifyReply.size());
            if (replyFault == Replay) std::memcpy(reply, identifyReply.data(), identifyReply.size());
        }
        return xfer.len - (replyFault == Short);
    }
    va_end(args);
    return 0;
}

int main(int argc, char** argv)
{
    assert(argc == 4);
    dir = argv[1];
    std::string digest = argv[2];
    const std::string image = argv[3];
    Try(4, false, 0, 0, false); // No receipt.
    Receipt(digest, image, 0);
    Try(4, false, 0, 0, false);
    Receipt(digest, image, 8);
    Try(2, false, 0, 0, false);
    Try(3, false, 0, 0, false);
    Receipt(digest, image, 0x18); // Reject the whole record, not merely the unknown core.
    Try(4, false, 0, 0, false);
    Receipt(image, image, 8); // Hash mismatch.
    Try(4, false, 0, 0, false);
    Receipt(digest, std::string(64, '1'), 8); // Image mismatch.
    Try(4, false, 0, 0, false);
    Receipt(digest, image, 0x0f);
    const auto path = dir + "/receipt";
    assert(chmod(path.c_str(), 0644) == 0);
    Try(4, false, 0, 0, false);
    assert(chmod(path.c_str(), 0600) == 0);
    const int exclusive = __real_open((dir + "/lock").c_str(), O_RDWR);
    assert(exclusive >= 0 && flock(exclusive, LOCK_EX | LOCK_NB) == 0);
    Try(4, false, 0, 0, false);
    close(exclusive);
    nonBootLocal = true;
    Try(4, false, 0, 0, false);
    nonBootLocal = false;
    // The loader leaves every reset released (0xf) between operations; anything else is a held core.
    for (unsigned held : {0x0bu, 0x07u, 0u}) {
        resetMask = held;
        Try(4, false, 0, 0);
    }
    resetMask = 0x0f;
    offline = true;
    Try(4, false, 0, 0);
    offline = false;
    for (changedCsr = 0x1800; changedCsr <= 0x181c; changedCsr += 4) Try(4, false, 0, 0);
    changedCsr = 0;
    // Capabilities must include SPIBone, DSP runtime, Q4.28 and AES50 B; extra bits are fine.
    const std::string profile = "1849b434f1c5a974d67a49795cff04ec26caa582a1336396ae022934aab9ee6a";
    for (unsigned missing : {1u, 2u, 4u, 8u}) {
        Receipt(Manifest(image, 0x7f & ~missing, profile), image, 0x0f);
        Try(4, false, 0, 0, false);
    }
    // The superseded profile (no DSP3 reset, 12.288 MHz CLKIN) is not this board.
    Receipt(Manifest(image, 0x7f, "3b29934cdc1c704de69b6b028bc71387ab7c50cb74e811fb35ca0f64664a3e9d"), image, 0x0f);
    Try(4, false, 0, 0, false);
    Receipt(Manifest(image, 0x0f, profile), image, 0x0f);
    Try(4, false, 0, 0); // the live CSR (0x7f) must equal the manifest
    digest = Manifest(image, 0x7f, profile);
    Receipt(digest, image, 0x0f);
    noEntropy = true;
    Try(4, false, 0, 1);
    noEntropy = false;
    for (auto core : {1u, 2u, 3u, 4u}) Try(core, true, 1, 1);
    assert(identity.extended && identity.fault == 0 && identity.blocks == 1234 && identity.coreClockHz == coreClock);
    OMC::RequireWingDspAudioClock(identity);
    for (bad = Empty; bad <= Reserved; bad = static_cast<BadReply>(bad + 1)) Try(4, false, 1, 1);
    bad = StatusCrc;
    Try(4, false, 1, 1);
    bad = SportFault;
    Try(4, false, 1, 1);
    bad = Legacy; // v1.0 firmware: Identify still succeeds, but reports no CCLK
    Try(4, true, 1, 1);
    assert(!identity.extended);
    const auto refused = [](const char* reason) {
        try {
            OMC::RequireWingDspAudioClock(identity);
        } catch (const std::runtime_error& e) {
            if (std::strstr(e.what(), reason)) return;
        }
        std::abort();
    };
    refused("v1.0");
    bad = Good;
    for (auto [clock, reason] : {std::pair{0u, "not measured"}, {393215999u, "below"}}) {
        coreClock = clock;
        Try(4, true, 1, 1);
        refused(reason);
    }
    coreClock = 393216000;
    Try(4, true, 1, 1);
    OMC::RequireWingDspAudioClock(identity);
    bad = Good;

    updateOp = 2;
    bad = Good;
    Receipt(digest, image, 3); // Other qualified cores never substitute for DSP4.
    Try(4, false, 0, 0, false);
    Receipt(digest, image, 0x0f);
    for (float q : {1.f, std::numeric_limits<float>::quiet_NaN()}) {
        auto invalid = overdrive;
        invalid[1] = q;
        Try(4, false, 0, 0, false, &invalid);
    }
    bad = Flags;
    Try(4, false, 1, 1); // Failed Identify must prevent the write.
    bad = Good;
    loseFabricAfterIdentify = true;
    Try(4, false, 1, 1);
    loseFabricAfterIdentify = false;
    Try(4, true, 2, 1);
    faultOnTransfer = 2;
    for (auto fault : {Empty, Short, Status, Flags, Replay, RequestCrc, ReplyCrc}) {
        bad = fault;
        Try(4, false, 2, 1); // No command is retried after an absent or invalid ACK.
    }
    updateOp = 1;
    bad = Status;
    Try(4, false, 2, 1, true, nullptr);
    bad = Good;
    Try(4, true, 2, 1, true, nullptr);
    std::cout << "PASS: W2 boot gates, WDSP v1.1 Identify and CCLK gate, DSP4 atomic Set and Bypass, and rejected ACKs\n";
}
