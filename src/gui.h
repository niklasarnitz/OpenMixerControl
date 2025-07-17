#ifndef GUI_H_
#define GUI_H_

#include "x32ctrl.h"

extern backend_t *sel_display_backend;

void guiInitInput();
void guiInit();
void guiNewButtonPress(uint16_t button, bool pressed);
void timer100msCallback(int sig, siginfo_t *si, void *uc);

#endif
