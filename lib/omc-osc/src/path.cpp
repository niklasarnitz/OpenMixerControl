#include "omc-osc/path.hpp"

namespace omc::osc {

FaderNumber faderNumber(uint16_t number)
{
    return FaderNumber{number};
}

Path FaderPaths::getValue(FaderNumber fader) const
{
    return Path{"/faders/" + std::to_string(fader.value) + "/value"};
}

const Paths OMC{};

} // namespace omc::osc
