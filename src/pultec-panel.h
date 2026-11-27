#pragma once

#include <cmath>
#include <cstdio>
#include <cstring>
#include "lvgl.h"
#include "eez/screens.h"

namespace OMC
{

// Live state of the program EQ faceplate. The faceplate itself (pultec_face
// in the Effects tab) is designed in EEZ Studio; this only moves the pointers,
// lights the lamps, writes the readouts and letters the controls the display
// encoders currently reach in amber.
class PultecPanel {
    public:
        // Control index = position in the FX parameter list of FX_TYPE::PULTEC
        enum Control {
            LF_FREQ, LF_BOOST, LF_ATTEN, HF_BOOST_FREQ, HF_BOOST, HF_BANDWIDTH,
            HF_ATTEN_FREQ, HF_ATTEN, EQ_IN, TUBE, DRIVE, CONTROL_COUNT
        };

        void Bind()
        {
            BindDial(LF_FREQ, objects.pultec_lo_freq_title, objects.pultec_lo_freq_scale, objects.pultec_lo_freq_needle, nullptr);
            BindDial(LF_BOOST, objects.pultec_lo_boost_title, objects.pultec_lo_boost_scale, objects.pultec_lo_boost_needle, objects.pultec_lo_boost_value);
            BindDial(LF_ATTEN, objects.pultec_lo_att_title, objects.pultec_lo_att_scale, objects.pultec_lo_att_needle, objects.pultec_lo_att_value);
            BindDial(HF_BOOST_FREQ, objects.pultec_hi_freq_title, objects.pultec_hi_freq_scale, objects.pultec_hi_freq_needle, nullptr);
            BindDial(HF_BOOST, objects.pultec_hi_boost_title, objects.pultec_hi_boost_scale, objects.pultec_hi_boost_needle, objects.pultec_hi_boost_value);
            BindDial(HF_BANDWIDTH, objects.pultec_hi_width_title, objects.pultec_hi_width_scale, objects.pultec_hi_width_needle, nullptr);
            BindDial(HF_ATTEN_FREQ, objects.pultec_hi_att_freq_title, objects.pultec_hi_att_freq_scale, objects.pultec_hi_att_freq_needle, nullptr);
            BindDial(HF_ATTEN, objects.pultec_hi_att_title, objects.pultec_hi_att_scale, objects.pultec_hi_att_needle, objects.pultec_hi_att_value);
            BindLamp(EQ_IN, objects.pultec_eq_in_title, objects.pultec_eq_in_lamp, objects.pultec_eq_in_value);
            BindLamp(TUBE, objects.pultec_tube_title, objects.pultec_tube_lamp, objects.pultec_tube_value);
            BindDial(DRIVE, objects.pultec_drive_title, objects.pultec_drive_scale, objects.pultec_drive_needle, nullptr);
        }

        void SetVisible(bool visible)
        {
            if (visible) {
                lv_obj_remove_flag(objects.pultec_face, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(objects.pultec_face, LV_OBJ_FLAG_HIDDEN);
            }
        }

        void SetSlot(unsigned slot, unsigned bank, unsigned bankCount)
        {
            char text[48];
            snprintf(text, sizeof(text), "FX%u", slot + 1);
            SetText(objects.pultec_slot, text);
            snprintf(text, sizeof(text), "ENCODERS: BANK %u/%u\n" LV_SYMBOL_UP " " LV_SYMBOL_DOWN " TO CHANGE", bank + 1, bankCount);
            SetText(objects.pultec_bank, text);
        }

        // position: 0..1 along the dial (switch positions for selectors), on: lamps,
        // text: exact value, active: reachable with the display encoders
        void SetControl(Control control, float position, bool on, const char* text, bool active)
        {
            Part& part = parts[control];
            if (part.lamp != nullptr) {
                lv_led_set_brightness(part.lamp, on ? LV_LED_BRIGHT_MAX : LAMP_OFF);
            } else {
                int32_t min = lv_scale_get_range_min_value(part.scale);
                int32_t max = lv_scale_get_range_max_value(part.scale);
                int32_t value = min + (int32_t)lroundf(fminf(fmaxf(position, 0.0f), 1.0f) * (float)(max - min));
                if (value != part.needleValue) {
                    // every needle move re-renders the dial in software, so only on change
                    part.needleValue = value;
                    lv_scale_set_line_needle_value(part.scale, part.needle, part.needleLength, value);
                }
            }
            if (part.value != nullptr) {
                SetText(part.value, text);
            }

            // remember the title color from EEZ as the inactive one
            lv_color_t color = active ? lv_color_hex(ACTIVE) : part.titleColor;
            if (!lv_color_eq(lv_obj_get_style_text_color(part.title, LV_PART_MAIN), color)) {
                lv_obj_set_style_text_color(part.title, color, LV_PART_MAIN);
            }
        }

    private:
        static constexpr uint32_t ACTIVE = 0xffc23d;
        static constexpr uint8_t LAMP_OFF = 40;

        struct Part {
            lv_obj_t* title = nullptr;
            lv_obj_t* scale = nullptr;
            lv_obj_t* needle = nullptr;
            lv_obj_t* lamp = nullptr;
            lv_obj_t* value = nullptr;
            lv_color_t titleColor;
            int32_t needleLength = 0;
            int32_t needleValue = INT32_MIN;
        };

        Part parts[CONTROL_COUNT];

        void BindDial(Control control, lv_obj_t* title, lv_obj_t* scale, lv_obj_t* needle, lv_obj_t* value)
        {
            Part& part = parts[control];
            part.title = title;
            part.titleColor = lv_obj_get_style_text_color(title, LV_PART_MAIN);
            part.scale = scale;
            part.needle = needle;
            part.value = value;

            // keep the needle length set in EEZ: the needle runs from the dial centre to its tip
            const lv_point_precise_t* points = lv_line_get_points(needle);
            if (points != nullptr && lv_line_get_point_count(needle) >= 2) {
                part.needleLength = (int32_t)lroundf(hypotf((float)(points[1].x - points[0].x), (float)(points[1].y - points[0].y)));
            }
        }

        void BindLamp(Control control, lv_obj_t* title, lv_obj_t* lamp, lv_obj_t* value)
        {
            Part& part = parts[control];
            part.title = title;
            part.titleColor = lv_obj_get_style_text_color(title, LV_PART_MAIN);
            part.lamp = lamp;
            part.value = value;
        }

        static void SetText(lv_obj_t* label, const char* text)
        {
            if (strcmp(lv_label_get_text(label), text) != 0) {
                lv_label_set_text(label, text);
            }
        }
};

}
