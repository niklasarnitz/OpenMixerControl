#include "omc-osc/value.hpp"

namespace omc::osc {

Value valueOf(const Serializable& object)
{
    return SerializableRef{object};
}

bool readValue(const Value& value, Serializable& object)
{
    const auto* blob = std::get_if<Blob>(&value);
    return blob && object.deserialize(*blob);
}

std::string toString(const Value& value)
{
    return std::visit(
        [](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, int32_t>) return std::to_string(v);
            if constexpr (std::is_same_v<T, float>) return std::to_string(v);
            if constexpr (std::is_same_v<T, std::string>) return v;
            if constexpr (std::is_same_v<T, bool>) return v ? "true" : "false";
            if constexpr (std::is_same_v<T, Blob>) return "<blob>";
            if constexpr (std::is_same_v<T, SerializableRef>) return "<serializable>";
        },
        value);
}

} // namespace omc::osc
