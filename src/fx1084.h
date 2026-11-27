#pragma once

#include <span>

#include "enum.h"

namespace OMC::ChannelEq1084
{
    inline constexpr unsigned CHANNEL_COUNT = static_cast<unsigned>(X32_VCHANNEL_BLOCK::FXRET);
    inline constexpr float MID_GAIN_NORMAL_LIMIT_DB = 12.0f;
    inline constexpr float MID_GAIN_HI_Q_LIMIT_DB = 18.0f;
    inline constexpr float GAIN_STEP_DB = 0.5f;

    inline constexpr float HP_HZ[] = {0, 45, 70, 160, 360};
    inline constexpr float LP_HZ[] = {0, 6000, 8000, 10000, 14000, 18000};
    inline constexpr float LOW_HZ[] = {0, 35, 60, 110, 220};
    inline constexpr float MID_HZ[] = {0, 360, 700, 1600, 3200, 4800, 7200};
    inline constexpr float HIGH_HZ[] = {0, 10000, 12000, 16000};

    inline constexpr std::span<const float> FREQUENCIES[] = {HP_HZ, LP_HZ, LOW_HZ, MID_HZ, HIGH_HZ};

    constexpr std::span<const float> Frequencies(MP_UOM uom)
    {
        return FREQUENCIES[static_cast<int>(uom) - static_cast<int>(MP_UOM::EQ1084_HP_FREQ)];
    }

    constexpr float FrequencyAt(std::span<const float> detents, float position)
    {
        // Scene loading can restore raw values without going through the parameter bounds.
        return position >= 0 && position < detents.size() ? detents[static_cast<unsigned>(position)] : 0;
    }

}
