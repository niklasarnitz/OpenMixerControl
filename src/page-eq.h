#pragma once

#include <algorithm>
#include <cmath>
#include <cstring>

#include "fx1084.h"
#include "page.h"
#include "pultec-panel.h"

using namespace std;

namespace OMC
{

class PageEq: public Page {

    using enum MP_ID;

    public:
        PageEq(PageBaseParameter* pagebasepar) : Page(pagebasepar) {
            prevPage = X32_PAGE::COMPRESSOR;
            nextPage = X32_PAGE::SENDS;
            tabLayer0 = objects.maintab;
            tabIndex0 = 0;
            tabLayer1 = objects.hometab;
            tabIndex1 = 4;
            noLedOnRack = true;

            fxmath = new FxMath(pagebasepar);
        }

        void OnInit() override {
            // setup EQ-graph
            lv_chart_set_type(objects.current_channel_eq, LV_CHART_TYPE_LINE);
            lv_obj_set_style_size(objects.current_channel_eq, 0, 0, LV_PART_INDICATOR);
            lv_obj_set_style_line_width(objects.current_channel_eq, 5, LV_PART_ITEMS);
            chartSeriesEQ = lv_chart_add_series(objects.current_channel_eq, lv_palette_main(LV_PALETTE_AMBER), LV_CHART_AXIS_PRIMARY_Y);
            chartSeriesEQPhase = lv_chart_add_series(objects.current_channel_eq, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);
            lv_chart_set_div_line_count(objects.current_channel_eq, 5, 7); // hdiv_num, vdiv_num
            lv_chart_set_range(objects.current_channel_eq, LV_CHART_AXIS_PRIMARY_Y, -15000, 15000);
            lv_chart_set_range(objects.current_channel_eq, LV_CHART_AXIS_SECONDARY_Y, -15000, 15000);
            lv_chart_set_point_count(objects.current_channel_eq, 200);
            lv_chart_set_series_color(objects.current_channel_eq, chartSeriesEQ, lv_color_hex(0xef7900));
            lv_chart_set_series_color(objects.current_channel_eq, chartSeriesEQPhase, lv_color_hex(0x008000));
            //chart-shadow: 0x7e4000

            eqp1aPanel.Bind();
        }

        void OnShow() override
        {
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_1, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_LOWCUT_FREQ);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_1, MixerparameterAction::TOGGLE_SELECTED_CHANNEL, CHANNEL_LOWCUT_ENABLE);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_6, MixerparameterAction::TOGGLE_SELECTED_CHANNEL, CHANNEL_EQ_ENABLE);
            BindModelControls();
            DrawEq();
        }

        void OnChange(bool force_update) override
        {
            const bool rebind = force_update || config->HasParametersChanged({SELECTED_CHANNEL, CHANNEL_EQ_MODEL, BANKING_EQ1084, BANKING_EQP1A});
            if (rebind)
                BindModelControls();
            if (rebind || config->HasParametersChanged(MP_CAT::CHANNEL_EQ) ||
                config->HasParametersChanged(MP_CAT::CHANNEL_EQ1084) || config->HasParametersChanged(MP_CAT::CHANNEL_EQP1A) ||
                config->HasParameterChanged(BANKING_EQ))
            {
                DrawEq();
                SyncEncoderWidgets(true);
            }
        }

    private:
        lv_chart_series_t* chartSeriesEQ;
        lv_chart_series_t* chartSeriesEQPhase;
        FxMath* fxmath;

        static constexpr MP_ID eq1084Banks[][4] = {
            {CHANNEL_EQ1084_LOW_FREQ, CHANNEL_EQ1084_LOW_GAIN, CHANNEL_EQ1084_MID_FREQ, CHANNEL_EQ1084_MID_GAIN},
            {CHANNEL_EQ1084_HIGH_FREQ, CHANNEL_EQ1084_HIGH_GAIN, CHANNEL_EQ1084_HI_Q, CHANNEL_EQ1084_ANALOG},
            {CHANNEL_EQ1084_HP_FREQ, CHANNEL_EQ1084_LP_FREQ, NONE, NONE}
        };
        int32_t eq1084Needles[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

        static constexpr MP_ID eqp1aBanks[][4] = {
            {CHANNEL_EQP1A_LF_FREQ, CHANNEL_EQP1A_LF_BOOST, CHANNEL_EQP1A_LF_ATTEN, CHANNEL_EQP1A_TUBE},
            {CHANNEL_EQP1A_HF_BOOST_FREQ, CHANNEL_EQP1A_HF_BOOST, CHANNEL_EQP1A_HF_BANDWIDTH, CHANNEL_EQP1A_DRIVE},
            {CHANNEL_EQP1A_HF_ATTEN_FREQ, CHANNEL_EQP1A_HF_ATTEN, NONE, NONE}
        };
        PultecPanel eqp1aPanel;

        EQ_MODEL Model(uint channel)
        {
            return Supports1084(channel) ? (EQ_MODEL)config->GetUint(CHANNEL_EQ_MODEL, channel) : EQ_MODEL::PEQ;
        }

        bool Supports1084(uint channel)
        {
            return channel < ChannelEq1084::CHANNEL_COUNT && !config->IsModelAnyWing();
        }

        void BindModelControls()
        {
            const uint channel = config->GetUint(SELECTED_CHANNEL);
            if (Supports1084(channel))
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_5, MixerparameterAction::TOGGLE_SELECTED_CHANNEL, CHANNEL_EQ_MODEL);
            else
                config->SurfaceUnbind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_5);

            const EQ_MODEL model = Model(channel);
            if (model != EQ_MODEL::PEQ)
            {
                const bool eqp1a = model == EQ_MODEL::EQP1A;
                const MP_ID banking = eqp1a ? BANKING_EQP1A : BANKING_EQ1084;
                const auto& bank = eqp1a ? eqp1aBanks[config->GetUint(banking)] : eq1084Banks[config->GetUint(banking)];
                for (uint i = 0; i < std::size(bank); ++i)
                {
                    const auto encoder = static_cast<SurfaceElementId>(static_cast<uint>(SurfaceElementId::DISPLAY_ENCODER_2) + i);
                    if (bank[i] == NONE)
                        config->SurfaceUnbind(encoder);
                    else
                        config->SurfaceBind(encoder, MixerparameterAction::CHANGE_SELECTED_CHANNEL, bank[i]);
                }
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_6, MixerparameterAction::CHANGE, banking);
                return;
            }

            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_2, MixerparameterAction::CHANGE__MP_INDIRECT__SELECTED_CHANNEL, CHANNEL_EQ_FREQ1, (uint)BANKING_EQ);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_3, MixerparameterAction::CHANGE__MP_INDIRECT__SELECTED_CHANNEL, CHANNEL_EQ_GAIN1, (uint)BANKING_EQ);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_4, MixerparameterAction::CHANGE__MP_INDIRECT__SELECTED_CHANNEL, CHANNEL_EQ_Q1, (uint)BANKING_EQ);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_5, MixerparameterAction::CHANGE__MP_INDIRECT__SELECTED_CHANNEL, CHANNEL_EQ_TYPE1, (uint)BANKING_EQ);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_6, MixerparameterAction::CHANGE, BANKING_EQ);
        }

        void Draw1084(uint channel)
        {
            const auto& bank = eq1084Banks[config->GetUint(BANKING_EQ1084)];
            auto inBank = [&](MP_ID id) { return std::find(std::begin(bank), std::end(bank), id) != std::end(bank); };
            auto setText = [](lv_obj_t* label, const char* text) {
                if (std::strcmp(lv_label_get_text(label), text) != 0)
                    lv_label_set_text(label, text);
            };
            auto setTextColor = [](lv_obj_t* label, uint32_t rgb) {
                const auto color = lv_color_hex(rgb);
                if (!lv_color_eq(lv_obj_get_style_text_color(label, LV_PART_MAIN), color))
                    lv_obj_set_style_text_color(label, color, LV_PART_MAIN);
            };
            const struct {
                MP_ID id;
                lv_obj_t* scale;
                lv_obj_t* needle;
                lv_obj_t* value;
                bool gain;
            } dials[] = {
                {CHANNEL_EQ1084_HIGH_FREQ, objects.eq1084__hf_freq_scale, objects.eq1084__hf_freq_needle, objects.eq1084__hf_freq_value, false},
                {CHANNEL_EQ1084_HIGH_GAIN, objects.eq1084__hf_gain_scale, objects.eq1084__hf_gain_needle, objects.eq1084__hf_gain_value, true},
                {CHANNEL_EQ1084_MID_FREQ, objects.eq1084__mf_freq_scale, objects.eq1084__mf_freq_needle, objects.eq1084__mf_freq_value, false},
                {CHANNEL_EQ1084_MID_GAIN, objects.eq1084__mf_gain_scale, objects.eq1084__mf_gain_needle, objects.eq1084__mf_gain_value, true},
                {CHANNEL_EQ1084_LOW_FREQ, objects.eq1084__lf_freq_scale, objects.eq1084__lf_freq_needle, objects.eq1084__lf_freq_value, false},
                {CHANNEL_EQ1084_LOW_GAIN, objects.eq1084__lf_gain_scale, objects.eq1084__lf_gain_needle, objects.eq1084__lf_gain_value, true},
                {CHANNEL_EQ1084_HP_FREQ, objects.eq1084__hp_freq_scale, objects.eq1084__hp_freq_needle, objects.eq1084__hp_freq_value, false},
                {CHANNEL_EQ1084_LP_FREQ, objects.eq1084__lp_freq_scale, objects.eq1084__lp_freq_needle, objects.eq1084__lp_freq_value, false},
            };
            static_assert(std::size(dials) == std::extent_v<decltype(eq1084Needles)>);
            for (size_t i = 0; i < std::size(dials); ++i)
            {
                const auto& dial = dials[i];
                auto* param = config->GetParameter(dial.id);
                const bool midGain = dial.id == CHANNEL_EQ1084_MID_GAIN;
                const float value = midGain ? config->GetEffectiveEq1084MidGain(channel) : config->GetFloat(dial.id, channel);
                const int32_t max = lv_scale_get_range_max_value(dial.scale);
                // Discrete switches follow their EEZ detents; gains sweep the inner cap.
                int32_t position;
                if (dial.gain)
                {
                    const float limit = param->GetMax();
                    const float gain = std::isfinite(value) ? std::clamp(value, -limit, limit) : 0.0f;
                    position = std::lround((gain + limit) * max / (2 * limit));
                }
                else
                    position = value >= 0 && value < max + 1 ? (int32_t)value : 0;
                if (eq1084Needles[i] != position)
                {
                    eq1084Needles[i] = position;
                    lv_scale_set_line_needle_value(dial.scale, dial.needle, dial.gain ? -6 : -16, position);
                }
                setText(dial.value, (midGain ? String(value, 1) + "dB" : param->GetFormatedValue(channel)).c_str());
                setTextColor(dial.value, inBank(dial.id) ? 0xffc36b : 0xf3eddd);
            }

            auto showSwitch = [&](MP_ID id, lv_obj_t* title, lv_obj_t* lamp, lv_obj_t* label, const char* on, const char* off) {
                const bool enabled = config->GetBool(id, channel);
                const uint8_t brightness = enabled ? LV_LED_BRIGHT_MAX : LV_LED_BRIGHT_MIN;
                if (lv_led_get_brightness(lamp) != brightness)
                    lv_led_set_brightness(lamp, brightness);
                setText(label, enabled ? on : off);
                setTextColor(title, inBank(id) ? 0x863a0e : 0x162a34);
            };
            showSwitch(CHANNEL_EQ1084_HI_Q, objects.eq1084__hiq_title, objects.eq1084__hiq_lamp, objects.eq1084__hiq_value, "On", "Off");
            showSwitch(CHANNEL_EQ_ENABLE, objects.eq1084__eq_title, objects.eq1084__eq_lamp, objects.eq1084__eq_value, "In", "Bypass");
            showSwitch(CHANNEL_EQ1084_ANALOG, objects.eq1084__analog_title, objects.eq1084__analog_lamp, objects.eq1084__analog_value, "On", "Off");
        }

        void DrawEqp1a(uint channel)
        {
            const uint bankIndex = config->GetUint(BANKING_EQP1A);
            const auto& bank = eqp1aBanks[bankIndex];
            auto inBank = [&](MP_ID id) { return std::find(std::begin(bank), std::end(bank), id) != std::end(bank); };
            const struct {
                PultecPanel::Control control;
                MP_ID id;
            } controls[] = {
                {PultecPanel::LF_FREQ, CHANNEL_EQP1A_LF_FREQ},
                {PultecPanel::LF_BOOST, CHANNEL_EQP1A_LF_BOOST},
                {PultecPanel::LF_ATTEN, CHANNEL_EQP1A_LF_ATTEN},
                {PultecPanel::HF_BOOST_FREQ, CHANNEL_EQP1A_HF_BOOST_FREQ},
                {PultecPanel::HF_BOOST, CHANNEL_EQP1A_HF_BOOST},
                {PultecPanel::HF_BANDWIDTH, CHANNEL_EQP1A_HF_BANDWIDTH},
                {PultecPanel::HF_ATTEN_FREQ, CHANNEL_EQP1A_HF_ATTEN_FREQ},
                {PultecPanel::HF_ATTEN, CHANNEL_EQP1A_HF_ATTEN},
                {PultecPanel::DRIVE, CHANNEL_EQP1A_DRIVE},
            };
            for (const auto& control : controls)
            {
                auto* param = config->GetParameter(control.id);
                const float min = param->GetMin();
                const float max = param->GetMax();
                const float value = config->GetFloat(control.id, channel);
                const float position = std::isfinite(value) && max > min ? (value - min) / (max - min) : 0.0f;
                eqp1aPanel.SetControl(control.control, position, false, param->GetFormatedValue(channel).c_str(), inBank(control.id));
            }
            const bool in = config->GetBool(CHANNEL_EQ_ENABLE, channel);
            eqp1aPanel.SetControl(PultecPanel::EQ_IN, 0, in, in ? "IN" : "OUT", false);
            const bool tube = config->GetBool(CHANNEL_EQP1A_TUBE, channel);
            eqp1aPanel.SetControl(PultecPanel::TUBE, 0, tube, tube ? "ON" : "OFF", inBank(CHANNEL_EQP1A_TUBE));
            eqp1aPanel.SetBank(bankIndex, std::size(eqp1aBanks));
        }

        void DrawEq() {

            uint selectedChannelIndex = config->GetUint(SELECTED_CHANNEL);
            const EQ_MODEL model = Model(selectedChannelIndex);
            bool show1084 = model == EQ_MODEL::EQ1084;
            bool showEqp1a = model == EQ_MODEL::EQP1A;
            lv_obj_set_flag(objects.eq1084, LV_OBJ_FLAG_HIDDEN, !show1084);
            lv_obj_set_flag(objects.eqp1a, LV_OBJ_FLAG_HIDDEN, !showEqp1a);
            lv_obj_set_flag(objects.current_channel_eq, LV_OBJ_FLAG_HIDDEN, show1084 || showEqp1a);
            if (showEqp1a)
            {
                DrawEqp1a(selectedChannelIndex);
                return;
            }
            if (show1084)
            {
                Draw1084(selectedChannelIndex);
                return;
            }

            // Draw EQ only for normal and aux channels
            if (selectedChannelIndex >= int(X32_VCHANNEL_BLOCK::FXRET)) {
                return;
            }

            bool lowCutEnabled = config->GetBool(CHANNEL_LOWCUT_ENABLE, selectedChannelIndex);
            bool eqEnabled = config->GetBool(CHANNEL_EQ_ENABLE, selectedChannelIndex);

            // calculate the filter-response between 20 Hz and 20 kHz for all 4 PEQs
            float eqValue[200];
            float freq;

            memset(&eqValue[0], 0, sizeof(eqValue));

            // draw the amplitude response over frequency
            int32_t* chartSeriesEqPoints = lv_chart_get_series_y_array(objects.current_channel_eq, chartSeriesEQ);
            
            for (uint16_t pixel = 0; pixel < 200; pixel++) {
                freq = 20.0f * powf(1000.0f, ((float)pixel/199.0f));

                // LowCut
                if (lowCutEnabled)
                {
                    eqValue[pixel] += fxmath->CalcFrequencyResponse_LC(freq, config->GetFloat(CHANNEL_LOWCUT_FREQ, selectedChannelIndex), config->GetUint(SAMPLERATE));
                }

                if (eqEnabled)
                {
                    // PEQ
                    for (uint8_t i_peq = 0; i_peq < MAX_CHAN_EQS; i_peq++)
                    {
                        sPEQ* peq = &state->rChannel[selectedChannelIndex].peq[i_peq];

                        fxmath->RecalcFilterCoefficients_PEQ(peq);

                        eqValue[pixel] += fxmath->CalcFrequencyResponse_PEQ(
                            peq->a[0],
                            peq->a[1],
                            peq->a[2],
                            peq->b[1],
                            peq->b[2],
                            freq,
                            config->GetUint(SAMPLERATE)
                        );
                    }
                }

                // draw point
                chartSeriesEqPoints[pixel] = eqValue[pixel] * 1000.0f; // convert to primary Y-axis range (+/-15 -> +/-15,000)
            }

            // draw the phase response over frequency
            int32_t* chartSeriesEqPhasePoints = lv_chart_get_series_y_array(objects.current_channel_eq, chartSeriesEQPhase);
            for (uint16_t pixel = 0; pixel < 200; pixel++) {
                freq = 20.0f * powf(1000.0f, ((float)pixel/199.0f));
                float phase = 0.0f;

                // LowCut
                if (lowCutEnabled)
                {
                    phase += fxmath->CalcPhaseResponse_LC(freq, config->GetFloat(CHANNEL_LOWCUT_FREQ, selectedChannelIndex));
                }

                if (eqEnabled)
                {
                    // PEQ  
                    for (uint8_t i_peq = 0; i_peq < MAX_CHAN_EQS; i_peq++)
                    {
                        sPEQ* peq = &state->rChannel[selectedChannelIndex].peq[i_peq];

                        phase += fxmath->CalcPhaseResponse_PEQ(
                            peq->a[0],
                            peq->a[1],
                            peq->a[2],
                            1.0f,
                            peq->b[1],
                            peq->b[2],
                            freq,
                            config->GetUint(SAMPLERATE));
                    }
                }

                // limit phase to +/- PI
                while (phase > PI)  phase -= 2.0f * PI;
                while (phase < -PI) phase += 2.0f * PI;

                // draw point
                chartSeriesEqPhasePoints[pixel] = phase * (-15000.0f / PI); // convert to secondary Y-axis range (+/-PI -> +/-15,000)
            }

            lv_chart_refresh(objects.current_channel_eq);
        }
};

}
