#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace omc::osc {

struct Path {
    std::string value;

    Path() = default;
    explicit Path(std::string value) : value(std::move(value)) {}
};

template <class TReturn>
struct TypedPath : Path {
    using Return = TReturn;

    TypedPath() = default;
    explicit TypedPath(std::string value) : Path(std::move(value)) {}
};

struct FaderNumber {
    uint16_t value = 0;
};

FaderNumber faderNumber(uint16_t number);

struct FaderPaths {
    TypedPath<float> getValue(FaderNumber fader) const;
};

struct Paths {
    FaderPaths faders;
};

extern const Paths OMC;

} // namespace omc::osc
