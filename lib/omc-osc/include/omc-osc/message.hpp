#pragma once

#include "omc-osc/endpoint.hpp"
#include "omc-osc/path.hpp"
#include "omc-osc/value.hpp"

#include <vector>

namespace omc::osc {

struct Message {
    Path path;
    std::vector<Value> args;
    Endpoint remote;

    template <class T>
    const T* arg(size_t index) const
    {
        return index < args.size() ? std::get_if<T>(&args[index]) : nullptr;
    }
};

} // namespace omc::osc
