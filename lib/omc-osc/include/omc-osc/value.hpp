#pragma once

#include "omc-osc/result.hpp"

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace omc::osc {

struct Blob {
    std::vector<uint8_t> bytes;
};

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual Blob serialize() const = 0;
    virtual bool deserialize(const Blob& blob) = 0;
};

struct SerializableRef {
    const Serializable& object;
};

using Value = std::variant<int32_t, float, std::string, bool, Blob, SerializableRef>;

Value valueOf(const Serializable& object);
bool readValue(const Value& value, Serializable& object);
std::string toString(const Value& value);

} // namespace omc::osc
