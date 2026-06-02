#include "simulator-gui.h"

#ifdef BODYLESS_SDL2
#include "enum.h"
#include <algorithm>
#include <string>

X32Ctrl* SimulatorGUI::ctrl = nullptr;
lv_display_t* SimulatorGUI::faders_disp = nullptr;
lv_display_t* SimulatorGUI::buttons_disp = nullptr;
std::vector<SimulatorGUI::VirtualFaderStrip> SimulatorGUI::virtual_faders;
std::vector<SimulatorGUI::VirtualButton> SimulatorGUI::virtual_buttons;

void SimulatorGUI::Init(X32Ctrl* ctrl_inst) {
    ctrl = ctrl_inst;
    
    // Create the separate OS windows
    CreateFadersWindow();
    CreateButtonsWindow();
}

void SimulatorGUI::CreateFadersWindow() {
    // 1. Create the window/display for Fader Section
    faders_disp = lv_sdl_window_create(1000, 350);
    lv_sdl_window_set_title(faders_disp, "OpenX32 - omc - Fader Section");
    
    // 2. Set default display so that objects are created on it
    lv_display_set_default(faders_disp);

    // 3. Create screen and load it
    lv_obj_t* scr = lv_obj_create(nullptr);
    lv_scr_load(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x222222), LV_PART_MAIN);

    // 4. Construct the channel strips
    bool is_full = ctrl->config->IsModelX32FullOrM32();
    int col_idx = 0;

    // Board L (Input Section): 8 channel strips
    for (int i = 0; i < 8; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "IN %d", i + 1);
        BuildFaderStrip(scr, col_idx++, (uint8_t)X32_BOARD::X32_BOARD_L, i, buf);
    }

    // Board M (Mid Section): 8 channel strips (Full console only)
    if (is_full) {
        for (int i = 0; i < 8; i++) {
            char buf[16];
            snprintf(buf, sizeof(buf), "MID %d", i + 1);
            BuildFaderStrip(scr, col_idx++, (uint8_t)X32_BOARD::X32_BOARD_M, i, buf);
        }
    }

    // Board R (Bus Section): 8 channel strips
    for (int i = 0; i < 8; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "BUS %d", i + 1);
        BuildFaderStrip(scr, col_idx++, (uint8_t)X32_BOARD::X32_BOARD_R, i, buf);
    }

    // Board R Main LR Fader
    BuildFaderStrip(scr, col_idx++, (uint8_t)X32_BOARD::X32_BOARD_R, 8, "MAIN LR");
}

void SimulatorGUI::CreateButtonsWindow() {
    // 1. Create the window/display for Console Buttons
    buttons_disp = lv_sdl_window_create(380, 500);
    lv_sdl_window_set_title(buttons_disp, "OpenX32 - omc - Console Buttons");
    
    // 2. Set default display
    lv_display_set_default(buttons_disp);

    // 3. Create screen and load it
    lv_obj_t* scr = lv_obj_create(nullptr);
    lv_scr_load(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1B1B1B), LV_PART_MAIN);

    // 4. Construct Left Fader Layer selects (Board L)
    lv_obj_t* lbl_l = lv_label_create(scr);
    lv_label_set_text(lbl_l, "L Layers");
    lv_obj_set_pos(lbl_l, 10, 10);
    lv_obj_set_style_text_color(lbl_l, lv_color_hex(0x888888), LV_PART_MAIN);

    BuildConsoleButton(scr, "CH 1-16", 10, 35, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_L, 0x01);
    BuildConsoleButton(scr, "CH 17-32", 10, 75, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_L, 0x02);
    BuildConsoleButton(scr, "AUX/USB", 10, 115, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_L, 0x03);
    BuildConsoleButton(scr, "BUS MAST", 10, 155, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_L, 0x04);

    // 5. Construct Right Fader Layer selects (Board R)
    lv_obj_t* lbl_r = lv_label_create(scr);
    lv_label_set_text(lbl_r, "R Layers");
    lv_obj_set_pos(lbl_r, 100, 10);
    lv_obj_set_style_text_color(lbl_r, lv_color_hex(0x888888), LV_PART_MAIN);

    BuildConsoleButton(scr, "DCA", 100, 35, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_R, 0x01);
    BuildConsoleButton(scr, "BUS 1-8", 100, 75, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_R, 0x02);
    BuildConsoleButton(scr, "BUS 9-16", 100, 115, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_R, 0x03);
    BuildConsoleButton(scr, "MTX/MAIN", 100, 155, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_R, 0x04);
    BuildConsoleButton(scr, "SND ON FD", 100, 195, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_R, 0x00);
    BuildConsoleButton(scr, "CLR SOLO", 100, 235, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_R, 0x05);

    // 6. Construct Navigation Buttons (Board Main)
    lv_obj_t* lbl_nav = lv_label_create(scr);
    lv_label_set_text(lbl_nav, "Screen Nav");
    lv_obj_set_pos(lbl_nav, 190, 10);
    lv_obj_set_style_text_color(lbl_nav, lv_color_hex(0x888888), LV_PART_MAIN);

    BuildConsoleButton(scr, "HOME", 190, 35, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x22);
    BuildConsoleButton(scr, "METERS", 190, 75, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x23);
    BuildConsoleButton(scr, "ROUTING", 190, 115, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x24);
    BuildConsoleButton(scr, "SETUP", 190, 155, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x25);
    BuildConsoleButton(scr, "LIBRARY", 190, 195, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x26);
    BuildConsoleButton(scr, "EFFECTS", 190, 235, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x27);
    BuildConsoleButton(scr, "MUTE GRP", 190, 275, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x28);
    BuildConsoleButton(scr, "UTILITY", 190, 315, 80, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x29);

    // 7. Arrow keys (Board Main)
    lv_obj_t* lbl_arr = lv_label_create(scr);
    lv_label_set_text(lbl_arr, "Navigation");
    lv_obj_set_pos(lbl_arr, 280, 10);
    lv_obj_set_style_text_color(lbl_arr, lv_color_hex(0x888888), LV_PART_MAIN);

    BuildConsoleButton(scr, "UP", 310, 50, 35, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x1D);
    BuildConsoleButton(scr, "LEFT", 280, 85, 45, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x1E);
    BuildConsoleButton(scr, "RIGHT", 325, 85, 45, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x1F);
    BuildConsoleButton(scr, "DOWN", 310, 120, 35, 30, (uint8_t)X32_BOARD::X32_BOARD_MAIN, 0x20);
}

void SimulatorGUI::BuildFaderStrip(lv_obj_t* parent, int col_idx, uint8_t boardId, uint8_t faderIndex, const char* default_label) {
    bool is_full = ctrl->config->IsModelX32FullOrM32();
    int num_strips = is_full ? 25 : 17;
    int strip_width = 1000 / num_strips;

    // Fader strip container
    lv_obj_t* strip_container = lv_obj_create(parent);
    lv_obj_set_pos(strip_container, col_idx * strip_width + 1, 5);
    lv_obj_set_size(strip_container, strip_width - 2, 340);
    lv_obj_set_style_pad_all(strip_container, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(strip_container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(strip_container, lv_color_hex(0x444444), LV_PART_MAIN);
    lv_obj_set_style_bg_color(strip_container, lv_color_hex(0x2D2D2D), LV_PART_MAIN);
    lv_obj_remove_flag(strip_container, LV_OBJ_FLAG_SCROLLABLE);

    // Calculate element indices
    uint8_t sel_code = (faderIndex == 8) ? 0x28 : (0x20 + faderIndex);
    uint8_t solo_code = (faderIndex == 8) ? 0x38 : (0x30 + faderIndex);
    uint8_t mute_code = (faderIndex == 8) ? 0x48 : (0x40 + faderIndex);

    // 1. Select Button
    lv_obj_t* sel_btn = lv_button_create(strip_container);
    lv_obj_set_pos(sel_btn, 0, 0);
    lv_obj_set_size(sel_btn, strip_width - 6, 28);
    lv_obj_set_style_radius(sel_btn, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(sel_btn, lv_color_hex(0x3E3E3E), LV_PART_MAIN);
    lv_obj_set_style_bg_color(sel_btn, lv_color_hex(0x0078D7), LV_STATE_CHECKED); // blue glow
    lv_obj_t* sel_lbl = lv_label_create(sel_btn);
    lv_label_set_text(sel_lbl, "SEL");
    lv_obj_set_style_text_font(sel_lbl, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_center(sel_lbl);

    VirtualButton vbtn_sel = { boardId, sel_code, sel_btn };
    uintptr_t sel_idx = virtual_buttons.size();
    virtual_buttons.push_back(vbtn_sel);
    lv_obj_add_event_cb(sel_btn, ButtonEventCB, LV_EVENT_ALL, (void*)sel_idx);

    // 2. Solo Button
    lv_obj_t* solo_btn = lv_button_create(strip_container);
    lv_obj_set_pos(solo_btn, 0, 32);
    lv_obj_set_size(solo_btn, strip_width - 6, 28);
    lv_obj_set_style_radius(solo_btn, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(solo_btn, lv_color_hex(0x3E3E3E), LV_PART_MAIN);
    lv_obj_set_style_bg_color(solo_btn, lv_color_hex(0xD19A0A), LV_STATE_CHECKED); // yellow glow
    lv_obj_t* solo_lbl = lv_label_create(solo_btn);
    lv_label_set_text(solo_lbl, "SOLO");
    lv_obj_set_style_text_font(solo_lbl, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_center(solo_lbl);

    VirtualButton vbtn_solo = { boardId, solo_code, solo_btn };
    uintptr_t solo_idx = virtual_buttons.size();
    virtual_buttons.push_back(vbtn_solo);
    lv_obj_add_event_cb(solo_btn, ButtonEventCB, LV_EVENT_ALL, (void*)solo_idx);

    // 3. Scribble LCD Container
    lv_obj_t* lcd_cont = lv_obj_create(strip_container);
    lv_obj_set_pos(lcd_cont, 0, 64);
    lv_obj_set_size(lcd_cont, strip_width - 6, 40);
    lv_obj_set_style_pad_all(lcd_cont, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(lcd_cont, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(lcd_cont, lv_color_hex(0x111111), LV_PART_MAIN);
    lv_obj_set_style_bg_color(lcd_cont, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_remove_flag(lcd_cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* lcd_lbl = lv_label_create(lcd_cont);
    lv_label_set_text(lcd_lbl, default_label);
    lv_obj_set_style_text_color(lcd_lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    if (strip_width < 45) {
        lv_obj_set_style_text_font(lcd_lbl, &lv_font_montserrat_10, LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_font(lcd_lbl, &lv_font_montserrat_12, LV_PART_MAIN);
    }
    lv_obj_center(lcd_lbl);

    // 4. Mute Button
    lv_obj_t* mute_btn = lv_button_create(strip_container);
    lv_obj_set_pos(mute_btn, 0, 108);
    lv_obj_set_size(mute_btn, strip_width - 6, 28);
    lv_obj_set_style_radius(mute_btn, 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(mute_btn, lv_color_hex(0x3E3E3E), LV_PART_MAIN);
    lv_obj_set_style_bg_color(mute_btn, lv_color_hex(0xE06C75), LV_STATE_CHECKED); // red glow
    lv_obj_t* mute_lbl = lv_label_create(mute_btn);
    lv_label_set_text(mute_lbl, "MUTE");
    lv_obj_set_style_text_font(mute_lbl, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_center(mute_lbl);

    VirtualButton vbtn_mute = { boardId, mute_code, mute_btn };
    uintptr_t mute_idx = virtual_buttons.size();
    virtual_buttons.push_back(vbtn_mute);
    lv_obj_add_event_cb(mute_btn, ButtonEventCB, LV_EVENT_ALL, (void*)mute_idx);

    // 5. Slider
    lv_obj_t* slider = lv_slider_create(strip_container);
    lv_obj_set_pos(slider, (strip_width - 6) / 2 - 8, 140);
    lv_obj_set_size(slider, 16, 190);
    lv_slider_set_range(slider, 0, 4095);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0x1B1B1B), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0x555555), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0xEEEEEE), LV_PART_KNOB);

    uintptr_t fader_idx = virtual_faders.size();
    VirtualFaderStrip vstrip = {
        boardId,
        faderIndex,
        sel_btn,
        solo_btn,
        mute_btn,
        slider,
        lcd_cont,
        lcd_lbl
    };
    virtual_faders.push_back(vstrip);
    lv_obj_add_event_cb(slider, FaderSliderEventCB, LV_EVENT_VALUE_CHANGED, (void*)fader_idx);
}

void SimulatorGUI::BuildConsoleButton(lv_obj_t* parent, const char* label, int x, int y, int w, int h, uint8_t boardId, uint8_t buttonCode) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, w, h);
    lv_obj_set_style_radius(btn, 3, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x353535), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00A0A0), LV_STATE_CHECKED); // greenish/cyan glow for active panel buttons

    lv_obj_t* lbl = lv_label_create(btn);
    lv_label_set_text(lbl, label);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_center(lbl);

    VirtualButton vbtn = { boardId, buttonCode, btn };
    uintptr_t btn_idx = virtual_buttons.size();
    virtual_buttons.push_back(vbtn);
    lv_obj_add_event_cb(btn, ButtonEventCB, LV_EVENT_ALL, (void*)btn_idx);
}

void SimulatorGUI::FaderSliderEventCB(lv_event_t* e) {
    lv_obj_t* slider = (lv_obj_t*)lv_event_get_target(e);
    uintptr_t fader_idx = (uintptr_t)lv_event_get_user_data(e);
    if (fader_idx >= virtual_faders.size()) return;

    VirtualFaderStrip& vstrip = virtual_faders[fader_idx];
    uint16_t val = (uint16_t)lv_slider_get_value(slider);
    
    SendEmulatedEvent(vstrip.boardId, 'f', vstrip.faderIndex, val);
}

void SimulatorGUI::ButtonEventCB(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    uintptr_t btn_idx = (uintptr_t)lv_event_get_user_data(e);
    if (btn_idx >= virtual_buttons.size()) return;

    VirtualButton& vbtn = virtual_buttons[btn_idx];

    if (code == LV_EVENT_PRESSED) {
        SendEmulatedEvent(vbtn.boardId, 'b', 0, vbtn.buttonCode + 0x80);
    } else if (code == LV_EVENT_RELEASED) {
        SendEmulatedEvent(vbtn.boardId, 'b', 0, vbtn.buttonCode);
    }
}

void SimulatorGUI::SendEmulatedEvent(uint8_t boardId, uint8_t classId, uint8_t index, uint16_t value) {
    if (ctrl == nullptr || ctrl->surface == nullptr || ctrl->surface->uart == nullptr) return;

    // 1. Send Board ID Announcement Frame
    char boardMsg[4];
    boardMsg[0] = 0xFE;
    boardMsg[1] = 0x80 | boardId;
    boardMsg[2] = 0xFE;
    int32_t sumBoard = 0xFE - 0xFE - (0x80 | boardId) - 1;
    boardMsg[3] = sumBoard & 0x7F;
    ctrl->surface->uart->WriteEmulatedRx(boardMsg, 4);

    // 2. Send Event Frame
    if (classId == 'b' || classId == 'e') { // Short package
        char eventMsg[5];
        eventMsg[0] = classId;
        eventMsg[1] = index;
        eventMsg[2] = (uint8_t)value;
        eventMsg[3] = 0xFE;
        int32_t sum = 0xFE - classId - index - (uint8_t)value - 2;
        eventMsg[4] = sum & 0x7F;
        ctrl->surface->uart->WriteEmulatedRx(eventMsg, 5);
    } else if (classId == 'f') { // Long package
        char eventMsg[6];
        eventMsg[0] = classId;
        eventMsg[1] = index;
        eventMsg[2] = value & 0xFF;
        eventMsg[3] = (value >> 8) & 0xFF;
        eventMsg[4] = 0xFE;
        int32_t sum = 0xFE - classId - index - (value & 0xFF) - ((value >> 8) & 0xFF) - 3;
        eventMsg[5] = sum & 0x7F;
        ctrl->surface->uart->WriteEmulatedRx(eventMsg, 6);
    }
}

void SimulatorGUI::Tick() {
    if (ctrl == nullptr || ctrl->surface == nullptr || ctrl->surface->uart == nullptr) return;

    // Parse commands from emulated TX buffer
    ParseEmulatedTxData();
}

void SimulatorGUI::ParseEmulatedTxData() {
    char buf[1024];
    size_t len = ctrl->surface->uart->ReadEmulatedTx(buf, sizeof(buf));
    if (len == 0) return;

    static std::vector<uint8_t> parse_buf;
    parse_buf.insert(parse_buf.end(), buf, buf + len);

    size_t i = 0;
    while (i < parse_buf.size()) {
        if (parse_buf[i] != 0xFE) {
            i++;
            continue;
        }

        if (i + 1 >= parse_buf.size()) {
            break; // Needs more bytes
        }

        uint8_t first = parse_buf[i + 1];
        if (first >= 0x80) {
            // Find next end divider 0xFE
            size_t end_idx = i + 2;
            while (end_idx < parse_buf.size() && parse_buf[end_idx] != 0xFE) {
                end_idx++;
            }

            if (end_idx >= parse_buf.size()) {
                break; // Incomplete packet
            }

            if (end_idx + 1 >= parse_buf.size()) {
                break; // Checksum not ready
            }

            uint8_t board = first & 0x7F;
            if (i + 2 < end_idx) {
                uint8_t cls = parse_buf[i + 2];
                uint8_t idx = (i + 3 < end_idx) ? parse_buf[i + 3] : 0;
                
                std::vector<uint8_t> data;
                for (size_t d = i + 4; d < end_idx; d++) {
                    data.push_back(parse_buf[d]);
                }

                ProcessEmulatedOutput(board, cls, idx, data);
            }

            i = end_idx + 2; // Jump past 0xFE and checksum
        } else {
            i++; // Skip invalid
        }
    }

    if (i > 0) {
        parse_buf.erase(parse_buf.begin(), parse_buf.begin() + std::min(i, parse_buf.size()));
    }
}

void SimulatorGUI::ProcessEmulatedOutput(uint8_t boardId, uint8_t classId, uint8_t index, const std::vector<uint8_t>& data) {
    if (classId == 'F') { // Fader update
        if (data.size() < 2) return;
        uint16_t position = ((uint16_t)data[1] << 8) | data[0];

        // Find matching fader strip
        for (auto& vstrip : virtual_faders) {
            if (vstrip.boardId == boardId && vstrip.faderIndex == index) {
                if (!lv_obj_has_state(vstrip.slider, LV_STATE_PRESSED)) {
                    lv_slider_set_value(vstrip.slider, position, LV_ANIM_OFF);
                }
                break;
            }
        }
    } else if (classId == 'L') { // LED update
        if (data.empty()) return;
        uint8_t val = data[0];
        bool ledOn = (val >= 0x80);
        uint8_t buttonCode = ledOn ? (val - 0x80) : val;

        // Search virtual buttons
        for (auto& vbtn : virtual_buttons) {
            if (vbtn.boardId == boardId && vbtn.buttonCode == buttonCode) {
                if (ledOn) {
                    lv_obj_add_state(vbtn.btn_obj, LV_STATE_CHECKED);
                } else {
                    lv_obj_clear_state(vbtn.btn_obj, LV_STATE_CHECKED);
                }
            }
        }
    } else if (classId == 'D') { // Scribble LCD update
        if (data.empty()) return;
        uint8_t color_byte = data[0];
        
        // Map color (bits 0,1,2) to hexadecimal
        // color values: BLACK=0, RED=1, GREEN=2, YELLOW=3, BLUE=4, PINK=5, CYAN=6, WHITE=7
        uint8_t color_idx = color_byte & 0x07;
        uint32_t color_hex = 0x000000;
        switch (color_idx) {
            case 0: color_hex = 0x1A1A1A; break; // Black
            case 1: color_hex = 0x990000; break; // Red
            case 2: color_hex = 0x008000; break; // Green
            case 3: color_hex = 0xD4AF37; break; // Yellow
            case 4: color_hex = 0x000080; break; // Blue
            case 5: color_hex = 0xC000C0; break; // Pink
            case 6: color_hex = 0x008080; break; // Cyan
            case 7: color_hex = 0xCCCCCC; break; // White
        }

        // Parse text blocks starting at index 4 (skip icon metadata)
        std::string display_text;
        size_t d_idx = 4;
        while (d_idx + 3 < data.size()) {
            uint8_t size_and_len = data[d_idx];
            uint8_t len = size_and_len & 0x1F;
            std::string s;
            for (size_t k = 0; k < len && d_idx + 3 + k < data.size(); k++) {
                s += (char)data[d_idx + 3 + k];
            }
            if (!s.empty()) {
                if (!display_text.empty()) display_text += " ";
                display_text += s;
            }
            d_idx += 3 + len;
        }

        // Update virtual fader LCD strip matching board and index
        for (auto& vstrip : virtual_faders) {
            // Index of display matches the channel strip index
            if (vstrip.boardId == boardId && vstrip.faderIndex == index) {
                lv_obj_set_style_bg_color(vstrip.lcd_container, lv_color_hex(color_hex), LV_PART_MAIN);
                lv_label_set_text(vstrip.lcd_label, display_text.c_str());
                
                // Set text color to contrast with the background
                bool is_dark = (color_idx == 0 || color_idx == 4 || color_idx == 1);
                lv_obj_set_style_text_color(vstrip.lcd_label, is_dark ? lv_color_hex(0xFFFFFF) : lv_color_hex(0x000000), LV_PART_MAIN);
                break;
            }
        }
    }
}
#endif
