#pragma once

#include "omc-osc/omc_osc.hpp"

class ExampleComplexMessage : public omc::osc::Serializable {
public:
    float fader = 0.0f;
    bool mute = false;

    omc::osc::Blob serialize() const override;
    bool deserialize(const omc::osc::Blob& blob) override;
};
