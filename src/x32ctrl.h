#ifndef X32CTRL_H_
#define X32CTRL_H_

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

// special includes
#include <string.h>

// includes for timer
#include <time.h>
#include <signal.h>

// includes for uart
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h> // for FIONREAD

// own includes
#include "uart.h"
#include "surface.h"
#include "auxiliary.h"

// function prototypes
void timerCallback(int signum);
void surfaceCallback(uint8_t boardId, uint8_t class, uint8_t index, uint8_t value);

#endif
