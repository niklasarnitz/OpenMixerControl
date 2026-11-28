// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#ifdef TARGET_WING
#include <array>
#include <cstdint>

namespace OMC {

// From the WDSP v1.1 Identify block; v1.0 firmware has none (extended false).
struct WingDspIdentity {
    bool extended = false;
    uint32_t fault = 0, blocks = 0, coreClockHz = 0; // coreClockHz 0: not measured yet
};

// Throws unless the requested core is boot-qualified and answers WDSP Identify.
WingDspIdentity IdentifyWingDsp(unsigned core);

// Slave TX at 24.576 MHz BCLK needs CCLK >= 393.216 MHz. Throws unless the core
// measured at least that; anything that raises return-valid must call this first.
void RequireWingDspAudioClock(const WingDspIdentity& identity);

// DSP4 pair 0 only. Values have WDSP units; nullptr requests bypass.
void SetWingOverdrive(const std::array<float, 6>* values);

} // namespace OMC
#endif
