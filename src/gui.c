#include "gui.h"

lv_indev_t *keypad_indev; // will be updated by buttonCallback
lv_indev_t *mouse_indev; // will be polled by LVGL
/*
lv_indev_t *buttons_indev; // will be updated by buttonCallback
lv_indev_t *encoder_indev;
*/

uint16_t lastButton;
bool lastButtonPressed;

void timer100msCallback(int sig, siginfo_t *si, void *uc) {
  // TODO
}

void guiNewButtonPress(uint16_t button, bool pressed) {
  lastButton = button;
  lastButtonPressed = pressed;

  // inform LVGL, that we received a new button
  lv_indev_read(keypad_indev);
}

void guiKeypadInputCallback(lv_indev_t * indev, lv_indev_data_t * data){
  data->key = lastButton;            // Get the last pressed or released key

  if (lastButtonPressed) {
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void guiMouseInputCallback(lv_indev_t * indev, lv_indev_data_t * data) {
/*
    if(touchpad_pressed) {
        data->point.x = touchpad_x;
        data->point.y = touchpad_y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
*/
  data->state = LV_INDEV_STATE_RELEASED;
}

/*
void guiButtonsInputCallback(lv_indev_t * indev, lv_indev_data_t * data) {
  data->state = LV_INDEV_STATE_PRESSED;
  data->state = LV_INDEV_STATE_RELEASED;
  data->btn_id = last_btn;
}
*/
/*
void guiEncoderInputCallback(lv_indev_t * indev, lv_indev_data_t * data) {
  data->enc_diff = enc_get_new_moves();

  if(enc_pressed()) data->state = LV_INDEV_STATE_PRESSED;
  else data->state = LV_INDEV_STATE_RELEASED;
}
*/

void guiInitInput() {
  // initialize keypad
  keypad_indev = lv_indev_create();
  lv_indev_set_type(keypad_indev, LV_INDEV_TYPE_KEYPAD);
  lv_indev_set_read_cb(keypad_indev, guiKeypadInputCallback);
  lv_indev_set_mode(keypad_indev, LV_INDEV_MODE_EVENT); // set to event-driven mode (without would be polling-mode)

  // initialize mouse
  mouse_indev = lv_indev_create();
  lv_indev_set_type(mouse_indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(mouse_indev, guiMouseInputCallback);
//  lv_indev_set_driver(mouse_indev, &my_mouse_driver);
//  lv_indev_set_mode(mouse_indev, LV_INDEV_MODE_EVENT); // set to event-driven mode (without would be polling-mode)

/*
  // intialize buttons
  buttons_indev = lv_indev_create();
  lv_indev_set_type(buttons_indev, LV_INDEV_TYPE_BUTTON);
  lv_indev_set_read_cb(buttons_indev, guiButtonsInputCallback);
  lv_indev_set_mode(buttons_indev, LV_INDEV_MODE_EVENT); // set to event-driven mode (without would be polling-mode)

  // intialize encoder
  encoder_indev = lv_indev_create();
  lv_indev_set_type(encoder_indev, LV_INDEV_TYPE_ENCODER);
  lv_indev_set_read_cb(encoder_indev, guiEncoderInputCallback);
  lv_indev_set_mode(encoder_indev, LV_INDEV_MODE_EVENT); // set to event-driven mode (without would be polling-mode)
*/


/*
  // Create a cursor object (e.g., an image)
  lv_obj_t *cursor_obj = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(cursor_obj, &my_cursor_image);
  lv_indev_set_cursor(mouse_indev, cursor_obj);
*/

/*
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
*/
}

void guiInit() {
  driver_backends_register();

  lv_init();

  driver_backends_init_backend("FBDEV");

  lv_timer_create(timer10msCallback, 10, NULL);
  lv_timer_create(timer100msCallback, 100, NULL);

  // initialize GUI created by EEZ
  ui_init();

  guiInitInput();

  // start endless loop
  driver_backends_run_loop();
}
