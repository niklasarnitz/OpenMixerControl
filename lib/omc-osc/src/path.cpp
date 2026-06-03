#include "omc-osc/path.hpp"

namespace omc::osc {

FaderNumber faderNumber(uint16_t number)
{
    return FaderNumber{number};
}

TypedPath<float> FaderPaths::getValue(FaderNumber fader) const
{
    return TypedPath<float>{"/faders/" + std::to_string(fader.value) + "/value"};
}

const Paths OMC{};

} // namespace omc::osc
