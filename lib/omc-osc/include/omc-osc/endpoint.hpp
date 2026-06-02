#pragma once

#include <cstdint>
#include <string>

namespace omc::osc {

struct Endpoint {
    std::string host = "127.0.0.1";
    uint16_t port = 0;
};

} // namespace omc::osc
