#ifndef X32CTRL_H_
#define X32CTRL_H_

// X32 Destination System

//#define X32_MODEL_FULL
#define X32_MODEL_COMPACT
//#define X32_MODEL_PRODUCER
//#define X32_MODEL_RACK
//#define X32_MODEL_CORE
// TODO: read X32-SD-Card and check which device-type we are



// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <linux/input.h>

// special includes
#include <string.h>

// includes for timer
#include <time.h>
#include <signal.h>

// includes for uart
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h> // for FIONREAD

// includes for lvgl
#include "lv_port_linux/lvgl/lvgl.h"
#include "lv_port_linux/src/lib/backends.h"

// including the GUI built by EEZ-Studio
#include "eez/src/ui/actions.h"
#include "eez/src/ui/fonts.h"
#include "eez/src/ui/images.h"
#include "eez/src/ui/screens.h"
#include "eez/src/ui/structs.h"
#include "eez/src/ui/styles.h"
#include "eez/src/ui/ui.h"
#include "eez/src/ui/vars.h"

// our own includes
#include "gui.h"
#include "uart.h"
#include "surface.h"
#include "auxiliary.h"

// function prototypes
//void timer50Callback(int signum);
void timer10msCallback(int sig, siginfo_t *si, void *uc);
void surfaceInit();
void surfaceCallback(uint8_t boardId, uint8_t class, uint8_t index, uint16_t value);

#endif
