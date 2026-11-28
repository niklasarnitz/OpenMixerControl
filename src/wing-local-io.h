#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <span>

namespace OMC::WingLocalIo {

using HeadampImage = std::array<uint8_t, 27>;
// Offline image edit only. Invalid indices/steps leave the image untouched.
bool EncodeHeadamp(HeadampImage& image, unsigned ha, unsigned step, bool phantom);

// A complete replacement, in physical socket order. Never invent settings for
// omitted sockets: the write-only shift chain latches all 24 at once.
std::string HeadampPayload(std::span<const std::string> gains);

// Stock reference metadata, never a SPIBone routing or write capability.
const std::string& ReferenceJson();

}
