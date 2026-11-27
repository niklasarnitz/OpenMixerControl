#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

screen_main_state_t screen_main_state;

// Global state variables

lv_style_t meter_section_main_style;
static bool meter_section_main_style_initialized;
lv_style_t meter_section_items_style;
static bool meter_section_items_style_initialized;
lv_style_t meter_section_indicator_style;
static bool meter_section_indicator_style_initialized;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_main(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: grp_KEY
        lv_group_remove_all_objs(groups.grp_KEY);
        lv_group_add_obj(groups.grp_KEY, objects.channel_box);
    }
}

//
// Screens
//

void create_screen_main() {
    screen_main_state_t *state = &screen_main_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, 0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // channel_box
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.channel_box = obj;
            lv_obj_set_pos(obj, 3, 3);
            lv_obj_set_size(obj, 94, 72);
            lv_obj_add_event_cb(obj, action_action_key, LV_EVENT_KEY, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // current_channel_number
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.current_channel_number = obj;
                    lv_obj_set_pos(obj, -19, 15);
                    lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "CH32");
                }
                {
                    // current_channel_name
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.current_channel_name = obj;
                    lv_obj_set_pos(obj, -19, 33);
                    lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Kanal 32");
                }
                {
                    // current_channel_color
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.current_channel_color = obj;
                    lv_obj_set_pos(obj, 61, -22);
                    lv_obj_set_size(obj, 11, 72);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // header_ip
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.header_ip = obj;
            lv_obj_set_pos(obj, 102, 3);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "IP:");
        }
        {
            // header_debug
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.header_debug = obj;
            lv_obj_set_pos(obj, 101, 39);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "DEBUG:");
        }
        {
            // header_aes50_info
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.header_aes50_info = obj;
            lv_obj_set_pos(obj, 102, 20);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "AES50: ");
        }
        {
            // header_statustext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.header_statustext = obj;
            lv_obj_set_pos(obj, 101, 58);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Status");
        }
        {
            // maintab
            lv_obj_t *obj = lv_tabview_create(parent_obj);
            objects.maintab = obj;
            lv_obj_set_pos(obj, LV_PCT(0), 78);
            lv_obj_set_size(obj, LV_PCT(100), 402);
            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
            lv_tabview_set_tab_bar_size(obj, 0);
            add_style_tabview_nopad(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Home");
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hometab
                            lv_obj_t *obj = lv_tabview_create(parent_obj);
                            objects.hometab = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
                            lv_tabview_set_tab_bar_size(obj, 20);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            add_style_tabview_nopad(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Home");
                                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // home_channelstrip_1
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.home_channelstrip_1 = obj;
                                            lv_obj_set_pos(obj, 0, 12);
                                            lv_obj_set_size(obj, 130, 306);
                                            add_style_panel_flat_bg(obj);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // home_ch_1
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.home_ch_1 = obj;
                                                    lv_obj_set_pos(obj, 0, 39);
                                                    lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                                {
                                                    // home_vumeter_1
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.home_vumeter_1 = obj;
                                                    lv_obj_set_pos(obj, 24, 82);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj0 = obj;
                                                    lv_obj_set_pos(obj, 70, 91);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // home_channelstrip_2
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.home_channelstrip_2 = obj;
                                            lv_obj_set_pos(obj, 134, 12);
                                            lv_obj_set_size(obj, 130, 306);
                                            add_style_panel_flat_bg(obj);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // home_ch_2
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.home_ch_2 = obj;
                                                    lv_obj_set_pos(obj, 0, 39);
                                                    lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                                {
                                                    // home_vumeter_2
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.home_vumeter_2 = obj;
                                                    lv_obj_set_pos(obj, 24, 82);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj1 = obj;
                                                    lv_obj_set_pos(obj, 70, 91);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // home_channelstrip_3
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.home_channelstrip_3 = obj;
                                            lv_obj_set_pos(obj, 268, 12);
                                            lv_obj_set_size(obj, 130, 306);
                                            add_style_panel_flat_bg(obj);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // home_ch_3
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.home_ch_3 = obj;
                                                    lv_obj_set_pos(obj, 0, 39);
                                                    lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                                {
                                                    // home_vumeter_3
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.home_vumeter_3 = obj;
                                                    lv_obj_set_pos(obj, 24, 82);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj2 = obj;
                                                    lv_obj_set_pos(obj, 70, 91);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // home_channelstrip_4
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.home_channelstrip_4 = obj;
                                            lv_obj_set_pos(obj, 402, 12);
                                            lv_obj_set_size(obj, 130, 306);
                                            add_style_panel_flat_bg(obj);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // home_ch_4
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.home_ch_4 = obj;
                                                    lv_obj_set_pos(obj, 0, 39);
                                                    lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                                {
                                                    // home_vumeter_4
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.home_vumeter_4 = obj;
                                                    lv_obj_set_pos(obj, 24, 82);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj3 = obj;
                                                    lv_obj_set_pos(obj, 70, 91);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // home_channelstrip_5
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.home_channelstrip_5 = obj;
                                            lv_obj_set_pos(obj, 536, 12);
                                            lv_obj_set_size(obj, 130, 306);
                                            add_style_panel_flat_bg(obj);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // home_ch_5
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.home_ch_5 = obj;
                                                    lv_obj_set_pos(obj, 0, 39);
                                                    lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                                {
                                                    // home_vumeter_5
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.home_vumeter_5 = obj;
                                                    lv_obj_set_pos(obj, 24, 82);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj4 = obj;
                                                    lv_obj_set_pos(obj, 70, 91);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // home_channelstrip_6
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.home_channelstrip_6 = obj;
                                            lv_obj_set_pos(obj, 670, 12);
                                            lv_obj_set_size(obj, 130, 306);
                                            add_style_panel_flat_bg(obj);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // home_ch_6
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.home_ch_6 = obj;
                                                    lv_obj_set_pos(obj, 0, 39);
                                                    lv_obj_set_size(obj, 130, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                                {
                                                    // home_vumeter_6
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.home_vumeter_6 = obj;
                                                    lv_obj_set_pos(obj, 24, 82);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj5 = obj;
                                                    lv_obj_set_pos(obj, 70, 91);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    // configtab
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Config");
                                    objects.configtab = obj;
                                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, 800, 322);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // current_channel_source_1
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.current_channel_source_1 = obj;
                                                    lv_obj_set_pos(obj, 563, 27);
                                                    lv_obj_set_size(obj, 209, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x264056), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "OUT");
                                                }
                                                {
                                                    // config_vumeter
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_vumeter = obj;
                                                    lv_obj_set_pos(obj, 18, 17);
                                                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_vumeter_colored);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    // config_gain_knob
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_gain_knob = obj;
                                                    lv_obj_set_pos(obj, 162, 182);
                                                    lv_obj_set_size(obj, 75, 75);
                                                    lv_image_set_src(obj, &img_knob);
                                                    lv_image_set_scale(obj, 192);
                                                    lv_obj_set_style_transform_pivot_x(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_transform_pivot_y(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_transform_rotation(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    // config_pan_knob
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_pan_knob = obj;
                                                    lv_obj_set_pos(obj, 697, 184);
                                                    lv_obj_set_size(obj, 75, 75);
                                                    lv_image_set_src(obj, &img_knob);
                                                    lv_image_set_scale(obj, 192);
                                                    lv_obj_set_style_transform_pivot_x(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_transform_pivot_y(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_transform_rotation(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    // config_volume_knob
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_volume_knob = obj;
                                                    lv_obj_set_pos(obj, 563, 183);
                                                    lv_obj_set_size(obj, 75, 75);
                                                    lv_image_set_src(obj, &img_knob);
                                                    lv_image_set_scale(obj, 192);
                                                    lv_obj_set_style_transform_pivot_x(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_transform_pivot_y(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_transform_rotation(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    // current_channel_source
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.current_channel_source = obj;
                                                    lv_obj_set_pos(obj, 114, 26);
                                                    lv_obj_set_size(obj, 189, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x264056), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "IN");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 149, 71);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "48V");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 149, 100);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "INVERT");
                                                }
                                                {
                                                    // current_channel_pan_bal
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.current_channel_pan_bal = obj;
                                                    lv_obj_set_pos(obj, 697, 163);
                                                    lv_obj_set_size(obj, 75, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "0");
                                                }
                                                {
                                                    // current_channel_volume
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.current_channel_volume = obj;
                                                    lv_obj_set_pos(obj, 563, 163);
                                                    lv_obj_set_size(obj, 75, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "0.0dBfs");
                                                }
                                                {
                                                    // config_phantom_checkbox
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_phantom_checkbox = obj;
                                                    lv_obj_set_pos(obj, 114, 65);
                                                    lv_obj_set_size(obj, 32, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_checkbox);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    // config_phase_checkbox
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_phase_checkbox = obj;
                                                    lv_obj_set_pos(obj, 114, 94);
                                                    lv_obj_set_size(obj, 32, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_checkbox);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    // config_mute_checkbox
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_mute_checkbox = obj;
                                                    lv_obj_set_pos(obj, 114, 123);
                                                    lv_obj_set_size(obj, 32, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_checkbox);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 148, 129);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "MUTE");
                                                }
                                                {
                                                    // current_channel_gain
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.current_channel_gain = obj;
                                                    lv_obj_set_pos(obj, 167, 163);
                                                    lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "0.0dB");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 151, 260);
                                                    lv_obj_set_size(obj, 97, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "GAIN");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 284, 127);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Mute Groups");
                                                }
                                                {
                                                    // list_mute_groups
                                                    lv_obj_t *obj = lv_list_create(parent_obj);
                                                    objects.list_mute_groups = obj;
                                                    lv_obj_set_pos(obj, 290, 151);
                                                    lv_obj_set_size(obj, 83, 148);
                                                    lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            // config_mute_group_1
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_mute_group_1 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "1");
                                                        }
                                                        {
                                                            // config_mute_group_2
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_mute_group_2 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "2");
                                                        }
                                                        {
                                                            // config_mute_group_3
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_mute_group_3 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "3");
                                                        }
                                                        {
                                                            // config_mute_group_4
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_mute_group_4 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "4");
                                                        }
                                                        {
                                                            // config_mute_group_5
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_mute_group_5 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "5");
                                                        }
                                                        {
                                                            // config_mute_group_6
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_mute_group_6 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "6");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 553, 260);
                                                    lv_obj_set_size(obj, 97, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "VOLUME");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 687, 260);
                                                    lv_obj_set_size(obj, 97, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text_static(obj, "PAN");
                                                }
                                                {
                                                    // openx32_demo_knob
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.openx32_demo_knob = obj;
                                                    lv_obj_set_pos(obj, 611, 56);
                                                    lv_obj_set_size(obj, 111, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_openx32_knob);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                                    objects.obj6 = obj;
                                                    lv_obj_set_pos(obj, 61, 27);
                                                    lv_obj_set_size(obj, 36, 191);
                                                    lv_scale_set_mode(obj, LV_SCALE_MODE_VERTICAL_RIGHT);
                                                    lv_scale_set_range(obj, 10, 40);
                                                    lv_scale_set_angle_range(obj, 270);
                                                    lv_scale_set_rotation(obj, 135);
                                                    lv_scale_set_total_tick_count(obj, 31);
                                                    lv_scale_set_major_tick_every(obj, 5);
                                                    lv_scale_set_label_show(obj, true);
                                                    static const char *label_texts[8] = {
                                                        "SIG",
                                                        "-30",
                                                        "-18",
                                                        "-12",
                                                        "-6",
                                                        "-3",
                                                        "CLIP",
                                                        NULL
                                                    };
                                                    lv_scale_set_text_src(obj, label_texts);
                                                    lv_obj_set_style_text_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_ITEMS | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x555555), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 424, 92);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "DCA Groups");
                                                }
                                                {
                                                    // list_dca_groups
                                                    lv_obj_t *obj = lv_list_create(parent_obj);
                                                    objects.list_dca_groups = obj;
                                                    lv_obj_set_pos(obj, 427, 118);
                                                    lv_obj_set_size(obj, 83, 184);
                                                    lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            // config_dca_group_1
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_1 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "1");
                                                        }
                                                        {
                                                            // config_dca_group_2
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_2 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "2");
                                                        }
                                                        {
                                                            // config_dca_group_3
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_3 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "3");
                                                        }
                                                        {
                                                            // config_dca_group_4
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_4 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "4");
                                                        }
                                                        {
                                                            // config_dca_group_5
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_5 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "5");
                                                        }
                                                        {
                                                            // config_dca_group_6
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_6 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "6");
                                                        }
                                                        {
                                                            // config_dca_group_7
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_7 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "7");
                                                        }
                                                        {
                                                            // config_dca_group_8
                                                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                                                            objects.config_dca_group_8 = obj;
                                                            lv_obj_set_pos(obj, 0, 0);
                                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                            lv_checkbox_set_text_static(obj, "8");
                                                        }
                                                    }
                                                }
                                                {
                                                    // config_phantom_button
                                                    lv_obj_t *obj = lv_image_create(parent_obj);
                                                    objects.config_phantom_button = obj;
                                                    lv_obj_set_pos(obj, 223, 47);
                                                    lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                                                    lv_image_set_src(obj, &img_buttononoff);
                                                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Gate");
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // current_channel_gate
                                            lv_obj_t *obj = lv_chart_create(parent_obj);
                                            objects.current_channel_gate = obj;
                                            lv_obj_set_pos(obj, 25, 25);
                                            lv_obj_set_size(obj, 200, 200);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dyn");
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // current_channel_comp
                                            lv_obj_t *obj = lv_chart_create(parent_obj);
                                            objects.current_channel_comp = obj;
                                            lv_obj_set_pos(obj, 25, 25);
                                            lv_obj_set_size(obj, 200, 200);
                                        }
                                        {
                                            // dbx160
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.dbx160 = obj;
                                            lv_obj_set_pos(obj, 10, 10);
                                            lv_obj_set_size(obj, 780, 300);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_dbx160_face(obj, 70, &state->dbx160);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                        }
                                    }
                                }
                                {
                                    // eqtab
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "EQ");
                                    objects.eqtab = obj;
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // current_channel_eq
                                            lv_obj_t *obj = lv_chart_create(parent_obj);
                                            objects.current_channel_eq = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), 4);
                                            lv_obj_set_size(obj, LV_PCT(100), 314);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // eq1084
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.eq1084 = obj;
                                            lv_obj_set_pos(obj, 10, 9);
                                            lv_obj_set_size(obj, 780, 300);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_eq1084_face(obj, 103);
                                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Sends");
                                    add_style_tab_nopad(obj);
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Main");
                                    add_style_tab_nopad(obj);
                                }
                            }
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Meters");
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // meterstab
                            lv_obj_t *obj = lv_tabview_create(parent_obj);
                            objects.meterstab = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
                            lv_tabview_set_tab_bar_size(obj, 20);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            add_style_tabview_nopad(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Meters");
                                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // meters_1_8
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_1_8 = obj;
                                            lv_obj_set_pos(obj, 0, 12);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 210);
                                        }
                                        {
                                            // meters_9_16
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_9_16 = obj;
                                            lv_obj_set_pos(obj, 160, 12);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 283);
                                        }
                                        {
                                            // meters_17_24
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_17_24 = obj;
                                            lv_obj_set_pos(obj, 320, 12);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 356);
                                        }
                                        {
                                            // meters_25_32
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_25_32 = obj;
                                            lv_obj_set_pos(obj, 480, 12);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 429);
                                        }
                                        {
                                            // meters_aux
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_aux = obj;
                                            lv_obj_set_pos(obj, 640, 12);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 502);
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 6, 133);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "1     2     3    4    5    6     7    8");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 167, 133);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "9   10    11   12   13   14   15   16");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 324, 133);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "17   18   19  20  21   22  23  24");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 483, 133);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "25  26  27  28  29  30  31  32");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 647, 133);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "1     2     3    4    5    6    7     8");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 295, 150);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Channel");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 706, 150);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "AUX");
                                        }
                                        {
                                            // meters_fxret
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_fxret = obj;
                                            lv_obj_set_pos(obj, 20, 184);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 575);
                                        }
                                        {
                                            // meters_bus_1_8
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_bus_1_8 = obj;
                                            lv_obj_set_pos(obj, 216, 184);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 648);
                                        }
                                        {
                                            // meters_bus_9_16
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_bus_9_16 = obj;
                                            lv_obj_set_pos(obj, 376, 184);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 721);
                                        }
                                        {
                                            // meters_matrix
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.meters_matrix = obj;
                                            lv_obj_set_pos(obj, 575, 184);
                                            lv_obj_set_size(obj, 160, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar8(obj, 794);
                                        }
                                        {
                                            // ms_main_lr
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.ms_main_lr = obj;
                                            lv_obj_set_pos(obj, 749, 184);
                                            lv_obj_set_size(obj, 20, 120);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            create_user_widget_ms_bar(obj, 867);
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 26, 306);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "1     2     3    4    5    6     7    8");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 222, 306);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "1     2     3    4    5    6     7    8");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 382, 306);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "9   10    11   12   13   14   15   16");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 582, 306);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "1     2     3    4    5    6    S  M/C");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 61, 323);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "DSP2 Return");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 355, 322);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Mixbus");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 617, 323);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Matrix");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 746, 306);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "L/R\nMain");
                                        }
                                    }
                                }
                                {
                                    // configtab_1
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "RTA");
                                    objects.configtab_1 = obj;
                                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_scale_create(parent_obj);
                                            objects.obj7 = obj;
                                            lv_obj_set_pos(obj, 78, 295);
                                            lv_obj_set_size(obj, 640, 26);
                                            lv_scale_set_mode(obj, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
                                            lv_scale_set_range(obj, 10, 40);
                                            lv_scale_set_angle_range(obj, 270);
                                            lv_scale_set_rotation(obj, 135);
                                            lv_scale_set_total_tick_count(obj, 25);
                                            lv_scale_set_major_tick_every(obj, 6);
                                            lv_scale_set_label_show(obj, true);
                                            static const char *label_texts[6] = {
                                                "0 Hz",
                                                " 600 Hz",
                                                " 2.5 kHz",
                                                " 8 kHz",
                                                " 20 kHz",
                                                NULL
                                            };
                                            lv_scale_set_text_src(obj, label_texts);
                                        }
                                        {
                                            // rta_chart
                                            lv_obj_t *obj = lv_chart_create(parent_obj);
                                            objects.rta_chart = obj;
                                            lv_obj_set_pos(obj, 78, 12);
                                            lv_obj_set_size(obj, 640, 275);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Routing");
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // routingtab
                            lv_obj_t *obj = lv_tabview_create(parent_obj);
                            objects.routingtab = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
                            lv_tabview_set_tab_bar_size(obj, 20);
                            add_style_tabview_nopad(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Inputs");
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // routing_in_devices
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_in_devices = obj;
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, 800, 28);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj8 = obj;
                                                    lv_obj_set_pos(obj, 3, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj9 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj10 = obj;
                                                    lv_obj_set_pos(obj, 117, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj11 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj12 = obj;
                                                    lv_obj_set_pos(obj, 231, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj13 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj14 = obj;
                                                    lv_obj_set_pos(obj, 345, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj15 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj16 = obj;
                                                    lv_obj_set_pos(obj, 459, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj17 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj18 = obj;
                                                    lv_obj_set_pos(obj, 573, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj19 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj20 = obj;
                                                    lv_obj_set_pos(obj, 687, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj21 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        {
                                            // routing_in_header
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_in_header = obj;
                                            lv_obj_set_pos(obj, 0, 28);
                                            lv_obj_set_size(obj, 800, 44);
                                            create_user_widget_routing_xp_header(obj, 895);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // routing_in_rows
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_in_rows = obj;
                                            lv_obj_set_pos(obj, 0, 72);
                                            lv_obj_set_size(obj, 800, 242);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj22 = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 918);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj23 = obj;
                                                    lv_obj_set_pos(obj, 0, 22);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 937);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj24 = obj;
                                                    lv_obj_set_pos(obj, 0, 44);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 956);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj25 = obj;
                                                    lv_obj_set_pos(obj, 0, 66);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 975);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj26 = obj;
                                                    lv_obj_set_pos(obj, 0, 88);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 994);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj27 = obj;
                                                    lv_obj_set_pos(obj, 0, 110);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1013);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj28 = obj;
                                                    lv_obj_set_pos(obj, 0, 132);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1032);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj29 = obj;
                                                    lv_obj_set_pos(obj, 0, 154);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1051);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj30 = obj;
                                                    lv_obj_set_pos(obj, 0, 176);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1070);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj31 = obj;
                                                    lv_obj_set_pos(obj, 0, 198);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1089);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj32 = obj;
                                                    lv_obj_set_pos(obj, 0, 220);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1108);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                            }
                                        }
                                        {
                                            // routing_in_scroll
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_in_scroll = obj;
                                            lv_obj_set_pos(obj, 797, 72);
                                            lv_obj_set_size(obj, 3, 22);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x5a5a5a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Outputs");
                                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // routing_out_devices
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_out_devices = obj;
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, 800, 28);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj33 = obj;
                                                    lv_obj_set_pos(obj, 3, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj34 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj35 = obj;
                                                    lv_obj_set_pos(obj, 117, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj36 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj37 = obj;
                                                    lv_obj_set_pos(obj, 231, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj38 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj39 = obj;
                                                    lv_obj_set_pos(obj, 345, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj40 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj41 = obj;
                                                    lv_obj_set_pos(obj, 459, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj42 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj43 = obj;
                                                    lv_obj_set_pos(obj, 573, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj44 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj45 = obj;
                                                    lv_obj_set_pos(obj, 687, 3);
                                                    lv_obj_set_size(obj, 108, 22);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    {
                                                        lv_obj_t *parent_obj = obj;
                                                        {
                                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                                            objects.obj46 = obj;
                                                            lv_obj_set_pos(obj, 0, 3);
                                                            lv_obj_set_size(obj, 108, LV_SIZE_CONTENT);
                                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xbbbbbb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                                            lv_label_set_text(obj, "Device");
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        {
                                            // routing_out_header
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_out_header = obj;
                                            lv_obj_set_pos(obj, 0, 28);
                                            lv_obj_set_size(obj, 800, 44);
                                            create_user_widget_routing_xp_header(obj, 1143);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // routing_out_rows
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_out_rows = obj;
                                            lv_obj_set_pos(obj, 0, 72);
                                            lv_obj_set_size(obj, 800, 242);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj47 = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1166);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj48 = obj;
                                                    lv_obj_set_pos(obj, 0, 22);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1185);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj49 = obj;
                                                    lv_obj_set_pos(obj, 0, 44);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1204);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj50 = obj;
                                                    lv_obj_set_pos(obj, 0, 66);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1223);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj51 = obj;
                                                    lv_obj_set_pos(obj, 0, 88);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1242);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj52 = obj;
                                                    lv_obj_set_pos(obj, 0, 110);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1261);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj53 = obj;
                                                    lv_obj_set_pos(obj, 0, 132);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1280);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj54 = obj;
                                                    lv_obj_set_pos(obj, 0, 154);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1299);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj55 = obj;
                                                    lv_obj_set_pos(obj, 0, 176);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1318);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj56 = obj;
                                                    lv_obj_set_pos(obj, 0, 198);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1337);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj57 = obj;
                                                    lv_obj_set_pos(obj, 0, 220);
                                                    lv_obj_set_size(obj, 800, 22);
                                                    create_user_widget_routing_xp_row(obj, 1356);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x16324a), LV_PART_MAIN | LV_STATE_CHECKED);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                                                }
                                            }
                                        }
                                        {
                                            // routing_out_scroll
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_out_scroll = obj;
                                            lv_obj_set_pos(obj, 797, 72);
                                            lv_obj_set_size(obj, 3, 22);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x5a5a5a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Advanced");
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // routing_fpga_target
                                            lv_obj_t *obj = lv_roller_create(parent_obj);
                                            objects.routing_fpga_target = obj;
                                            lv_obj_set_pos(obj, 0, 15);
                                            lv_obj_set_size(obj, 92, 303);
                                            lv_roller_set_options(obj, "Channel In\nAUX Out\nXLR Out\nAES50A Out\nAES50B Out\nCARD Out\nUltranet Out", LV_ROLLER_MODE_NORMAL);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x2f3237), LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // routing_matrix
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.routing_matrix = obj;
                                            lv_obj_set_pos(obj, 94, 15);
                                            lv_obj_set_size(obj, 579, 303);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj58 = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1378);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj59 = obj;
                                                    lv_obj_set_pos(obj, 72, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1382);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj60 = obj;
                                                    lv_obj_set_pos(obj, 144, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1386);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj61 = obj;
                                                    lv_obj_set_pos(obj, 216, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1390);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj62 = obj;
                                                    lv_obj_set_pos(obj, 288, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1394);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj63 = obj;
                                                    lv_obj_set_pos(obj, 360, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1398);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj64 = obj;
                                                    lv_obj_set_pos(obj, 432, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1402);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj65 = obj;
                                                    lv_obj_set_pos(obj, 504, 0);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1406);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj66 = obj;
                                                    lv_obj_set_pos(obj, 0, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1410);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj67 = obj;
                                                    lv_obj_set_pos(obj, 72, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1414);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj68 = obj;
                                                    lv_obj_set_pos(obj, 144, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1418);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj69 = obj;
                                                    lv_obj_set_pos(obj, 216, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1422);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj70 = obj;
                                                    lv_obj_set_pos(obj, 288, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1426);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj71 = obj;
                                                    lv_obj_set_pos(obj, 360, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1430);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj72 = obj;
                                                    lv_obj_set_pos(obj, 432, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1434);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj73 = obj;
                                                    lv_obj_set_pos(obj, 504, 50);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1438);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj74 = obj;
                                                    lv_obj_set_pos(obj, 0, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1442);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj75 = obj;
                                                    lv_obj_set_pos(obj, 72, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1446);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj76 = obj;
                                                    lv_obj_set_pos(obj, 144, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1450);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj77 = obj;
                                                    lv_obj_set_pos(obj, 216, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1454);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj78 = obj;
                                                    lv_obj_set_pos(obj, 288, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1458);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj79 = obj;
                                                    lv_obj_set_pos(obj, 360, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1462);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj80 = obj;
                                                    lv_obj_set_pos(obj, 432, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1466);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj81 = obj;
                                                    lv_obj_set_pos(obj, 504, 100);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1470);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj82 = obj;
                                                    lv_obj_set_pos(obj, 0, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1474);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj83 = obj;
                                                    lv_obj_set_pos(obj, 72, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1478);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj84 = obj;
                                                    lv_obj_set_pos(obj, 144, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1482);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj85 = obj;
                                                    lv_obj_set_pos(obj, 216, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1486);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj86 = obj;
                                                    lv_obj_set_pos(obj, 288, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1490);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj87 = obj;
                                                    lv_obj_set_pos(obj, 360, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1494);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj88 = obj;
                                                    lv_obj_set_pos(obj, 432, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1498);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj89 = obj;
                                                    lv_obj_set_pos(obj, 504, 150);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1502);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj90 = obj;
                                                    lv_obj_set_pos(obj, 0, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1506);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj91 = obj;
                                                    lv_obj_set_pos(obj, 72, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1510);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj92 = obj;
                                                    lv_obj_set_pos(obj, 144, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1514);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj93 = obj;
                                                    lv_obj_set_pos(obj, 216, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1518);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj94 = obj;
                                                    lv_obj_set_pos(obj, 288, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1522);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj95 = obj;
                                                    lv_obj_set_pos(obj, 360, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1526);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj96 = obj;
                                                    lv_obj_set_pos(obj, 432, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1530);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj97 = obj;
                                                    lv_obj_set_pos(obj, 504, 200);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1534);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj98 = obj;
                                                    lv_obj_set_pos(obj, 0, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1538);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj99 = obj;
                                                    lv_obj_set_pos(obj, 72, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1542);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj100 = obj;
                                                    lv_obj_set_pos(obj, 144, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1546);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj101 = obj;
                                                    lv_obj_set_pos(obj, 216, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1550);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj102 = obj;
                                                    lv_obj_set_pos(obj, 288, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1554);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj103 = obj;
                                                    lv_obj_set_pos(obj, 360, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1558);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj104 = obj;
                                                    lv_obj_set_pos(obj, 432, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1562);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj105 = obj;
                                                    lv_obj_set_pos(obj, 504, 250);
                                                    lv_obj_set_size(obj, 71, 49);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    create_user_widget_routing_element(obj, 1566);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // routing_fpga_source
                                            lv_obj_t *obj = lv_roller_create(parent_obj);
                                            objects.routing_fpga_source = obj;
                                            lv_obj_set_pos(obj, 675, 15);
                                            lv_obj_set_size(obj, 125, 303);
                                            lv_roller_set_options(obj, "XLR In\nCARD In\nAUX In\nChannel Out\nAES50A In\nAES50B In", LV_ROLLER_MODE_NORMAL);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Setup");
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // setuptab
                            lv_obj_t *obj = lv_tabview_create(parent_obj);
                            objects.setuptab = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
                            lv_tabview_set_tab_bar_size(obj, 20);
                            add_style_tabview_nopad(obj);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Setup");
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // current_channel_source_4
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.current_channel_source_4 = obj;
                                            lv_obj_set_pos(obj, 32, 9);
                                            lv_obj_set_size(obj, 218, LV_SIZE_CONTENT);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x264056), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "BOARD VERSION INFO");
                                        }
                                        {
                                            // current_channel_source_5
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.current_channel_source_5 = obj;
                                            lv_obj_set_pos(obj, 277, 9);
                                            lv_obj_set_size(obj, 218, LV_SIZE_CONTENT);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x264056), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "IP ADDRESS");
                                        }
                                        {
                                            // setup_ipaddresstext
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.setup_ipaddresstext = obj;
                                            lv_obj_set_pos(obj, 277, 48);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "...");
                                        }
                                        {
                                            // setup_versioninfotext
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.setup_versioninfotext = obj;
                                            lv_obj_set_pos(obj, 32, 47);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "...");
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Card");
                                    add_style_tab_nopad(obj);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            lv_obj_set_pos(obj, 448, 265);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_play);
                                        }
                                        {
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            lv_obj_set_pos(obj, 312, 265);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_stop);
                                        }
                                        {
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            lv_obj_set_pos(obj, 579, 264);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_record);
                                        }
                                        {
                                            // setup_card_sdcard
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            objects.setup_card_sdcard = obj;
                                            lv_obj_set_pos(obj, 167, 254);
                                            lv_obj_set_size(obj, 64, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_sdcard);
                                            lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                        }
                                        {
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            lv_obj_set_pos(obj, 705, 261);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_select);
                                        }
                                        {
                                            // setup_card_sdusb
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            objects.setup_card_sdusb = obj;
                                            lv_obj_set_pos(obj, 40, 256);
                                            lv_obj_set_size(obj, 50, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_sdusb);
                                            lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_LEFT);
                                        }
                                        {
                                            // setup_card_toc_header
                                            lv_obj_t *obj = lv_table_create(parent_obj);
                                            objects.setup_card_toc_header = obj;
                                            lv_obj_set_pos(obj, 354, 8);
                                            lv_obj_set_size(obj, 400, 34);
                                            lv_obj_set_style_pad_top(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_column(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_row(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // setup_card_totaltime
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.setup_card_totaltime = obj;
                                            lv_obj_set_pos(obj, 568, 230);
                                            lv_obj_set_size(obj, 201, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "...");
                                        }
                                        {
                                            // setup_card_currentposition
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.setup_card_currentposition = obj;
                                            lv_obj_set_pos(obj, 31, 231);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "...");
                                        }
                                        {
                                            // setup_card_toc
                                            lv_obj_t *obj = lv_table_create(parent_obj);
                                            objects.setup_card_toc = obj;
                                            lv_obj_set_pos(obj, 354, 44);
                                            lv_obj_set_size(obj, 400, 147);
                                            lv_obj_set_style_pad_top(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_row(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_column(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // setup_card_progress
                                            lv_obj_t *obj = lv_bar_create(parent_obj);
                                            objects.setup_card_progress = obj;
                                            lv_obj_set_pos(obj, 31, 203);
                                            lv_obj_set_size(obj, 738, 22);
                                            lv_bar_set_value(obj, 50, LV_ANIM_OFF);
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 32, 73);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "X-Card-Source:");
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // setup_card_sourcemode
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.setup_card_sourcemode = obj;
                                                    lv_obj_set_pos(obj, 126, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                            }
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 32, 98);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "X-Card-String:");
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // setup_card_detected
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.setup_card_detected = obj;
                                                    lv_obj_set_pos(obj, 126, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "-");
                                                }
                                            }
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 32, 47);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "Channelmode:");
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // setup_card_channelmode
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.setup_card_channelmode = obj;
                                                    lv_obj_set_pos(obj, 126, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                            }
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 32, 154);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "SD-Card 1:");
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // setup_card_sd1info
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.setup_card_sd1info = obj;
                                                    lv_obj_set_pos(obj, 128, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                            }
                                        }
                                        {
                                            // current_channel_source_2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.current_channel_source_2 = obj;
                                            lv_obj_set_pos(obj, 31, 122);
                                            lv_obj_set_size(obj, 312, LV_SIZE_CONTENT);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x264056), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "SD CARD INFORMATION (X-LIVE)");
                                        }
                                        {
                                            // current_channel_source_3
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.current_channel_source_3 = obj;
                                            lv_obj_set_pos(obj, 32, 9);
                                            lv_obj_set_size(obj, 312, LV_SIZE_CONTENT);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x264056), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "EXPANSION CARD INFORMATION");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 31, 174);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "SD-Card 2:");
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // setup_card_sd2info
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.setup_card_sd2info = obj;
                                                    lv_obj_set_pos(obj, 129, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Text");
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Surface");
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            lv_obj_set_pos(obj, -20, -20);
                                            lv_obj_set_size(obj, 800, 323);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 43, 17);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Bank");
                                                }
                                                {
                                                    // setup_surface_roller_bank
                                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                                    objects.setup_surface_roller_bank = obj;
                                                    lv_obj_set_pos(obj, 0, 42);
                                                    lv_obj_set_size(obj, 130, 276);
                                                    lv_roller_set_options(obj, "Assign A\nAssign B\nAssign C", LV_ROLLER_MODE_NORMAL);
                                                    add_style_roller_standard(obj);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 172, 17);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Control");
                                                }
                                                {
                                                    // setup_surface_roller_control
                                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                                    objects.setup_surface_roller_control = obj;
                                                    lv_obj_set_pos(obj, 134, 42);
                                                    lv_obj_set_size(obj, 130, 276);
                                                    lv_roller_set_options(obj, "ASSIGN Encoder 1\nASSIGN Encoder 2\nASSIGN Encoder 3\nASSIGN Encoder 4\nASSIGN Button 1\nASSIGN Button 2\nASSIGN Button 3\nASSIGN Button 4\nASSIGN Button 5\nASSIGN Button 6\nASSIGN Button 7\nASSIGN Button 8", LV_ROLLER_MODE_NORMAL);
                                                    lv_roller_set_selected(obj, 3, LV_ANIM_OFF);
                                                    add_style_roller_standard(obj);
                                                }
                                                {
                                                    // setup_surface_roller_action
                                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                                    objects.setup_surface_roller_action = obj;
                                                    lv_obj_set_pos(obj, 536, 42);
                                                    lv_obj_set_size(obj, 130, 276);
                                                    lv_roller_set_options(obj, "Set\nSet to Index\nChange\nToggle\nReset\nRefresh\nClear Solo", LV_ROLLER_MODE_NORMAL);
                                                    lv_roller_set_selected(obj, 2, LV_ANIM_OFF);
                                                    add_style_roller_standard(obj);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 409, 17);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Mixerparameter");
                                                }
                                                {
                                                    // setup_surface_roller_mixerparmeter
                                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                                    objects.setup_surface_roller_mixerparmeter = obj;
                                                    lv_obj_set_pos(obj, 402, 42);
                                                    lv_obj_set_size(obj, 130, 276);
                                                    lv_roller_set_options(obj, "Channel\nDMX", LV_ROLLER_MODE_NORMAL);
                                                    add_style_roller_standard(obj);
                                                }
                                                {
                                                    // setup_surface_roller_index
                                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                                    objects.setup_surface_roller_index = obj;
                                                    lv_obj_set_pos(obj, 670, 42);
                                                    lv_obj_set_size(obj, 130, 276);
                                                    lv_roller_set_options(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n", LV_ROLLER_MODE_NORMAL);
                                                    lv_roller_set_selected(obj, 2, LV_ANIM_OFF);
                                                    add_style_roller_standard(obj);
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 576, 17);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Action");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 310, 17);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Group");
                                                }
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 682, 17);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text_static(obj, "Channel/Index");
                                                }
                                                {
                                                    // setup_surface_roller_group
                                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                                    objects.setup_surface_roller_group = obj;
                                                    lv_obj_set_pos(obj, 268, 42);
                                                    lv_obj_set_size(obj, 130, 276);
                                                    lv_roller_set_options(obj, "Settings\nRouting\nState\nGlobal\nChannel\nChannel Gate\nChannel EQ\nChannel Dyn\nChannel Sends\nFX\nDMX", LV_ROLLER_MODE_NORMAL);
                                                    lv_roller_set_selected(obj, 3, LV_ANIM_OFF);
                                                    add_style_roller_standard(obj);
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "About");
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            lv_obj_set_pos(obj, -4, 194);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_image_set_src(obj, &img_tux);
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 83, 159);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "AES50-IP-Core:\nMarkus Noll (YetAnotherElectronicsChannel)");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 433, 109);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "FPGA, DSPs and Effects:\nChris Noeding (xn--nding-jua)");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 83, 109);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Main-Control, UI and Backend:\nAlexander Schulz (schulz-alexander)");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 188, 53);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "https://www.openx32.com\nhttps://www.github.com/OpenMixerProject/OpenX32");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 433, 159);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text_static(obj, "Optimizations for Linux:\nSebastian Gottschall (BrainSlayer)");
                                        }
                                        {
                                            // openx32logo_2
                                            lv_obj_t *obj = lv_image_create(parent_obj);
                                            objects.openx32logo_2 = obj;
                                            lv_obj_set_pos(obj, 177, -4);
                                            lv_obj_set_size(obj, 402, 59);
                                            lv_image_set_src(obj, &img_openx32logo);
                                            lv_image_set_scale(obj, 100);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "DEBUG");
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // setup_debug_label
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.setup_debug_label = obj;
                                            lv_obj_set_pos(obj, 1, -2);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text_static(obj, "Text");
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Prototype GUI");
                                }
                            }
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Library");
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 15, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text_static(obj, "Library");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Effects");
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // fxtable
                            lv_obj_t *obj = lv_table_create(parent_obj);
                            objects.fxtable = obj;
                            lv_obj_set_pos(obj, 0, 1);
                            lv_obj_set_size(obj, 800, 337);
                            lv_obj_set_style_pad_top(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_left(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_row(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                        }
                        {
                            // pultec_face
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.pultec_face = obj;
                            lv_obj_set_pos(obj, 0, 1);
                            lv_obj_set_size(obj, 800, 337);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7d8d9a), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x4c5761), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj106 = obj;
                                    lv_obj_set_pos(obj, 8, 8);
                                    lv_obj_set_size(obj, 12, 12);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa9afb5), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj107 = obj;
                                    lv_obj_set_pos(obj, 778, 8);
                                    lv_obj_set_size(obj, 12, 12);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa9afb5), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj108 = obj;
                                    lv_obj_set_pos(obj, 8, 315);
                                    lv_obj_set_size(obj, 12, 12);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa9afb5), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj109 = obj;
                                    lv_obj_set_pos(obj, 778, 315);
                                    lv_obj_set_size(obj, 12, 12);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa9afb5), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // pultec_slot
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_slot = obj;
                                    lv_obj_set_pos(obj, 28, 8);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "FX1");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.obj110 = obj;
                                    lv_obj_set_pos(obj, 0, 6);
                                    lv_obj_set_size(obj, 796, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "PROGRAM EQUALIZER");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.obj111 = obj;
                                    lv_obj_set_pos(obj, 20, 36);
                                    lv_obj_set_size(obj, 300, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "LOW FREQUENCY");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.obj112 = obj;
                                    lv_obj_set_pos(obj, 320, 36);
                                    lv_obj_set_size(obj, 300, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "HIGH FREQUENCY");
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj113 = obj;
                                    lv_obj_set_pos(obj, 30, 43);
                                    lv_obj_set_size(obj, 70, 1);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj114 = obj;
                                    lv_obj_set_pos(obj, 242, 43);
                                    lv_obj_set_size(obj, 78, 1);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj115 = obj;
                                    lv_obj_set_pos(obj, 330, 43);
                                    lv_obj_set_size(obj, 68, 1);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj116 = obj;
                                    lv_obj_set_pos(obj, 544, 43);
                                    lv_obj_set_size(obj, 70, 1);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // pultec_lo_boost_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_lo_boost_title = obj;
                                    lv_obj_set_pos(obj, 25, 58);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "LO BOOST");
                                }
                                {
                                    // pultec_lo_boost_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_lo_boost_scale = obj;
                                    lv_obj_set_pos(obj, 43, 76);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 10);
                                    lv_scale_set_angle_range(obj, 270);
                                    lv_scale_set_rotation(obj, 135);
                                    lv_scale_set_total_tick_count(obj, 11);
                                    lv_scale_set_major_tick_every(obj, 2);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[7] = {
                                        "0",
                                        "2",
                                        "4",
                                        "6",
                                        "8",
                                        "10",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj117 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj118 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_lo_boost_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_lo_boost_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj119 = obj;
                                    lv_obj_set_pos(obj, 55, 184);
                                    lv_obj_set_size(obj, 80, 18);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x27313a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 204, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // pultec_lo_boost_value
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pultec_lo_boost_value = obj;
                                            lv_obj_set_pos(obj, 0, 1);
                                            lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "");
                                        }
                                    }
                                }
                                {
                                    // pultec_lo_att_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_lo_att_title = obj;
                                    lv_obj_set_pos(obj, 175, 58);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "LO ATT");
                                }
                                {
                                    // pultec_lo_att_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_lo_att_scale = obj;
                                    lv_obj_set_pos(obj, 193, 76);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 10);
                                    lv_scale_set_angle_range(obj, 270);
                                    lv_scale_set_rotation(obj, 135);
                                    lv_scale_set_total_tick_count(obj, 11);
                                    lv_scale_set_major_tick_every(obj, 2);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[7] = {
                                        "0",
                                        "2",
                                        "4",
                                        "6",
                                        "8",
                                        "10",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj120 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj121 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_lo_att_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_lo_att_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj122 = obj;
                                    lv_obj_set_pos(obj, 205, 184);
                                    lv_obj_set_size(obj, 80, 18);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x27313a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 204, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // pultec_lo_att_value
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pultec_lo_att_value = obj;
                                            lv_obj_set_pos(obj, 0, 1);
                                            lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "");
                                        }
                                    }
                                }
                                {
                                    // pultec_hi_boost_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_hi_boost_title = obj;
                                    lv_obj_set_pos(obj, 325, 58);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "HI BOOST");
                                }
                                {
                                    // pultec_hi_boost_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_hi_boost_scale = obj;
                                    lv_obj_set_pos(obj, 343, 76);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 10);
                                    lv_scale_set_angle_range(obj, 270);
                                    lv_scale_set_rotation(obj, 135);
                                    lv_scale_set_total_tick_count(obj, 11);
                                    lv_scale_set_major_tick_every(obj, 2);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[7] = {
                                        "0",
                                        "2",
                                        "4",
                                        "6",
                                        "8",
                                        "10",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj123 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj124 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_hi_boost_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_hi_boost_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj125 = obj;
                                    lv_obj_set_pos(obj, 355, 184);
                                    lv_obj_set_size(obj, 80, 18);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x27313a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 204, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // pultec_hi_boost_value
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pultec_hi_boost_value = obj;
                                            lv_obj_set_pos(obj, 0, 1);
                                            lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "");
                                        }
                                    }
                                }
                                {
                                    // pultec_hi_att_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_hi_att_title = obj;
                                    lv_obj_set_pos(obj, 475, 58);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "HI ATT");
                                }
                                {
                                    // pultec_hi_att_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_hi_att_scale = obj;
                                    lv_obj_set_pos(obj, 493, 76);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 10);
                                    lv_scale_set_angle_range(obj, 270);
                                    lv_scale_set_rotation(obj, 135);
                                    lv_scale_set_total_tick_count(obj, 11);
                                    lv_scale_set_major_tick_every(obj, 2);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[7] = {
                                        "0",
                                        "2",
                                        "4",
                                        "6",
                                        "8",
                                        "10",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj126 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj127 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_hi_att_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_hi_att_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj128 = obj;
                                    lv_obj_set_pos(obj, 505, 184);
                                    lv_obj_set_size(obj, 80, 18);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x27313a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 204, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // pultec_hi_att_value
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pultec_hi_att_value = obj;
                                            lv_obj_set_pos(obj, 0, 1);
                                            lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "");
                                        }
                                    }
                                }
                                {
                                    // pultec_lo_freq_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_lo_freq_title = obj;
                                    lv_obj_set_pos(obj, 25, 206);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "LO FREQ");
                                }
                                {
                                    // pultec_lo_freq_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_lo_freq_scale = obj;
                                    lv_obj_set_pos(obj, 43, 224);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 3);
                                    lv_scale_set_angle_range(obj, 180);
                                    lv_scale_set_rotation(obj, 180);
                                    lv_scale_set_total_tick_count(obj, 4);
                                    lv_scale_set_major_tick_every(obj, 1);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[5] = {
                                        "20",
                                        "30",
                                        "60",
                                        "100",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj129 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj130 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_lo_freq_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_lo_freq_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    // pultec_hi_freq_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_hi_freq_title = obj;
                                    lv_obj_set_pos(obj, 175, 206);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "HI FREQ");
                                }
                                {
                                    // pultec_hi_freq_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_hi_freq_scale = obj;
                                    lv_obj_set_pos(obj, 193, 224);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 6);
                                    lv_scale_set_angle_range(obj, 210);
                                    lv_scale_set_rotation(obj, 165);
                                    lv_scale_set_total_tick_count(obj, 7);
                                    lv_scale_set_major_tick_every(obj, 1);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[8] = {
                                        "3",
                                        "4",
                                        "5",
                                        "8",
                                        "10",
                                        "12",
                                        "16",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj131 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj132 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_hi_freq_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_hi_freq_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    // pultec_hi_width_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_hi_width_title = obj;
                                    lv_obj_set_pos(obj, 325, 206);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "HI WIDTH");
                                }
                                {
                                    // pultec_hi_width_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_hi_width_scale = obj;
                                    lv_obj_set_pos(obj, 343, 224);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 10);
                                    lv_scale_set_angle_range(obj, 270);
                                    lv_scale_set_rotation(obj, 135);
                                    lv_scale_set_total_tick_count(obj, 11);
                                    lv_scale_set_major_tick_every(obj, 10);
                                    lv_scale_set_label_show(obj, false);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj133 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj134 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_hi_width_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_hi_width_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.obj135 = obj;
                                    lv_obj_set_pos(obj, 321, 316);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "SHARP");
                                }
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.obj136 = obj;
                                    lv_obj_set_pos(obj, 433, 316);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "BROAD");
                                }
                                {
                                    // pultec_hi_att_freq_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_hi_att_freq_title = obj;
                                    lv_obj_set_pos(obj, 475, 206);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "HI-ATT FREQ");
                                }
                                {
                                    // pultec_hi_att_freq_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_hi_att_freq_scale = obj;
                                    lv_obj_set_pos(obj, 493, 224);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 2);
                                    lv_scale_set_angle_range(obj, 120);
                                    lv_scale_set_rotation(obj, 210);
                                    lv_scale_set_total_tick_count(obj, 3);
                                    lv_scale_set_major_tick_every(obj, 1);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[4] = {
                                        "5",
                                        "10",
                                        "20",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj137 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj138 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_hi_att_freq_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_hi_att_freq_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj139 = obj;
                                    lv_obj_set_pos(obj, 628, 40);
                                    lv_obj_set_size(obj, 2, 280);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x56636e), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 179, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj140 = obj;
                                    lv_obj_set_pos(obj, 646, 66);
                                    lv_obj_set_size(obj, 28, 28);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x9aa0a6), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // pultec_eq_in_lamp
                                    lv_obj_t *obj = lv_led_create(parent_obj);
                                    objects.pultec_eq_in_lamp = obj;
                                    lv_obj_set_pos(obj, 650, 70);
                                    lv_obj_set_size(obj, 20, 20);
                                    lv_led_set_color(obj, lv_color_hex(0xff3b2f));
                                    lv_led_set_brightness(obj, 40);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_spread(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // pultec_eq_in_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_eq_in_title = obj;
                                    lv_obj_set_pos(obj, 684, 73);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "EQ IN");
                                }
                                {
                                    // pultec_eq_in_value
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_eq_in_value = obj;
                                    lv_obj_set_pos(obj, 742, 73);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "");
                                }
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.obj141 = obj;
                                    lv_obj_set_pos(obj, 646, 106);
                                    lv_obj_set_size(obj, 28, 28);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x9aa0a6), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // pultec_tube_lamp
                                    lv_obj_t *obj = lv_led_create(parent_obj);
                                    objects.pultec_tube_lamp = obj;
                                    lv_obj_set_pos(obj, 650, 110);
                                    lv_obj_set_size(obj, 20, 20);
                                    lv_led_set_color(obj, lv_color_hex(0xffa11a));
                                    lv_led_set_brightness(obj, 40);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_spread(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // pultec_tube_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_tube_title = obj;
                                    lv_obj_set_pos(obj, 684, 113);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "TUBE");
                                }
                                {
                                    // pultec_tube_value
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_tube_value = obj;
                                    lv_obj_set_pos(obj, 742, 113);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "");
                                }
                                {
                                    // pultec_drive_title
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_drive_title = obj;
                                    lv_obj_set_pos(obj, 640, 206);
                                    lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "DRIVE");
                                }
                                {
                                    // pultec_drive_scale
                                    lv_obj_t *obj = lv_scale_create(parent_obj);
                                    objects.pultec_drive_scale = obj;
                                    lv_obj_set_pos(obj, 658, 224);
                                    lv_obj_set_size(obj, 104, 104);
                                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                                    lv_scale_set_range(obj, 0, 4);
                                    lv_scale_set_angle_range(obj, 270);
                                    lv_scale_set_rotation(obj, 135);
                                    lv_scale_set_total_tick_count(obj, 5);
                                    lv_scale_set_major_tick_every(obj, 2);
                                    lv_scale_set_label_show(obj, true);
                                    static const char *label_texts[4] = {
                                        "-12",
                                        "0",
                                        "+12",
                                        NULL
                                    };
                                    lv_scale_set_text_src(obj, label_texts);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_radial(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN);
                                    lv_obj_set_style_length(obj, 3, LV_PART_ITEMS);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_ITEMS);
                                    lv_obj_set_style_length(obj, 6, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_INDICATOR);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_INDICATOR);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_obj_create(parent_obj);
                                            objects.obj142 = obj;
                                            lv_obj_set_pos(obj, 30, 30);
                                            lv_obj_set_size(obj, 43, 43);
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_border_color(obj, lv_color_hex(0x3c434a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                                    objects.obj143 = obj;
                                                    lv_obj_set_pos(obj, 7, 7);
                                                    lv_obj_set_size(obj, 27, 27);
                                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_radius(obj, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // pultec_drive_needle
                                            lv_obj_t *obj = lv_line_create(parent_obj);
                                            objects.pultec_drive_needle = obj;
                                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf2efe4), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_scale_set_line_needle_value(parent_obj, obj, 19, 0);
                                        }
                                    }
                                }
                                {
                                    // pultec_bank
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.pultec_bank = obj;
                                    lv_obj_set_pos(obj, 650, 150);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf6f4ec), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "BANK 1/3");
                                }
                            }
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Mute Group");
                    add_style_tab_nopad(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 15, 20);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text_static(obj, "Mute Groups");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Scenes");
                }
            }
        }
        {
            // display_encoder_sliders
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.display_encoder_sliders = obj;
            lv_obj_set_pos(obj, 0, 420);
            lv_obj_set_size(obj, LV_PCT(100), 60);
            add_style_panel_flat_black(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // widget1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.widget1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 130, 60);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_config_slider_display_encoder(obj, 1678);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ONE|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    add_style_display_encoder_panels(obj);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // widget2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.widget2 = obj;
                    lv_obj_set_pos(obj, 134, 0);
                    lv_obj_set_size(obj, 130, 60);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_config_slider_display_encoder(obj, 1682);
                    add_style_display_encoder_panels(obj);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // widget3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.widget3 = obj;
                    lv_obj_set_pos(obj, 268, 0);
                    lv_obj_set_size(obj, 130, 60);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_config_slider_display_encoder(obj, 1686);
                    add_style_display_encoder_panels(obj);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // widget4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.widget4 = obj;
                    lv_obj_set_pos(obj, 402, 0);
                    lv_obj_set_size(obj, 130, 60);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_config_slider_display_encoder(obj, 1690);
                    add_style_display_encoder_panels(obj);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // widget5
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.widget5 = obj;
                    lv_obj_set_pos(obj, 536, 0);
                    lv_obj_set_size(obj, 130, 60);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_config_slider_display_encoder(obj, 1694);
                    add_style_display_encoder_panels(obj);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // widget6
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.widget6 = obj;
                    lv_obj_set_pos(obj, 670, 0);
                    lv_obj_set_size(obj, 130, 60);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_config_slider_display_encoder(obj, 1698);
                    add_style_display_encoder_panels(obj);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // header_omc_version
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.header_omc_version = obj;
            lv_obj_set_pos(obj, 237, 3);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x7d7d7d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "OMC vX.X.X build on 01.01.1999 ");
        }
        {
            // header_time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.header_time = obj;
            lv_obj_set_pos(obj, 701, 46);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "22:23:54");
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    screen_main_state_t *state = &screen_main_state;
    (void)state;
    tick_user_widget_dbx160_face(70, &state->dbx160);
    tick_user_widget_eq1084_face(103);
    tick_user_widget_ms_bar8(210);
    tick_user_widget_ms_bar8(283);
    tick_user_widget_ms_bar8(356);
    tick_user_widget_ms_bar8(429);
    tick_user_widget_ms_bar8(502);
    tick_user_widget_ms_bar8(575);
    tick_user_widget_ms_bar8(648);
    tick_user_widget_ms_bar8(721);
    tick_user_widget_ms_bar8(794);
    tick_user_widget_ms_bar(867);
    tick_user_widget_routing_xp_header(895);
    tick_user_widget_routing_xp_row(918);
    tick_user_widget_routing_xp_row(937);
    tick_user_widget_routing_xp_row(956);
    tick_user_widget_routing_xp_row(975);
    tick_user_widget_routing_xp_row(994);
    tick_user_widget_routing_xp_row(1013);
    tick_user_widget_routing_xp_row(1032);
    tick_user_widget_routing_xp_row(1051);
    tick_user_widget_routing_xp_row(1070);
    tick_user_widget_routing_xp_row(1089);
    tick_user_widget_routing_xp_row(1108);
    tick_user_widget_routing_xp_header(1143);
    tick_user_widget_routing_xp_row(1166);
    tick_user_widget_routing_xp_row(1185);
    tick_user_widget_routing_xp_row(1204);
    tick_user_widget_routing_xp_row(1223);
    tick_user_widget_routing_xp_row(1242);
    tick_user_widget_routing_xp_row(1261);
    tick_user_widget_routing_xp_row(1280);
    tick_user_widget_routing_xp_row(1299);
    tick_user_widget_routing_xp_row(1318);
    tick_user_widget_routing_xp_row(1337);
    tick_user_widget_routing_xp_row(1356);
    tick_user_widget_routing_element(1378);
    tick_user_widget_routing_element(1382);
    tick_user_widget_routing_element(1386);
    tick_user_widget_routing_element(1390);
    tick_user_widget_routing_element(1394);
    tick_user_widget_routing_element(1398);
    tick_user_widget_routing_element(1402);
    tick_user_widget_routing_element(1406);
    tick_user_widget_routing_element(1410);
    tick_user_widget_routing_element(1414);
    tick_user_widget_routing_element(1418);
    tick_user_widget_routing_element(1422);
    tick_user_widget_routing_element(1426);
    tick_user_widget_routing_element(1430);
    tick_user_widget_routing_element(1434);
    tick_user_widget_routing_element(1438);
    tick_user_widget_routing_element(1442);
    tick_user_widget_routing_element(1446);
    tick_user_widget_routing_element(1450);
    tick_user_widget_routing_element(1454);
    tick_user_widget_routing_element(1458);
    tick_user_widget_routing_element(1462);
    tick_user_widget_routing_element(1466);
    tick_user_widget_routing_element(1470);
    tick_user_widget_routing_element(1474);
    tick_user_widget_routing_element(1478);
    tick_user_widget_routing_element(1482);
    tick_user_widget_routing_element(1486);
    tick_user_widget_routing_element(1490);
    tick_user_widget_routing_element(1494);
    tick_user_widget_routing_element(1498);
    tick_user_widget_routing_element(1502);
    tick_user_widget_routing_element(1506);
    tick_user_widget_routing_element(1510);
    tick_user_widget_routing_element(1514);
    tick_user_widget_routing_element(1518);
    tick_user_widget_routing_element(1522);
    tick_user_widget_routing_element(1526);
    tick_user_widget_routing_element(1530);
    tick_user_widget_routing_element(1534);
    tick_user_widget_routing_element(1538);
    tick_user_widget_routing_element(1542);
    tick_user_widget_routing_element(1546);
    tick_user_widget_routing_element(1550);
    tick_user_widget_routing_element(1554);
    tick_user_widget_routing_element(1558);
    tick_user_widget_routing_element(1562);
    tick_user_widget_routing_element(1566);
    tick_user_widget_config_slider_display_encoder(1678);
    tick_user_widget_config_slider_display_encoder(1682);
    tick_user_widget_config_slider_display_encoder(1686);
    tick_user_widget_config_slider_display_encoder(1690);
    tick_user_widget_config_slider_display_encoder(1694);
    tick_user_widget_config_slider_display_encoder(1698);
}

void create_user_widget_ms_bar(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // panel_clip
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 3, 5);
            lv_obj_set_size(obj, 14, 6);
            add_style_panel_flat_dark_grey(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_CHECKED);
        }
        {
            // panel_4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 3, 12);
            lv_obj_set_size(obj, 14, 5);
            add_style_panel_flat_dark_grey(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffab00), LV_PART_MAIN | LV_STATE_CHECKED);
        }
        {
            // panel_3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 3, 18);
            lv_obj_set_size(obj, 14, 5);
            add_style_panel_flat_dark_grey(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffab00), LV_PART_MAIN | LV_STATE_CHECKED);
        }
        {
            // panel_2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 3, 24);
            lv_obj_set_size(obj, 14, 5);
            add_style_panel_flat_dark_grey(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffab00), LV_PART_MAIN | LV_STATE_CHECKED);
        }
        {
            // panel_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
            lv_obj_set_pos(obj, 3, 30);
            lv_obj_set_size(obj, 14, 5);
            add_style_panel_flat_dark_grey(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_CHECKED);
        }
        {
            // panel_0
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
            lv_obj_set_pos(obj, 3, 36);
            lv_obj_set_size(obj, 14, 5);
            add_style_panel_flat_dark_grey(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_CHECKED);
        }
        {
            // fader
            lv_obj_t *obj = lv_slider_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 6] = obj;
            lv_obj_set_pos(obj, 8, 54);
            lv_obj_set_size(obj, 4, 58);
            lv_slider_set_range(obj, -60, 10);
            lv_slider_set_value(obj, 10, LV_ANIM_OFF);
            add_style_slider_fader(obj);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 7] = obj;
            lv_obj_set_pos(obj, 2, 62);
            lv_obj_set_size(obj, 16, 2);
            add_style_panel_flat_black(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x565353), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_user_widget_ms_bar(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_ms_bar8(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 1);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 9] = obj;
            lv_obj_set_pos(obj, 20, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 10);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 18] = obj;
            lv_obj_set_pos(obj, 40, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 19);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 27] = obj;
            lv_obj_set_pos(obj, 60, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 28);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 36] = obj;
            lv_obj_set_pos(obj, 80, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 37);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 45] = obj;
            lv_obj_set_pos(obj, 100, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 46);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 54] = obj;
            lv_obj_set_pos(obj, 120, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 55);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 63] = obj;
            lv_obj_set_pos(obj, 140, 0);
            lv_obj_set_size(obj, 20, 120);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_ms_bar(obj, startWidgetIndex + 64);
        }
    }
}

void tick_user_widget_ms_bar8(int startWidgetIndex) {
    (void)startWidgetIndex;
    tick_user_widget_ms_bar(startWidgetIndex + 1);
    tick_user_widget_ms_bar(startWidgetIndex + 10);
    tick_user_widget_ms_bar(startWidgetIndex + 19);
    tick_user_widget_ms_bar(startWidgetIndex + 28);
    tick_user_widget_ms_bar(startWidgetIndex + 37);
    tick_user_widget_ms_bar(startWidgetIndex + 46);
    tick_user_widget_ms_bar(startWidgetIndex + 55);
    tick_user_widget_ms_bar(startWidgetIndex + 64);
}

void create_user_widget_config_slider_display_encoder(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // label
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 6);
            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Label");
        }
        {
            // slider
            lv_obj_t *obj = lv_slider_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 6, 28);
            lv_obj_set_size(obj, 119, 4);
            lv_slider_set_range(obj, -2, 101);
            lv_slider_set_value(obj, 100, LV_ANIM_OFF);
            add_style_slider_config(obj);
        }
        {
            // label_buttonpress
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 6, 39);
            lv_obj_set_size(obj, 119, LV_SIZE_CONTENT);
            lv_obj_set_style_pad_left(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Center");
        }
    }
}

void tick_user_widget_config_slider_display_encoder(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_widget_fx(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // label
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), 20);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x167ada), LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_label_set_text_static(obj, "Empty");
        }
        {
            // table
            lv_obj_t *obj = lv_table_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 0, 20);
            lv_obj_set_size(obj, 100, 320);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_row(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
        }
    }
}

void tick_user_widget_widget_fx(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_routing_element(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // lbl_header
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 2, 3);
            lv_obj_set_size(obj, 67, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xb7b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_label_set_text_static(obj, "CH 01");
        }
        {
            // lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 2, 28);
            lv_obj_set_size(obj, 67, LV_SIZE_CONTENT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "XLR 1");
        }
        {
            // lbl_footer
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 2, 14);
            lv_obj_set_size(obj, 67, LV_SIZE_CONTENT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xb7b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Kanal 1");
        }
    }
}

void tick_user_widget_routing_element(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_dbx160_face(lv_obj_t *parent_obj, int startWidgetIndex, user_widget_dbx160_face_state_t *state) {
    (void)startWidgetIndex;
    (void)state;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // face
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 780, 300);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x181818), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x8c8c8c), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // deco_1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
                    lv_obj_set_pos(obj, 10, 10);
                    lv_obj_set_size(obj, 14, 14);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x7a7a7a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
                    lv_obj_set_pos(obj, 756, 10);
                    lv_obj_set_size(obj, 14, 14);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x7a7a7a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
                    lv_obj_set_pos(obj, 10, 276);
                    lv_obj_set_size(obj, 14, 14);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x7a7a7a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, 756, 276);
                    lv_obj_set_size(obj, 14, 14);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x7a7a7a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
                    lv_obj_set_pos(obj, 34, 22);
                    lv_obj_set_size(obj, 120, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf2f2f2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "160");
                }
                {
                    // deco_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 6] = obj;
                    lv_obj_set_pos(obj, 36, 66);
                    lv_obj_set_size(obj, 170, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xa8a8a8), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "COMPRESSOR / LIMITER");
                }
                {
                    // deco_8
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 7] = obj;
                    lv_obj_set_pos(obj, 34, 92);
                    lv_obj_set_size(obj, 250, 150);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xdccda6), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x050505), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // meter
                            lv_obj_t *obj = lv_scale_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 8] = obj;
                            lv_obj_set_pos(obj, -29, 26);
                            lv_obj_set_size(obj, 300, 300);
                            lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                            lv_scale_set_range(obj, 0, 60);
                            lv_scale_set_angle_range(obj, 80);
                            lv_scale_set_rotation(obj, 230);
                            lv_scale_set_total_tick_count(obj, 31);
                            lv_scale_set_major_tick_every(obj, 5);
                            lv_scale_set_label_show(obj, true);
                            static const char *label_texts[8] = {
                                "0",
                                "5",
                                "10",
                                "15",
                                "20",
                                "25",
                                "30",
                                NULL
                            };
                            lv_scale_set_text_src(obj, label_texts);
                            {
                                state->scale_section = lv_scale_add_section(obj);
                                lv_scale_section_set_range(state->scale_section, 40, 60);
                                {
                                    if (!meter_section_main_style_initialized) {
                                        lv_style_init(&meter_section_main_style);
                                        meter_section_main_style_initialized = true;
                                        lv_style_set_arc_color(&meter_section_main_style, lv_color_hex(0xb3261e));
                                    }
                                    lv_scale_set_section_style_main(obj, state->scale_section, &meter_section_main_style);
                                }
                                {
                                    if (!meter_section_items_style_initialized) {
                                        lv_style_init(&meter_section_items_style);
                                        meter_section_items_style_initialized = true;
                                        lv_style_set_line_color(&meter_section_items_style, lv_color_hex(0xb3261e));
                                    }
                                    lv_scale_set_section_style_items(obj, state->scale_section, &meter_section_items_style);
                                }
                                {
                                    if (!meter_section_indicator_style_initialized) {
                                        lv_style_init(&meter_section_indicator_style);
                                        meter_section_indicator_style_initialized = true;
                                        lv_style_set_line_color(&meter_section_indicator_style, lv_color_hex(0xb3261e));
                                        lv_style_set_text_color(&meter_section_indicator_style, lv_color_hex(0xb3261e));
                                    }
                                    lv_scale_set_section_style_indicator(obj, state->scale_section, &meter_section_indicator_style);
                                }
                            }
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0x1a1a1a), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x1a1a1a), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_length(obj, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x1a1a1a), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0x1a1a1a), LV_PART_ITEMS | LV_STATE_DEFAULT);
                            lv_obj_set_style_length(obj, 6, LV_PART_ITEMS | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // needle
                                    lv_obj_t *obj = lv_line_create(parent_obj);
                                    ((lv_obj_t **)&objects)[startWidgetIndex + 9] = obj;
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_line_color(obj, lv_color_hex(0x8c1a12), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_scale_set_line_needle_value(parent_obj, obj, 132, 0);
                                }
                            }
                        }
                        {
                            // deco_7
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 10] = obj;
                            lv_obj_set_pos(obj, 0, 112);
                            lv_obj_set_size(obj, 242, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "GAIN REDUCTION  dB");
                        }
                    }
                }
                {
                    // led_below
                    lv_obj_t *obj = lv_led_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 11] = obj;
                    lv_obj_set_pos(obj, 310, 130);
                    lv_obj_set_size(obj, 16, 16);
                    lv_led_set_color(obj, lv_color_hex(0x2ee05a));
                    lv_led_set_brightness(obj, 80);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x5a5a5a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_9
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 12] = obj;
                    lv_obj_set_pos(obj, 338, 131);
                    lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "BELOW");
                }
                {
                    // led_above
                    lv_obj_t *obj = lv_led_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 13] = obj;
                    lv_obj_set_pos(obj, 310, 190);
                    lv_obj_set_size(obj, 16, 16);
                    lv_led_set_color(obj, lv_color_hex(0xff3b2f));
                    lv_led_set_brightness(obj, 80);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x5a5a5a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 14] = obj;
                    lv_obj_set_pos(obj, 338, 191);
                    lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "ABOVE");
                }
                {
                    // deco_11
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 15] = obj;
                    lv_obj_set_pos(obj, 395, 72);
                    lv_obj_set_size(obj, 120, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf2f2f2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "THRESHOLD");
                }
                {
                    // threshold_arc
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 16] = obj;
                    lv_obj_set_pos(obj, 407, 104);
                    lv_obj_set_size(obj, 96, 96);
                    lv_arc_set_range(obj, 0, 1000);
                    lv_arc_set_value(obj, 0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_arc_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0x3c3c3c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 4, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xe6e6e6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                }
                {
                    // deco_12
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 17] = obj;
                    lv_obj_set_pos(obj, 425, 122);
                    lv_obj_set_size(obj, 60, 60);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4a4a4a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x9a9a9a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_13
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 18] = obj;
                    lv_obj_set_pos(obj, 403, 214);
                    lv_obj_set_size(obj, 104, 26);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x050505), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // threshold_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 19] = obj;
                            lv_obj_set_pos(obj, 0, 3);
                            lv_obj_set_size(obj, 102, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffb000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0.0 dB");
                        }
                    }
                }
                {
                    // deco_14
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 20] = obj;
                    lv_obj_set_pos(obj, 510, 72);
                    lv_obj_set_size(obj, 120, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf2f2f2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "COMPRESSION");
                }
                {
                    // ratio_arc
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 21] = obj;
                    lv_obj_set_pos(obj, 522, 104);
                    lv_obj_set_size(obj, 96, 96);
                    lv_arc_set_range(obj, 0, 1000);
                    lv_arc_set_value(obj, 0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_arc_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0x3c3c3c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 4, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xe6e6e6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                }
                {
                    // deco_15
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 22] = obj;
                    lv_obj_set_pos(obj, 540, 122);
                    lv_obj_set_size(obj, 60, 60);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4a4a4a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x9a9a9a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_16
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 23] = obj;
                    lv_obj_set_pos(obj, 518, 214);
                    lv_obj_set_size(obj, 104, 26);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x050505), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ratio_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 24] = obj;
                            lv_obj_set_pos(obj, 0, 3);
                            lv_obj_set_size(obj, 102, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffb000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0.0 dB");
                        }
                    }
                }
                {
                    // deco_17
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 25] = obj;
                    lv_obj_set_pos(obj, 625, 72);
                    lv_obj_set_size(obj, 120, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf2f2f2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "OUTPUT GAIN");
                }
                {
                    // output_arc
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 26] = obj;
                    lv_obj_set_pos(obj, 637, 104);
                    lv_obj_set_size(obj, 96, 96);
                    lv_arc_set_range(obj, 0, 1000);
                    lv_arc_set_value(obj, 0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_arc_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0x3c3c3c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 4, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xe6e6e6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
                }
                {
                    // deco_18
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 27] = obj;
                    lv_obj_set_pos(obj, 655, 122);
                    lv_obj_set_size(obj, 60, 60);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4a4a4a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x9a9a9a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_19
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 28] = obj;
                    lv_obj_set_pos(obj, 633, 214);
                    lv_obj_set_size(obj, 104, 26);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x050505), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // output_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 29] = obj;
                            lv_obj_set_pos(obj, 0, 3);
                            lv_obj_set_size(obj, 102, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffb000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0.0 dB");
                        }
                    }
                }
            }
        }
    }
}

void tick_user_widget_dbx160_face(int startWidgetIndex, user_widget_dbx160_face_state_t *state) {
    (void)startWidgetIndex;
    (void)state;
}

void create_user_widget_eq1084_face(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // face
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 780, 300);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2b424d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x7a939e), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // deco_1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
                    lv_obj_set_pos(obj, 9, 9);
                    lv_obj_set_size(obj, 9, 9);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6d8790), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x111d23), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
                    lv_obj_set_pos(obj, 762, 9);
                    lv_obj_set_size(obj, 9, 9);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6d8790), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x111d23), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
                    lv_obj_set_pos(obj, 9, 282);
                    lv_obj_set_size(obj, 9, 9);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6d8790), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x111d23), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, 762, 282);
                    lv_obj_set_size(obj, 9, 9);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6d8790), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x111d23), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
                    lv_obj_set_pos(obj, 24, 12);
                    lv_obj_set_size(obj, 94, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "1084");
                }
                {
                    // deco_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 6] = obj;
                    lv_obj_set_pos(obj, 118, 24);
                    lv_obj_set_size(obj, 320, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "CLASSIC  /  CHANNEL EQUALIZER");
                }
                {
                    // deco_7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 7] = obj;
                    lv_obj_set_pos(obj, 514, 26);
                    lv_obj_set_size(obj, 240, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "FREQUENCY  /  GAIN");
                }
                {
                    // deco_8
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 8] = obj;
                    lv_obj_set_pos(obj, 24, 56);
                    lv_obj_set_size(obj, 732, 1);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6e8791), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_9
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 9] = obj;
                    lv_obj_set_pos(obj, 393, 70);
                    lv_obj_set_size(obj, 1, 198);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x5b7580), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_10
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 10] = obj;
                    lv_obj_set_pos(obj, 637, 70);
                    lv_obj_set_size(obj, 1, 198);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x5b7580), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_11
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 11] = obj;
                    lv_obj_set_pos(obj, 21, 71);
                    lv_obj_set_size(obj, 116, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "HF SHELF");
                }
                {
                    // deco_12
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 12] = obj;
                    lv_obj_set_pos(obj, 36, 109);
                    lv_obj_set_size(obj, 86, 86);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x101c24), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 43, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_13
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 13] = obj;
                    lv_obj_set_pos(obj, 41, 114);
                    lv_obj_set_size(obj, 76, 76);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe3ded0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 38, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x141f27), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xa49e90), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // hf_freq_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 14] = obj;
                    lv_obj_set_pos(obj, 28, 101);
                    lv_obj_set_size(obj, 102, 102);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 3);
                    lv_scale_set_angle_range(obj, 270);
                    lv_scale_set_rotation(obj, 135);
                    lv_scale_set_total_tick_count(obj, 4);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hf_freq_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 15] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xe39b51), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 35, 0);
                        }
                    }
                }
                {
                    // deco_14
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 16] = obj;
                    lv_obj_set_pos(obj, 52, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x667684), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 27, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x253743), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x33444f), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_15
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 17] = obj;
                    lv_obj_set_pos(obj, 24, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "OFF");
                }
                {
                    // deco_16
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 18] = obj;
                    lv_obj_set_pos(obj, 24, 106);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10k");
                }
                {
                    // deco_17
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 19] = obj;
                    lv_obj_set_pos(obj, 100, 106);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "12k");
                }
                {
                    // deco_18
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 20] = obj;
                    lv_obj_set_pos(obj, 100, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "16k");
                }
                {
                    // hf_gain_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 21] = obj;
                    lv_obj_set_pos(obj, 52, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 1000);
                    lv_scale_set_angle_range(obj, 250);
                    lv_scale_set_rotation(obj, 145);
                    lv_scale_set_total_tick_count(obj, 2);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hf_gain_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 22] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 21, 0);
                        }
                    }
                }
                {
                    // deco_19
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 23] = obj;
                    lv_obj_set_pos(obj, 55, 181);
                    lv_obj_set_size(obj, 10, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "-");
                }
                {
                    // deco_20
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 24] = obj;
                    lv_obj_set_pos(obj, 93, 181);
                    lv_obj_set_size(obj, 10, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "+");
                }
                {
                    // deco_21
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 25] = obj;
                    lv_obj_set_pos(obj, 29, 218);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x13252d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hf_freq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 26] = obj;
                            lv_obj_set_pos(obj, 0, 4);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                        {
                            // hf_gain_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 27] = obj;
                            lv_obj_set_pos(obj, 0, 25);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0.0 dB");
                        }
                    }
                }
                {
                    // deco_22
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 28] = obj;
                    lv_obj_set_pos(obj, 149, 71);
                    lv_obj_set_size(obj, 116, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "MID BAND");
                }
                {
                    // deco_23
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 29] = obj;
                    lv_obj_set_pos(obj, 164, 109);
                    lv_obj_set_size(obj, 86, 86);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x101c24), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 43, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_24
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 30] = obj;
                    lv_obj_set_pos(obj, 169, 114);
                    lv_obj_set_size(obj, 76, 76);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe3ded0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 38, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x141f27), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xa49e90), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // mf_freq_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 31] = obj;
                    lv_obj_set_pos(obj, 156, 101);
                    lv_obj_set_size(obj, 102, 102);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 6);
                    lv_scale_set_angle_range(obj, 270);
                    lv_scale_set_rotation(obj, 135);
                    lv_scale_set_total_tick_count(obj, 7);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // mf_freq_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 32] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xe39b51), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 35, 0);
                        }
                    }
                }
                {
                    // deco_25
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 33] = obj;
                    lv_obj_set_pos(obj, 180, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x667684), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 27, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x253743), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x33444f), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_26
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 34] = obj;
                    lv_obj_set_pos(obj, 152, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "OFF");
                }
                {
                    // deco_27
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 35] = obj;
                    lv_obj_set_pos(obj, 136, 145);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "360");
                }
                {
                    // deco_28
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 36] = obj;
                    lv_obj_set_pos(obj, 152, 106);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "700");
                }
                {
                    // deco_29
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 37] = obj;
                    lv_obj_set_pos(obj, 190, 90);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "1.6k");
                }
                {
                    // deco_30
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 38] = obj;
                    lv_obj_set_pos(obj, 228, 106);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "3.2k");
                }
                {
                    // deco_31
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 39] = obj;
                    lv_obj_set_pos(obj, 244, 145);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "4.8k");
                }
                {
                    // deco_32
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 40] = obj;
                    lv_obj_set_pos(obj, 228, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "7.2k");
                }
                {
                    // mf_gain_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 41] = obj;
                    lv_obj_set_pos(obj, 180, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 1000);
                    lv_scale_set_angle_range(obj, 250);
                    lv_scale_set_rotation(obj, 145);
                    lv_scale_set_total_tick_count(obj, 2);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // mf_gain_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 42] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 21, 0);
                        }
                    }
                }
                {
                    // deco_33
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 43] = obj;
                    lv_obj_set_pos(obj, 183, 181);
                    lv_obj_set_size(obj, 10, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "-");
                }
                {
                    // deco_34
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 44] = obj;
                    lv_obj_set_pos(obj, 221, 181);
                    lv_obj_set_size(obj, 10, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "+");
                }
                {
                    // deco_35
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 45] = obj;
                    lv_obj_set_pos(obj, 157, 218);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x13252d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // mf_freq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 46] = obj;
                            lv_obj_set_pos(obj, 0, 4);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                        {
                            // mf_gain_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 47] = obj;
                            lv_obj_set_pos(obj, 0, 25);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0.0 dB");
                        }
                    }
                }
                {
                    // deco_36
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 48] = obj;
                    lv_obj_set_pos(obj, 277, 71);
                    lv_obj_set_size(obj, 116, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "LF SHELF");
                }
                {
                    // deco_37
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 49] = obj;
                    lv_obj_set_pos(obj, 292, 109);
                    lv_obj_set_size(obj, 86, 86);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x101c24), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 43, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_38
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 50] = obj;
                    lv_obj_set_pos(obj, 297, 114);
                    lv_obj_set_size(obj, 76, 76);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe3ded0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 38, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x141f27), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xa49e90), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // lf_freq_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 51] = obj;
                    lv_obj_set_pos(obj, 284, 101);
                    lv_obj_set_size(obj, 102, 102);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 4);
                    lv_scale_set_angle_range(obj, 270);
                    lv_scale_set_rotation(obj, 135);
                    lv_scale_set_total_tick_count(obj, 5);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // lf_freq_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 52] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xe39b51), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 35, 0);
                        }
                    }
                }
                {
                    // deco_39
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 53] = obj;
                    lv_obj_set_pos(obj, 308, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x667684), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 27, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x253743), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x33444f), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_40
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 54] = obj;
                    lv_obj_set_pos(obj, 280, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "OFF");
                }
                {
                    // deco_41
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 55] = obj;
                    lv_obj_set_pos(obj, 268, 124);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "35");
                }
                {
                    // deco_42
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 56] = obj;
                    lv_obj_set_pos(obj, 318, 90);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "60");
                }
                {
                    // deco_43
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 57] = obj;
                    lv_obj_set_pos(obj, 368, 124);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "110");
                }
                {
                    // deco_44
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 58] = obj;
                    lv_obj_set_pos(obj, 356, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "220");
                }
                {
                    // lf_gain_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 59] = obj;
                    lv_obj_set_pos(obj, 308, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 1000);
                    lv_scale_set_angle_range(obj, 250);
                    lv_scale_set_rotation(obj, 145);
                    lv_scale_set_total_tick_count(obj, 2);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // lf_gain_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 60] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 21, 0);
                        }
                    }
                }
                {
                    // deco_45
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 61] = obj;
                    lv_obj_set_pos(obj, 311, 181);
                    lv_obj_set_size(obj, 10, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "-");
                }
                {
                    // deco_46
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 62] = obj;
                    lv_obj_set_pos(obj, 349, 181);
                    lv_obj_set_size(obj, 10, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "+");
                }
                {
                    // deco_47
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 63] = obj;
                    lv_obj_set_pos(obj, 285, 218);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x13252d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // lf_freq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 64] = obj;
                            lv_obj_set_pos(obj, 0, 4);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                        {
                            // lf_gain_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 65] = obj;
                            lv_obj_set_pos(obj, 0, 25);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0.0 dB");
                        }
                    }
                }
                {
                    // deco_48
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 66] = obj;
                    lv_obj_set_pos(obj, 397, 71);
                    lv_obj_set_size(obj, 116, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "HIGH PASS");
                }
                {
                    // deco_49
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 67] = obj;
                    lv_obj_set_pos(obj, 412, 109);
                    lv_obj_set_size(obj, 86, 86);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x101c24), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 43, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_50
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 68] = obj;
                    lv_obj_set_pos(obj, 417, 114);
                    lv_obj_set_size(obj, 76, 76);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x527989), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 38, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x141f27), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x2d5268), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // hp_freq_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 69] = obj;
                    lv_obj_set_pos(obj, 404, 101);
                    lv_obj_set_size(obj, 102, 102);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 4);
                    lv_scale_set_angle_range(obj, 270);
                    lv_scale_set_rotation(obj, 135);
                    lv_scale_set_total_tick_count(obj, 5);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hp_freq_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 70] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 35, 0);
                        }
                    }
                }
                {
                    // deco_51
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 71] = obj;
                    lv_obj_set_pos(obj, 428, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x215470), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 27, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x123849), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x123849), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_52
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 72] = obj;
                    lv_obj_set_pos(obj, 400, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "OFF");
                }
                {
                    // deco_53
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 73] = obj;
                    lv_obj_set_pos(obj, 388, 124);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "45");
                }
                {
                    // deco_54
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 74] = obj;
                    lv_obj_set_pos(obj, 438, 90);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "70");
                }
                {
                    // deco_55
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 75] = obj;
                    lv_obj_set_pos(obj, 488, 124);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "160");
                }
                {
                    // deco_56
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 76] = obj;
                    lv_obj_set_pos(obj, 476, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "360");
                }
                {
                    // deco_57
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 77] = obj;
                    lv_obj_set_pos(obj, 405, 218);
                    lv_obj_set_size(obj, 100, 29);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x13252d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hp_freq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 78] = obj;
                            lv_obj_set_pos(obj, 0, 4);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                    }
                }
                {
                    // deco_58
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 79] = obj;
                    lv_obj_set_pos(obj, 517, 71);
                    lv_obj_set_size(obj, 116, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "LOW PASS");
                }
                {
                    // deco_59
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 80] = obj;
                    lv_obj_set_pos(obj, 532, 109);
                    lv_obj_set_size(obj, 86, 86);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x101c24), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 43, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_60
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 81] = obj;
                    lv_obj_set_pos(obj, 537, 114);
                    lv_obj_set_size(obj, 76, 76);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x527989), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 38, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x141f27), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x2d5268), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // lp_freq_scale
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 82] = obj;
                    lv_obj_set_pos(obj, 524, 101);
                    lv_obj_set_size(obj, 102, 102);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 5);
                    lv_scale_set_angle_range(obj, 270);
                    lv_scale_set_rotation(obj, 135);
                    lv_scale_set_total_tick_count(obj, 6);
                    lv_scale_set_major_tick_every(obj, 1);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xbacbd0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_length(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // lp_freq_needle
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 83] = obj;
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_scale_set_line_needle_value(parent_obj, obj, 35, 0);
                        }
                    }
                }
                {
                    // deco_61
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 84] = obj;
                    lv_obj_set_pos(obj, 548, 125);
                    lv_obj_set_size(obj, 54, 54);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x215470), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 27, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x123849), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x123849), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // deco_62
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 85] = obj;
                    lv_obj_set_pos(obj, 520, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "OFF");
                }
                {
                    // deco_63
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 86] = obj;
                    lv_obj_set_pos(obj, 505, 136);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "6k");
                }
                {
                    // deco_64
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 87] = obj;
                    lv_obj_set_pos(obj, 533, 96);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "8k");
                }
                {
                    // deco_65
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 88] = obj;
                    lv_obj_set_pos(obj, 583, 96);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10k");
                }
                {
                    // deco_66
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 89] = obj;
                    lv_obj_set_pos(obj, 611, 136);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "14k");
                }
                {
                    // deco_67
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 90] = obj;
                    lv_obj_set_pos(obj, 596, 184);
                    lv_obj_set_size(obj, 34, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xbacbd0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "18k");
                }
                {
                    // deco_68
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 91] = obj;
                    lv_obj_set_pos(obj, 525, 218);
                    lv_obj_set_size(obj, 100, 29);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x13252d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x526b75), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // lp_freq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 92] = obj;
                            lv_obj_set_pos(obj, 0, 4);
                            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xf3eddd), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                    }
                }
                {
                    // deco_69
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 93] = obj;
                    lv_obj_set_pos(obj, 655, 76);
                    lv_obj_set_size(obj, 100, 48);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xc8c3b2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x152733), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // hiq_lamp
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 94] = obj;
                            lv_obj_set_pos(obj, 9, 19);
                            lv_obj_set_size(obj, 8, 8);
                            lv_led_set_color(obj, lv_color_hex(0xffb23d));
                            lv_led_set_brightness(obj, 80);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                        }
                        {
                            // hiq_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 95] = obj;
                            lv_obj_set_pos(obj, 22, 5);
                            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x162a34), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "HI-Q");
                        }
                        {
                            // hiq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 96] = obj;
                            lv_obj_set_pos(obj, 22, 22);
                            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x213644), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                    }
                }
                {
                    // deco_70
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 97] = obj;
                    lv_obj_set_pos(obj, 655, 138);
                    lv_obj_set_size(obj, 100, 48);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xc8c3b2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x152733), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // eq_lamp
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 98] = obj;
                            lv_obj_set_pos(obj, 9, 19);
                            lv_obj_set_size(obj, 8, 8);
                            lv_led_set_color(obj, lv_color_hex(0x6ee896));
                            lv_led_set_brightness(obj, 80);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                        }
                        {
                            // eq_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 99] = obj;
                            lv_obj_set_pos(obj, 22, 5);
                            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x162a34), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "EQ");
                        }
                        {
                            // eq_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 100] = obj;
                            lv_obj_set_pos(obj, 22, 22);
                            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x213644), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                    }
                }
                {
                    // deco_71
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 101] = obj;
                    lv_obj_set_pos(obj, 655, 200);
                    lv_obj_set_size(obj, 100, 48);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xc8c3b2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x152733), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // analog_lamp
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 102] = obj;
                            lv_obj_set_pos(obj, 9, 19);
                            lv_obj_set_size(obj, 8, 8);
                            lv_led_set_color(obj, lv_color_hex(0xffb23d));
                            lv_led_set_brightness(obj, 80);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                        }
                        {
                            // analog_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 103] = obj;
                            lv_obj_set_pos(obj, 22, 5);
                            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x162a34), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "ANALOG");
                        }
                        {
                            // analog_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            ((lv_obj_t **)&objects)[startWidgetIndex + 104] = obj;
                            lv_obj_set_pos(obj, 22, 22);
                            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x213644), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                    }
                }
            }
        }
    }
}

void tick_user_widget_eq1084_face(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_routing_xp_row(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // name
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 8, 3);
            lv_obj_set_size(obj, 64, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Ch 01");
        }
        {
            // sub
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 74, 4);
            lv_obj_set_size(obj, 52, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x8a8a8a), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Kanal 1");
        }
        {
            // x1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 133, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 175, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
            lv_obj_set_pos(obj, 217, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
            lv_obj_set_pos(obj, 259, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 6] = obj;
            lv_obj_set_pos(obj, 301, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 7] = obj;
            lv_obj_set_pos(obj, 343, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 8] = obj;
            lv_obj_set_pos(obj, 385, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 9] = obj;
            lv_obj_set_pos(obj, 427, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 10] = obj;
            lv_obj_set_pos(obj, 469, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x10
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 11] = obj;
            lv_obj_set_pos(obj, 511, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x11
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 12] = obj;
            lv_obj_set_pos(obj, 553, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x12
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 13] = obj;
            lv_obj_set_pos(obj, 595, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x13
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 14] = obj;
            lv_obj_set_pos(obj, 637, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x14
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 15] = obj;
            lv_obj_set_pos(obj, 679, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x15
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 16] = obj;
            lv_obj_set_pos(obj, 721, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // x16
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 17] = obj;
            lv_obj_set_pos(obj, 763, 3);
            lv_obj_set_size(obj, 32, 16);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x262626), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
    }
}

void tick_user_widget_routing_xp_row(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_routing_xp_header(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // info
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 22);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1b1b1b), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // info_left
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
                    lv_obj_set_pos(obj, 8, 3);
                    lv_obj_set_size(obj, 560, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Ch 01");
                }
                {
                    // info_right
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
                    lv_obj_set_pos(obj, 570, 4);
                    lv_obj_set_size(obj, 222, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x9a9a9a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // columns
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 0, 22);
            lv_obj_set_size(obj, 800, 22);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1d2a36), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // dest
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, 8, 4);
                    lv_obj_set_size(obj, 120, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Channel");
                }
                {
                    // h1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
                    lv_obj_set_pos(obj, 129, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "01");
                }
                {
                    // h2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 6] = obj;
                    lv_obj_set_pos(obj, 171, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "02");
                }
                {
                    // h3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 7] = obj;
                    lv_obj_set_pos(obj, 213, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "03");
                }
                {
                    // h4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 8] = obj;
                    lv_obj_set_pos(obj, 255, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "04");
                }
                {
                    // h5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 9] = obj;
                    lv_obj_set_pos(obj, 297, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "05");
                }
                {
                    // h6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 10] = obj;
                    lv_obj_set_pos(obj, 339, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "06");
                }
                {
                    // h7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 11] = obj;
                    lv_obj_set_pos(obj, 381, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "07");
                }
                {
                    // h8
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 12] = obj;
                    lv_obj_set_pos(obj, 423, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "08");
                }
                {
                    // h9
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 13] = obj;
                    lv_obj_set_pos(obj, 465, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "09");
                }
                {
                    // h10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 14] = obj;
                    lv_obj_set_pos(obj, 507, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "10");
                }
                {
                    // h11
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 15] = obj;
                    lv_obj_set_pos(obj, 549, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "11");
                }
                {
                    // h12
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 16] = obj;
                    lv_obj_set_pos(obj, 591, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "12");
                }
                {
                    // h13
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 17] = obj;
                    lv_obj_set_pos(obj, 633, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "13");
                }
                {
                    // h14
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 18] = obj;
                    lv_obj_set_pos(obj, 675, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "14");
                }
                {
                    // h15
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 19] = obj;
                    lv_obj_set_pos(obj, 717, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "15");
                }
                {
                    // h16
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 20] = obj;
                    lv_obj_set_pos(obj, 759, 5);
                    lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xcfe3f5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_label_set_text(obj, "16");
                }
            }
        }
    }
}

void tick_user_widget_routing_xp_header(int startWidgetIndex) {
    (void)startWidgetIndex;
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 1) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
// Groups
//

groups_t groups;
static bool groups_created = false;
void ui_create_groups() {
    if (!groups_created) {
        groups.grp_KEY = lv_group_create();
        groups_created = true;
    }
}

//
//
//

void create_screens() {
    
    // Initialize groups
    ui_create_groups();
    
    // Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
}