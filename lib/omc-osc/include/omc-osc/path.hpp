#pragma once

#include <cstdint>
#include <string>

namespace omc::osc {

struct Path {
    std::string value;
};

struct FaderNumber {
    uint16_t value = 0;
};

FaderNumber faderNumber(uint16_t number);

struct FaderPaths {
    Path getValue(FaderNumber fader) const;
};

struct Paths {
    FaderPaths faders;
};

extern const Paths OMC;

} // namespace omc::osc
