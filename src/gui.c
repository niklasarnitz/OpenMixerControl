#include "gui.h"

void timer100msCallback(int sig, siginfo_t *si, void *uc) {
  // TODO
}

/*
void guiInitMouse() {
  const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
  lv_display_t *disp = lv_linux_fbdev_create();

  lv_linux_fbdev_set_file(disp, device);

  char *mousePath = lv_libinput_find_dev(LV_LIBINPUT_CAPABILITY_POINTER, true);
  lv_indev_t *mouse = lv_libinput_create(LV_INDEV_TYPE_POINTER, mousePath);

  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon);
  lv_obj_t *cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active());
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);
  lv_indev_set_cursor(mouse, cursor_obj);
}
*/

void guiInit() {
  driver_backends_register();

  lv_init();

  driver_backends_init_backend("FBDEV");

  lv_timer_create(timer10msCallback, 10, NULL);
  lv_timer_create(timer100msCallback, 100, NULL);

  // initialize GUI created by EEZ
  ui_init();
/*
  // create simple Hello World-text
  lv_obj_t *label_1 = lv_label_create(lv_scr_act());
  lv_label_set_text(label_1, "Hello World!");
  lv_obj_set_style_text_font(label_1, &lv_font_montserrat_30, 0);
  lv_obj_align(label_1, LV_ALIGN_CENTER, 0, -20);
  lv_obj_t *label_2 = lv_label_create(lv_scr_act());
  lv_label_set_text_fmt(label_2, "From OpenX32 - The OpenSource Operating System for Behringers X32");
  lv_obj_set_style_text_font(label_2, &lv_font_montserrat_16, 0);
  lv_obj_align_to(label_2, label_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_obj_t *label_3 = lv_label_create(lv_scr_act());
  lv_label_set_text_fmt(label_3, "LVGL (%d.%d.%d)", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);
  lv_obj_set_style_text_font(label_3, &lv_font_montserrat_16, 0);
  lv_obj_align_to(label_3, label_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
*/

  // start endless loop
  driver_backends_run_loop();
}
