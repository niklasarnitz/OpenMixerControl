#pragma once

#include "page.h"
#include "pse-panel.h"

using namespace std;

namespace OMC
{

class PageGate: public Page {
    public:
        PageGate(PageBaseParameter* pagebasepar) : Page(pagebasepar) {
            prevPage = X32_PAGE::CONFIG;
            nextPage = X32_PAGE::COMPRESSOR;
            tabLayer0 = objects.maintab;
            tabIndex0 = 0;
            tabLayer1 = objects.hometab;
            tabIndex1 = 2;
            noLedOnRack = true;
        }

        void OnInit() override {
            // setup Gate-graph
            lv_chart_set_type(objects.current_channel_gate, LV_CHART_TYPE_LINE);
            lv_obj_set_style_size(objects.current_channel_gate, 0, 0, LV_PART_INDICATOR);
            lv_obj_set_style_line_width(objects.current_channel_gate, 5, LV_PART_ITEMS);
            chartSeriesGate = lv_chart_add_series(objects.current_channel_gate, lv_palette_main(LV_PALETTE_AMBER), LV_CHART_AXIS_PRIMARY_Y);
            chartSeriesGateAudio = lv_chart_add_series(objects.current_channel_gate, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_CHART_AXIS_PRIMARY_Y);
            lv_chart_set_div_line_count(objects.current_channel_gate, 5, 7);
            lv_chart_set_range(objects.current_channel_gate, LV_CHART_AXIS_PRIMARY_Y, -6000, 0);
            lv_chart_set_point_count(objects.current_channel_gate, 200); // with incoming audio every 10ms we see 2 seconds of audio-history
            lv_chart_set_series_color(objects.current_channel_gate, chartSeriesGate, lv_color_hex(0xef7900));

            // register draw event callback to change color of audio level depending on gate threshold
            lv_obj_add_event_cb(objects.current_channel_gate, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
            lv_obj_add_flag(objects.current_channel_gate, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

            // store config pointer in user data for use in draw callback
            lv_obj_set_user_data(objects.current_channel_gate, config);
            //chart-shadow: 0x7e4000

            psePanel.Bind();
        }

        void OnShow() override
        {
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_1, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_GATE_TRESHOLD);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_1, MixerparameterAction::TOGGLE_SELECTED_CHANNEL, CHANNEL_GATE_ENABLE);
            config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_6, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_GATE_MODE);

            boundLayout = EncoderLayout(config->GetUint(SELECTED_CHANNEL));
            if (boundLayout != 0)
            {
                // Primary source expander: Threshold, Depth, Time Constant A-F, RMS/Peak
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_2, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_PSE_DEPTH);
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_3, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_PSE_TIMECONSTANT);
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_4, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_PSE_PEAK);
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_4, MixerparameterAction::TOGGLE_SELECTED_CHANNEL, CHANNEL_PSE_PEAK);
                config->SurfaceUnbind(SurfaceElementId::DISPLAY_ENCODER_5);
            }
            else
            {
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_2, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_GATE_RANGE);
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_3, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_GATE_ATTACK);
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_4, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_GATE_HOLD);
                config->SurfaceUnbind(SurfaceElementId::DISPLAY_ENCODER_BUTTON_4);
                config->SurfaceBind(SurfaceElementId::DISPLAY_ENCODER_5, MixerparameterAction::CHANGE_SELECTED_CHANNEL, CHANNEL_GATE_RELEASE);
            }
        }

        void OnChange(bool force_update) override
        {
            using enum MP_ID;

            uint chanIndex = config->GetUint(SELECTED_CHANNEL);

            // the encoder set depends on gate mode and peak switch of the selected channel; rebind only
            // when that layout changes, every SurfaceBind allocates a binding
            // (not while mute-group or utility mode own the encoders; leaving them calls OnShow)
            if (EncoderLayout(chanIndex) != boundLayout &&
                !config->GetBool(DISPLAY_MUTE_GROUP) && !config->GetBool(DISPLAY_UTILITY))
            {
                OnShow();
            }

            if (config->HasParametersChanged({CHANNEL_GATE_ATTACK, CHANNEL_GATE_HOLD, CHANNEL_GATE_RANGE, CHANNEL_GATE_RELEASE, CHANNEL_GATE_TRESHOLD,
                    CHANNEL_GATE_MODE, CHANNEL_PSE_DEPTH}, chanIndex) ||
                config->HasParameterChanged(SELECTED_CHANNEL) ||
                force_update)
            {
                DrawGate(chanIndex);
            }

            if (config->HasParametersChanged({CHANNEL_GATE_MODE, CHANNEL_GATE_ENABLE, CHANNEL_GATE_TRESHOLD,
                    CHANNEL_PSE_DEPTH, CHANNEL_PSE_TIMECONSTANT, CHANNEL_PSE_PEAK}, chanIndex) ||
                config->HasParameterChanged(SELECTED_CHANNEL) ||
                force_update)
            {
                // the PSE faceplate replaces the gate curve
                bool pse = chanIndex < 40 && IsPse(chanIndex);
                psePanel.SetVisible(pse);
                if (pse) {
                    lv_obj_add_flag(objects.current_channel_gate, LV_OBJ_FLAG_HIDDEN);
                    psePanel.SetControls(config->GetBool(CHANNEL_GATE_ENABLE, chanIndex),
                        config->GetUint(CHANNEL_PSE_TIMECONSTANT, chanIndex), config->GetBool(CHANNEL_PSE_PEAK, chanIndex),
                        config->GetFloat(CHANNEL_GATE_TRESHOLD, chanIndex), config->GetFloat(CHANNEL_PSE_DEPTH, chanIndex));
                } else {
                    lv_obj_remove_flag(objects.current_channel_gate, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }

        void OnUpdateMeters() override 
        {
            // TODO: Metering

            int32_t gateValueAudioDbfs = -120;

            uint8_t selectedChannelIndex = config->GetUint(MP_ID::SELECTED_CHANNEL);
            if (selectedChannelIndex < 40) {
                gateValueAudioDbfs = helper->get_dbfs_from_peak_arm_opt(state->rChannel[selectedChannelIndex].meterDecay) * 100.0f;
            }
            
            // add new value to chart
            lv_chart_set_next_value(objects.current_channel_gate, chartSeriesGateAudio, gateValueAudioDbfs);

            // process active: the channel level reaches the threshold (approximated from the meter)
            if (selectedChannelIndex < 40) {
                psePanel.SetActive(gateValueAudioDbfs >= config->GetFloat(CHANNEL_GATE_TRESHOLD, selectedChannelIndex) * 100.0f);
            }

            //lv_chart_refresh(objects.current_channel_gate);
        }

    private:
        lv_chart_series_t* chartSeriesGate;
        lv_chart_series_t* chartSeriesGateAudio;
        PsePanel psePanel;

        // 0 gate, 1 PSE RMS, 2 PSE peak; the peak switch changes the time constant label
        int boundLayout = -1;

        bool IsPse(uint chanIndex) {
            return config->GetUint(CHANNEL_GATE_MODE, chanIndex) == 1;
        }

        int EncoderLayout(uint chanIndex) {
            return IsPse(chanIndex) ? (config->GetBool(CHANNEL_PSE_PEAK, chanIndex) ? 2 : 1) : 0;
        }

        static void draw_event_cb(lv_event_t * e) {
            lv_draw_task_t* draw_task = lv_event_get_draw_task(e);
            lv_draw_dsc_base_t* base_dsc = (lv_draw_dsc_base_t*)lv_draw_task_get_draw_dsc(draw_task);

            if (base_dsc->part != LV_PART_ITEMS) {
                return;
            }

            if (lv_draw_task_get_type(draw_task) != LV_DRAW_TASK_TYPE_LINE) {
                return;
            }

            lv_obj_t* chart = (lv_obj_t*)lv_event_get_target(e);
            lv_obj_t* obj = (lv_obj_t*)lv_event_get_target_obj(e);

            uint32_t series_id = base_dsc->id1;
            //uint32_t point_id = base_dsc->id2; // this does not work for moving charts

            // only change audio-level line
            if (series_id != 1) {
                return; // series_id 1 = audio level
            }

            // find desired series
            lv_chart_series_t* series = lv_chart_get_series_next(chart, NULL);
            for (uint32_t i = 0; i < series_id; i++) {
                series = lv_chart_get_series_next(chart, series);
            }

            lv_draw_line_dsc_t* line_dsc = lv_draw_task_get_line_dsc(draw_task);
            if (line_dsc) {
                // change color depending on gate threshold

                // find the first point (left) of chart and add 199 to get the last point (right) being drawn
                uint32_t point_id = lv_chart_get_x_start_point(chart, series) + lv_chart_get_point_count(chart) - 1;
                if (point_id >= lv_chart_get_point_count(chart)) {
                    point_id -= lv_chart_get_point_count(chart);
                }

                int32_t* y_array = lv_chart_get_series_y_array(chart, series); //lv_chart_get_y_array
                int32_t value = y_array[point_id];

                Config* config = (Config*)lv_obj_get_user_data(obj);
                if (value >= config->GetFloat(CHANNEL_GATE_TRESHOLD, config->GetUint(SELECTED_CHANNEL)) * 100.0f) {
                    line_dsc->color = lv_palette_main(LV_PALETTE_GREEN);
                } else {
                    line_dsc->color = lv_palette_main(LV_PALETTE_RED);
                }
            }
        }

        void DrawGate(uint8_t selectedChannelIndex) {
            if (selectedChannelIndex >= 40) {
                return;
            }

            // calculate the gate curve
            float gateValue[200];
            float inputLevel;
            float outputLevel;

            memset(&gateValue[0], 0, sizeof(gateValue));

            // PSE attenuates by its depth (0..20 dB), the gate by its range
            float reduction = IsPse(selectedChannelIndex) ?
                -config->GetFloat(CHANNEL_PSE_DEPTH, selectedChannelIndex) :
                config->GetFloat(CHANNEL_GATE_RANGE, selectedChannelIndex);

            for (uint16_t pixel = 0; pixel < 200; pixel++) {
                inputLevel = (60.0f * ((float)pixel/199.0f)) - 60.0f; // from -60 dB to 0 dB

                if (inputLevel >= config->GetFloat(CHANNEL_GATE_TRESHOLD, selectedChannelIndex)) {
                    // above threshold
                    outputLevel = inputLevel;
                }else{
                    // below threshold -> apply reduction
                    outputLevel = inputLevel - reduction;
                }

                // scale outputLevel to -6000 .. 0
                gateValue[pixel] = outputLevel * 100.0f;
            }

            int32_t* chartSeriesGatePoints = lv_chart_get_series_y_array(objects.current_channel_gate, chartSeriesGate);
            for (uint16_t i = 0; i < 200; i++) {
                chartSeriesGatePoints[i] = gateValue[i];
            }
            lv_chart_refresh(objects.current_channel_gate);
        }
};

}