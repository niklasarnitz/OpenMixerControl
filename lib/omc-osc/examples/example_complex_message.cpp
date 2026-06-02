#include "example_complex_message.hpp"

#include <cstring>

struct Payload {
    float fader;
    bool mute;
};

omc::osc::Blob ExampleComplexMessage::serialize() const
{
    Payload payload{fader, mute};
    omc::osc::Blob blob;
    blob.bytes.resize(sizeof(payload));
    std::memcpy(blob.bytes.data(), &payload, sizeof(payload));
    return blob;
}

bool ExampleComplexMessage::deserialize(const omc::osc::Blob& blob)
{
    if (blob.bytes.size() != sizeof(Payload)) return false;

    Payload payload{};
    std::memcpy(&payload, blob.bytes.data(), sizeof(payload));
    fader = payload.fader;
    mute = payload.mute;
    return true;
}
