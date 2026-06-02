#include "omc-osc/peer.hpp"

#include "lo/lo.h"

#include <cstdlib>
#include <map>
#include <stdexcept>

namespace omc::osc {
namespace {

Result addValue(lo_message msg, const Value& value)
{
    return std::visit(
        [msg](const auto& v) -> Result {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, int32_t>) {
                return lo_message_add_int32(msg, v) < 0 ? Result::error : Result::ok;
            } else if constexpr (std::is_same_v<T, float>) {
                return lo_message_add_float(msg, v) < 0 ? Result::error : Result::ok;
            } else if constexpr (std::is_same_v<T, std::string>) {
                return lo_message_add_string(msg, v.c_str()) < 0 ? Result::error : Result::ok;
            } else if constexpr (std::is_same_v<T, bool>) {
                return (v ? lo_message_add_true(msg) : lo_message_add_false(msg)) < 0 ? Result::error : Result::ok;
            } else if constexpr (std::is_same_v<T, Blob>) {
                lo_blob blob = lo_blob_new(static_cast<int32_t>(v.bytes.size()), v.bytes.data());
                if (!blob) return Result::error;
                const auto result = lo_message_add_blob(msg, blob);
                lo_blob_free(blob);
                return result < 0 ? Result::error : Result::ok;
            } else if constexpr (std::is_same_v<T, SerializableRef>) {
                return addValue(msg, v.object.serialize());
            }
        },
        value);
}

std::vector<Value> readValues(const char* types, lo_arg** argv, int argc)
{
    std::vector<Value> out;

    for (int i = 0; i < argc; ++i) {
        switch (types[i]) {
            case LO_INT32:
                out.emplace_back(static_cast<int32_t>(argv[i]->i32));
                break;
            case LO_FLOAT:
                out.emplace_back(argv[i]->f32);
                break;
            case LO_STRING:
                out.emplace_back(std::string(&argv[i]->s));
                break;
            case LO_TRUE:
                out.emplace_back(true);
                break;
            case LO_FALSE:
                out.emplace_back(false);
                break;
            case LO_BLOB: {
                auto* blob = reinterpret_cast<lo_blob>(argv[i]);
                auto* data = static_cast<const uint8_t*>(lo_blob_dataptr(blob));
                const size_t size = lo_blob_datasize(blob);
                Blob copy;
                if (data && size > 0) copy.bytes.assign(data, data + size);
                out.emplace_back(std::move(copy));
                break;
            }
        }
    }

    return out;
}

Endpoint sourceOf(lo_message msg)
{
    lo_address source = lo_message_get_source(msg);
    const char* host = source ? lo_address_get_hostname(source) : nullptr;
    const char* port = source ? lo_address_get_port(source) : nullptr;
    return {host ? host : "127.0.0.1", static_cast<uint16_t>(port ? std::strtoul(port, nullptr, 10) : 0)};
}

void onError(int, const char*, const char*) {}

int onMessage(const char* path, const char* types, lo_arg** argv, int argc, lo_message raw, void* userData)
{
    auto* ctx = static_cast<std::pair<Peer*, Peer::Handler>*>(userData);
    Message msg;
    msg.path = Path{path ? path : ""};
    msg.remote = sourceOf(raw);
    msg.args = readValues(types ? types : "", argv, argc);
    ctx->second(*ctx->first, msg);
    return 0;
}

} // namespace

struct Peer::Impl {
    lo_server server = nullptr;
    std::map<std::string, Handler> handlers;
    std::map<std::string, std::pair<Peer*, Handler>> contexts;
};

Peer::Peer(uint16_t listenPort) : impl_(std::make_unique<Impl>())
{
    const auto port = std::to_string(listenPort);
    impl_->server = lo_server_new(port.c_str(), onError);
    if (!impl_->server) throw std::runtime_error("could not start OSC peer");
}

Peer::~Peer()
{
    if (impl_ && impl_->server) lo_server_free(impl_->server);
}

Result Peer::route(Path path, Handler handler)
{
    impl_->handlers[path.value] = std::move(handler);
    impl_->contexts[path.value] = {this, impl_->handlers[path.value]};

    return lo_server_add_method(impl_->server, path.value.c_str(), nullptr, onMessage, &impl_->contexts[path.value])
        ? Result::ok
        : Result::error;
}

Result Peer::send(const Endpoint& endpoint, Path path, std::vector<Value> args)
{
    auto* target = lo_address_new(endpoint.host.c_str(), std::to_string(endpoint.port).c_str());
    auto* msg = lo_message_new();
    if (!target || !msg) return Result::error;

    for (const auto& arg : args) {
        if (addValue(msg, arg) != Result::ok) {
            lo_message_free(msg);
            lo_address_free(target);
            return Result::error;
        }
    }

    const auto result = lo_send_message_from(target, impl_->server, path.value.c_str(), msg);
    lo_message_free(msg);
    lo_address_free(target);
    return result < 0 ? Result::error : Result::ok;
}

Result Peer::poll(int timeoutMs)
{
    return lo_server_recv_noblock(impl_->server, timeoutMs) < 0 ? Result::error : Result::ok;
}

} // namespace omc::osc
