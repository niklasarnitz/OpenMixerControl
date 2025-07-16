/*
  Control software for Behringer X32 using OpenX32
  https://github.com/xn--nding-jua/OpenX32
  v0.0.1, 16.07.2025

  Parts of this software with kind permission of Music Tribe. Thank you!

  TODO:
  - Value 0xFE must not appear inside class, index or data-part of the message. Encode 0xFE as 0xFE, 0xFF instead
*/

#include "x32ctrl.h"

/*
// called every 50ms
void timer50msCallback(int signum) {
}
*/

void timer10msCallback(int sig, siginfo_t *si, void *uc) {
  // call EEZ-GUI tick
  ui_tick();

  // reads data from surface and calls surfaceCallback()
  uartRead();
}

void surfaceInit() {
	// set brightness and contrast
    printf("  Setting brightness and contrast for all displays...\n");
    setBrightness(4, 255); // brightness of LEDs
    setBrightness(5, 255);
    setBrightness(8, 255);
    setContrast(4, 0x24); // contrast of LCDs
    setContrast(5, 0x24);
    setContrast(8, 0x24);

    printf("  Setting Button-LEDs for boards 0 and 1...\n");
    setLed(0, 0, 1); // boardId 0 = user-section
    setLed(1, 1, 1); // boardId 1 = EQ-section and monitor

    printf("  Setting Button-LEDs for boards 4, 5 and 8...\n");
    for (uint8_t i=0; i<=8; i++) {
      // select-LEDs
      setLed(4, 20+i, 1); // boardId 4 = faderL
      setLed(5, 20+i, 1); // boardId 5 = faderM
      setLed(8, 20+i, 1); // boardId 8 = faderR

      // cue leds
      setLed(4, 30+i, 1); // boardId 4 = faderL
      setLed(5, 30+i, 1); // boardId 5 = faderM
      setLed(8, 30+i, 1); // boardId 8 = faderR

      // mute leds
      setLed(4, 40+i, 1); // boardId 4 = faderL
      setLed(5, 40+i, 1); // boardId 5 = faderM
      setLed(8, 40+i, 1); // boardId 8 = faderR

      // random leds
      setLed(4, 0+i, 1); // boardId 4 = faderL
      setLed(5, 0+i, 1); // boardId 5 = faderM
      setLed(8, 0+i, 1); // boardId 8 = faderR
    }

    // fader ranges from 0...8
    printf("  Setting Faders for boards 4, 5 and 8...\n");
    for (uint8_t i=0; i<=8; i++) {
      setFader(4, i, 0x03FF);
      setFader(5, i, 0x03FF);
      setFader(8, i, 0x03FF);
    }

    // meterLED ranges from 1...9
    printf("  Setting VU-Meters for boards 4, 5 and 8...\n");
    for (uint8_t i=0; i<=8; i++) {
      setMeterLed(4, i, 0xFF);
      setMeterLed(5, i, 0xFF);
      setMeterLed(8, i, 0xFF);
    }

    // set display
    printf("  Setting Displays for board 0...\n");
    for (uint8_t i=0; i<4; i++) {
      setLcd(0, i, 7, 0xE2, 0x20, 0, 0, "Board 0", 0x00, 0, 48, "OpenX32"); // setLcd(boardId, index, color, icon, sizeA, xA, yA, const char* strA, sizeB, xB, yB, const char* strB)
    }

    printf("  Setting Displays for boards 4, 5 and 8...\n");
    for (uint8_t i=0; i<=8; i++) {
      setLcd(4, i, 1, 0xE2, 0x20, 0, 0, "Board 1", 0x00, 0, 48, "OpenX32"); // setLcd(boardId, index, color, icon, sizeA, xA, yA, const char* strA, sizeB, xB, yB, const char* strB)
      setLcd(5, i, 2, 0xE2, 0x20, 0, 0, "Board 2", 0x00, 0, 48, "OpenX32"); // setLcd(boardId, index, color, icon, sizeA, xA, yA, const char* strA, sizeB, xB, yB, const char* strB)
      setLcd(6, i, 4, 0xE2, 0x20, 0, 0, "Board 3", 0x00, 0, 48, "OpenX32"); // setLcd(boardId, index, color, icon, sizeA, xA, yA, const char* strA, sizeB, xB, yB, const char* strB)
    }
}

// function will be called by uart-receive-function
void surfaceCallback(uint8_t boardId, uint8_t class, uint8_t index, uint8_t value) {
  if (class == 'f') {
      float pct = value / 40.95; // convert to percent
      printf("Fader   : boardId = 0x%02X | class = 0x%02X | index = 0x%02X | data = 0x%04X = %f\n", boardId, class, index, value, pct);
  }else if (class == 'b') {
      printf("Button  : boardId = 0x%02X | class = 0x%02X | index = 0x%02X | data = 0x%02X\n", boardId, class, index, value);

      // register buttons
      switch (boardId) {
          case 0: // monitor-section
            break;
          case 1: // eq-section and screen-area
            switch(index) {
              case 0: // left
            break;
              case 1: // right
            break;
              case 2: // up
            break;
              case 3: // down
            break;
            }
            break;
          case 4: // faderBoard left
            break;
          case 5: // faderBoard middle
            break;
          case 8: // faderBoard right
            break;
      }

      // inform LVGL about this new button-press
      guiNewButtonPress(((uint16_t)boardId << 8) + index, value > 0);
  }else if (class == 'e') {
      printf("Encoder : boardId = 0x%02X | class = 0x%02X | index = 0x%02X | data = 0x%02X\n", boardId, class, index, value);
  }
}

// the main-function - of course
int main() {
    srand(time(NULL));
    printf("OpenX32 UserInterface\n");
    printf("v0.0.1, 16.07.2025\n");
    printf("https://github.com/xn--nding-jua/OpenX32\n");

    printf("Connecting to UART1...\n");
    uartOpen();

    printf("Initializing X32 Surface...\n");
    surfaceInit();

/*
    printf("Start Timer...\n");
    initTimer();

    printf("Wait for incoming data on /dev/ttymxc1...\n");
    printf("Press Ctrl+C to terminate program.\n");
    while (1) {
      uartRead();

      // sleep for 1ms to lower CPU-load
      usleep(1000);
    }
*/

    printf("Initializing GUI...\n");
    guiInit(); // initializes LVGL and FBDEV and starts endless loop

    return 0;
}
