#pragma once

#include "omc-osc/endpoint.hpp"
#include "omc-osc/path.hpp"
#include "omc-osc/value.hpp"

#include <cstddef>
#include <type_traits>
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

    template <class TPath>
    auto arg(const TPath&, size_t index = 0) const -> const typename std::remove_cvref_t<TPath>::Return*
    {
        return arg<typename std::remove_cvref_t<TPath>::Return>(index);
    }
};

} // namespace omc::osc
