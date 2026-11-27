#pragma once

#include <cmath>
#include <cstdio>
#include <cstring>
#include "lvgl.h"
#include "eez/screens.h"

namespace OMC
{

// Faceplate of the "160" compressor mode on the dynamics page, a functional
// model of the documented behaviour: gain reduction meter, below/above
// threshold LEDs and the three controls threshold, compression and output
// gain. The widgets are designed in OpenX32Gui.eez-project
// (user widget dbx160_face, instance dbx160 on the Dyn tab); this class only
// updates them.
class Dbx160Panel {
    public:
        void Bind()
        {
            face = objects.dbx160;
            meter = objects.dbx160__meter;
            needle = objects.dbx160__needle;
            ledBelow = objects.dbx160__led_below;
            ledAbove = objects.dbx160__led_above;
            knobThreshold = {objects.dbx160__threshold_arc, objects.dbx160__threshold_value};
            knobRatio = {objects.dbx160__ratio_arc, objects.dbx160__ratio_value};
            knobOutput = {objects.dbx160__output_arc, objects.dbx160__output_value};
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

        void SetControls(float thresholdDb, float ratio, float outputDb, float thresholdMin, float thresholdMax, float ratioMax, float outputMax)
        {
            char text[24];

            snprintf(text, sizeof(text), "%.1f dB", thresholdDb);
            SetKnob(knobThreshold, (thresholdDb - thresholdMin) / (thresholdMax - thresholdMin), text);

            // the classic dial runs from 1:1 to infinity, so use a log scale
            if (ratio >= ratioMax) {
                snprintf(text, sizeof(text), "INF : 1");
            } else {
                snprintf(text, sizeof(text), "%.1f : 1", ratio);
            }
            SetKnob(knobRatio, logf(ratio) / logf(ratioMax), text);

            snprintf(text, sizeof(text), "%+.1f dB", outputDb);
            SetKnob(knobOutput, outputDb / outputMax, text);
        }

        // Gain reduction as reported by the DSP. Called every 50 ms, so it
        // only touches LVGL when something visible changes: every redraw of
        // the needle re-renders the meter area in software.
        void SetGainReduction(float reductionDb, bool enabled)
        {
            int32_t value = enabled ? (int32_t)lroundf(fminf(fmaxf(reductionDb, 0.0f), 30.0f) * 2.0f) : 0;
            if (value != needleValue) {
                needleValue = value;
                lv_scale_set_line_needle_value(meter, needle, 132, value);
            }

            // the 160 lights ABOVE while it reduces gain, BELOW otherwise
            lv_led_set_brightness(ledBelow, (enabled && value == 0) ? LV_LED_BRIGHT_MAX : LV_LED_BRIGHT_MIN);
            lv_led_set_brightness(ledAbove, (enabled && value > 0) ? LV_LED_BRIGHT_MAX : LV_LED_BRIGHT_MIN);
        }
    private:
        struct KnobParts {
            lv_obj_t* arc;
            lv_obj_t* value;
        };

        lv_obj_t* face = nullptr;
        lv_obj_t* meter = nullptr;
        lv_obj_t* needle = nullptr;
        lv_obj_t* ledBelow = nullptr;
        lv_obj_t* ledAbove = nullptr;
        KnobParts knobThreshold, knobRatio, knobOutput;
        int32_t needleValue = 0;

        void SetKnob(KnobParts& knob, float position, const char* text)
        {
            position = fminf(fmaxf(position, 0.0f), 1.0f);
            lv_arc_set_value(knob.arc, (int32_t)lroundf(position * 1000.0f)); // no redraw if unchanged
            if (strcmp(lv_label_get_text(knob.value), text) != 0) {
                lv_label_set_text(knob.value, text);
            }
        }
};

}
