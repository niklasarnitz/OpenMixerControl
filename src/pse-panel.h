#pragma once

#include <cmath>
#include <cstdio>
#include <cstring>
#include "lvgl.h"
#include "defines.h"
#include "eez/screens.h"

namespace OMC
{

// Faceplate of the primary source expander (PSE) mode on the gate page: a
// stylised homage to the classic single-channel feedback-reduction panel with
// its six position time constant switch, threshold and depth controls, the
// process active lamp and the illuminated engage and RMS/peak buttons. The
// widgets are designed in OpenX32Gui.eez-project (user widget pse_face,
// instance pse on the Gate tab); this class only updates them.
class PsePanel {
    public:
        void Bind()
        {
            face = objects.pse;
            time = {objects.pse__time_scale, objects.pse__time_needle, objects.pse__time_value};
            threshold = {objects.pse__threshold_scale, objects.pse__threshold_needle, objects.pse__threshold_value};
            depth = {objects.pse__depth_scale, objects.pse__depth_needle, objects.pse__depth_value};
            SetVisible(false);
        }

        void SetVisible(bool visible)
        {
            if (visible) {
                lv_obj_remove_flag(face, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(face, LV_OBJ_FLAG_HIDDEN);
            }
        }

        void SetControls(bool engage, uint timeConstant, bool peak, float thresholdDb, float depthDb)
        {
            // as DSP1::SendGate applies them: RMS attack = release, peak attack fixed 20 ms
            static const char* rms[6] = {"50 ms", "100 ms", "200 ms", "750 ms", "1.5 s", "3 s"};
            static const char* release[6] = {"20 ms", "200 ms", "1 s", "2 s", "5 s", "5 s"};
            char text[24];

            timeConstant = timeConstant > 5 ? 5 : timeConstant;
            if (peak) {
                snprintf(text, sizeof(text), "20 ms / %s", release[timeConstant]);
            } else {
                snprintf(text, sizeof(text), "%s", rms[timeConstant]);
            }
            SetDial(time, (int32_t)timeConstant, text);

            snprintf(text, sizeof(text), "%.1f dB", (double)thresholdDb);
            SetDial(threshold, (int32_t)lroundf(thresholdDb), text);

            // the depth scale runs 0 .. 20 dB of attenuation from left to right
            snprintf(text, sizeof(text), "%.1f dB", (double)depthDb);
            SetDial(depth, (int32_t)lroundf(-depthDb), text);

            engaged = engage;
            SetLamp(objects.pse__engage_lamp, engaged ? 0x3c8dff : 0x4d5157);
            SetLamp(objects.pse__peak_lamp, peak ? 0xfff1c9 : 0x4d5157);
        }

        // lit while the sensed level reaches the threshold (and the PSE is engaged)
        void SetActive(bool active)
        {
            SetLamp(objects.pse__active_lamp, (engaged && active) ? 0x39e46a : 0x2c5a38);
        }

    private:
        struct Dial {
            lv_obj_t* scale;
            lv_obj_t* needle;
            lv_obj_t* value;
            int32_t needleValue = INT32_MIN;
        };

        lv_obj_t* face = nullptr;
        Dial time, threshold, depth;
        bool engaged = false;

        // the pointer length is designed in EEZ; read it back from the generated line
        static int32_t NeedleLength(lv_obj_t* needle)
        {
            const lv_point_precise_t* p = lv_line_get_points(needle);
            if (p == nullptr || lv_line_get_point_count(needle) < 2) {
                return 36;
            }
            return (int32_t)lroundf(hypotf((float)(p[1].x - p[0].x), (float)(p[1].y - p[0].y)));
        }

        // only touch LVGL when something visible changes: every redraw re-renders in software
        void SetDial(Dial& dial, int32_t value, const char* text)
        {
            if (value != dial.needleValue) {
                dial.needleValue = value;
                lv_scale_set_line_needle_value(dial.scale, dial.needle, NeedleLength(dial.needle), value);
            }
            if (strcmp(lv_label_get_text(dial.value), text) != 0) {
                lv_label_set_text(dial.value, text);
            }
        }

        static void SetLamp(lv_obj_t* lamp, uint32_t color)
        {
            lv_color_t c = lv_color_hex(color);
            if (!lv_color_eq(lv_obj_get_style_bg_color(lamp, LV_PART_MAIN), c)) {
                lv_obj_set_style_bg_color(lamp, c, LV_PART_MAIN);
            }
        }
};

}
