#pragma once

#ifdef BODYLESS_SDL2

#include <lvgl/lvgl.h>
#include <vector>
#include <string>
#include "ctrl.h"

// Forward declaration of X32Ctrl
class X32Ctrl;

class SimulatorGUI {
public:
    static void Init(X32Ctrl* ctrl_inst);
    static void Tick();

private:
    static void CreateFadersWindow();
    static void CreateButtonsWindow();
    
    // UI creation helpers
    static void BuildFaderStrip(lv_obj_t* parent, int col_idx, uint8_t boardId, uint8_t faderIndex, const char* default_label);
    static void BuildConsoleButton(lv_obj_t* parent, const char* label, int x, int y, int w, int h, uint8_t boardId, uint8_t buttonCode);

    // Event callbacks
    static void FaderSliderEventCB(lv_event_t* e);
    static void ButtonEventCB(lv_event_t* e);

    // Protocol helper
    static void SendEmulatedEvent(uint8_t boardId, uint8_t classId, uint8_t index, uint16_t value);
    static void ParseEmulatedTxData();
    static void ProcessEmulatedOutput(uint8_t boardId, uint8_t classId, uint8_t index, const std::vector<uint8_t>& data);

    // GUI Widget structure
    struct VirtualFaderStrip {
        uint8_t boardId;
        uint8_t faderIndex;
        lv_obj_t* select_btn;
        lv_obj_t* solo_btn;
        lv_obj_t* mute_btn;
        lv_obj_t* slider;
        lv_obj_t* lcd_container;
        lv_obj_t* lcd_label;
    };

    struct VirtualButton {
        uint8_t boardId;
        uint8_t buttonCode;
        lv_obj_t* btn_obj;
    };

    static X32Ctrl* ctrl;
    static lv_display_t* faders_disp;
    static lv_display_t* buttons_disp;

    static std::vector<VirtualFaderStrip> virtual_faders;
    static std::vector<VirtualButton> virtual_buttons;
};

#endif
