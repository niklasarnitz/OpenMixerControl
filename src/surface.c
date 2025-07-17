#include "surface.h"

// boardId = 0, 1, 4, 5, 8
// index = 0 ... 8
// position = 0x0000 ... 0x0FFF
void setFader(uint8_t boardId, uint8_t index, uint16_t position) {
  // 0xFE, 0x8i, class, index, data[], 0xFE, chksum
  // 0x46, i, position.w * (1..8(9))

  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'F'); // class: F = Fader
  messageBuilderAddDataByte(&message, index); // index
  messageBuilderAddDataByte(&message, (position & 0xFF)); // 
  messageBuilderAddDataByte(&message, (unsigned char)((position & 0x0F00) >> 8)); // 
  messageBuilderAddRawByte(&message, 0xFE); // endbyte

  uartTxData(&message);
}

// boardId = 0, 1, 4, 5, 8
// index = 0 ... 8
// ledId = individual index of led
// state = 0 / 1
void setLed(uint8_t boardId, uint8_t ledId, uint8_t state) {
  // 0xFE, 0x8i, class, index, data[], 0xFE, chksum
  // 0x4C, 0x80, leds.b[]

  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'L'); // class: L = LED
  messageBuilderAddDataByte(&message, index); // index
  if (state > 0) {
    // turn LED on
    messageBuilderAddDataByte(&message, ledId + 0x80);
  }else{
    // turn LED off
    messageBuilderAddDataByte(&message, ledId);
  }
  messageBuilderAddRawByte(&message, 0xFE); // endbyte

  uartTxData(&message);
}

// boardId = 0, 1, 4, 5, 8
// index = 0 ... 8
// leds = 8-bit bitwise (bit 0=-60dB ... 4=-6dB, 5=Clip, 6=Gate, 7=Comp)
void setMeterLed(uint8_t boardId, uint8_t index, uint8_t leds) {
  // 0xFE, 0x8i, class, index, data[], 0xFE, chksum
  // 0x4C, index, leds.b[]

  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'M'); // class: M = Meter
  messageBuilderAddDataByte(&message, index); // index
  messageBuilderAddDataByte(&message, leds); // 
  messageBuilderAddRawByte(&message, 0xFE); // endbyte

  uartTxData(&message);
}

// boardId = 0, 1, 4, 5, 8
// index = 0 ... 8
// leds = 16-bit value. 
void setEncoderRing(uint8_t boardId, uint8_t index, uint8_t ledMode, uint8_t ledPct, bool backlight) {
  // 0xFE, 0x8i, class, index, data[], 0xFE, chksum
  // 0x52, index, leds.w[]

  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'R'); // class: R = Ring
  messageBuilderAddDataByte(&message, index); // index

  uint16_t leds = 0;
  switch (ledMode) {
	case 1: // calcEncoderRingLedIncrement
		leds = calcEncoderRingLedIncrement(ledPct);
		break;
	case 2: //
		leds = calcEncoderRingLedPosition(ledPct);
		break;
	case 3: //
		leds = calcEncoderRingLedBalance(ledPct);
		break;
	case 4: //
		leds = calcEncoderRingLedWidth(ledPct);
		break;
  }
  messageBuilderAddDataByte(&message, leds & 0xFF); // 
  if (backlight) {
    messageBuilderAddDataByte(&message, (leds & 0x7F00 >> 8) + 0x80);
  }else{
    messageBuilderAddDataByte(&message, (leds & 0x7F00 >> 8));
  }
  messageBuilderAddRawByte(&message, 0xFE); // endbyte
  
  uartTxData(&message);
}

// boardId = 0, 1, 4, 5, 8
// index = 0 ... 8
// brightness = 0 ... 255
void setBrightness(uint8_t boardId, uint8_t brightness) {
  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'C'); // class: C = Controlmessage
  messageBuilderAddDataByte(&message, 'B'); // index
  messageBuilderAddDataByte(&message, brightness); // 
  messageBuilderAddRawByte(&message, 0xFE); // endbyte

  uartTxData(&message);
}

// boardId = 0, 1, 4, 5, 8
// index = 0 ... 8
// brightness = 0 ... 255
void setContrast(uint8_t boardId, uint8_t contrast) {
  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'C'); // class: C = Controlmessage
  messageBuilderAddDataByte(&message, 'C'); // index
  messageBuilderAddDataByte(&message, contrast & 0x3F); // 
  messageBuilderAddRawByte(&message, 0xFE); // endbyte

  uartTxData(&message);
}

// boardId = 0, 4, 5, 8
// index = 0 ... 8
// color = 0=BLACK, 1=RED, 2=GREEN, 3=YELLOW, 4=BLUE, 5=PINK, 6=CYAN, 7=WHITE
// icon = TODO
// sizeA/B = 0x00 (small) or 0x20 (large)
// xA/B = horizontal position in pixel
// yA/B = vertical position in pixel
// strA/B = String of Text to be displayed
void setLcd(uint8_t boardId, uint8_t index, uint8_t color, uint8_t xicon, uint8_t yicon, uint8_t icon, uint8_t sizeA, uint8_t xA, uint8_t yA, const char* strA, uint8_t sizeB, uint8_t xB, uint8_t yB, const char* strB) {
  // 0xFE, 0x8i, class, index, data[], 0xFE, chksum
  // 0x44, i, color.b, script[]

  messageBuilderInit(&message);

  messageBuilderAddRawByte(&message, 0xFE); // startbyte
  messageBuilderAddDataByte(&message, 0x80 + boardId); // start message for specific boardId
  messageBuilderAddDataByte(&message, 'D'); // class: D = Display
  messageBuilderAddDataByte(&message, index); // index
  messageBuilderAddDataByte(&message, color & 0x07); // use only 3 bits (bit 0=R, 1=G, 2=B)

  
  // begin of script
  // transmit icon
  messageBuilderAddDataByte(&message, icon);
  messageBuilderAddDataByte(&message, xicon);
  messageBuilderAddDataByte(&message, yicon);
  
  // transmit first text
  messageBuilderAddDataByte(&message, sizeA + strlen(strA)); // size + textLength
  messageBuilderAddDataByte(&message, xA);
  messageBuilderAddDataByte(&message, yA);
  messageBuilderAddRawByteArray(&message, strA); // this is ASCII, so we can omit byte-stuffing
  
  messageBuilderAddDataByte(&message, sizeB + strlen(strB)); // size + textLength
  messageBuilderAddDataByte(&message, xB);
  messageBuilderAddDataByte(&message, yB);
  messageBuilderAddRawByteArray(&message, strB); // this is ASCII, so we can omit byte-stuffing
  

  // end of script
  
  messageBuilderAddRawByte(&message, 0xFE); // endbyte

  uartTxData(&message);
}

// bit 0=CCW, bit 14 = CW, bit 15=encoder-backlight
// CCW <- XXXXXXX X XXXXXXX -> CW
uint16_t calcEncoderRingLedIncrement(uint8_t pct) {
    uint8_t num_leds_to_light = 0;
    if (pct <= 50) {
        // Scale 0-50 to 0-8 LEDs
        num_leds_to_light = (uint8_t)((float)pct / 50.0f * 8.0f);
    } else {
        // Scale 51-100 to 9-15 LEDs (7 more LEDs)
        // From 51 to 100, there are 50 steps.
        // We need to add (pct - 50) steps mapped to the remaining 7 LEDs.
        // (pct - 50) / 50.0f * 7.0f
        num_leds_to_light = 8 + (uint8_t)(((float)(pct - 50) / 50.0f) * 7.0f);

        if (num_leds_to_light > 15) {
            num_leds_to_light = 15;
        }
    }

    uint16_t led_mask = 0;
    // Set the lower 'num_leds_to_light' bits to 1.
    // A common way to do this is (1 << N) - 1, which creates a mask with N bits set.
    // However, if N is 0, (1 << 0) - 1 = 0, which is correct.
    if (num_leds_to_light > 0) {
        led_mask = (1U << num_leds_to_light) - 1;
    }

    // Ensure Bit 15 is always 0, as only 15 LEDs are controlled (bits 0-14).
    // The mask generated above already ensures this if num_leds_to_light is <= 15.
    // The max value for led_mask will be (1U << 15) - 1 = 0x7FFF.

    return led_mask;
}

// bit 0=CCW, bit 14 = CW, bit 15=encoder-backlight
// CCW <- XXXXXXX X XXXXXXX -> CW
uint16_t calcEncoderRingLedPosition(uint8_t pct) {
    uint8_t led_index = (uint8_t)(((float)pct / 100.0f) * 15.0f + 0.5f); // +0.5f für Rundung

    if (led_index > 15) {
        led_index = 15;
    }

    return (1U << led_index);
}

// bit 0=CCW, bit 14 = CW, bit 15=encoder-backlight
// CCW <- XXXXXXX X XXXXXXX -> CW
uint16_t calcEncoderRingLedBalance(uint8_t pct) {
    // Wenn der Wert genau 50 ist, nur Bit 7 setzen.
    if (pct == 50) {
        return (1U << 7);
    }

    uint16_t led_mask = 0;

    if (pct < 50) {
        float scale = (float)pct / 50.0f; // Skaliert 0-49 auf 0-0.98
        // 8 - (scale * 7) = Anzahl der LEDs, die an sind.
        uint8_t num_on_left_side = (uint8_t)roundf(8.0f - (scale * 7.0f));
        
        // Sicherstellen, dass mindestens Bit 7 an ist, wenn pct < 50
        if (num_on_left_side < 1) num_on_left_side = 1;

        // Setze die Bits von Bit 0 bis zum berechneten Index
        for (int i = 0; i < num_on_left_side; ++i) {
            if (i <= 7) { // Sicherstellen, dass wir im Bereich Bits 0-7 bleiben
                led_mask |= (1U << i);
            }
        }

    } else { // pct > 50
        // Skaliere (pct - 50) von 1-50 auf die Anzahl der LEDs, die von Bit 7 nach rechts zusätzlich an sein sollen.
        float scale = (float)(pct - 50) / 50.0f; // Skaliert 51-100 auf 0.02-1
        uint8_t num_on_right_side = (uint8_t)roundf(1.0f + (scale * 8.0f)); // 1 für Bit 7, plus 8 weitere LEDs
        
        // Sicherstellen, dass mindestens Bit 7 an ist, wenn pct > 50
        if (num_on_right_side < 1) num_on_right_side = 1;

        // Setze die Bits von Bit 7 bis zum berechneten Index
        for (int i = 0; i < num_on_right_side; ++i) {
            if ((7 + i) <= 15) { // Sicherstellen, dass wir im Bereich Bits 7-15 bleiben
                led_mask |= (1U << (7 + i));
            }
        }
    }

    return led_mask;
}

// bit 0=CCW, bit 14 = CW, bit 15=encoder-backlight
// CCW <- XXXXXXX X XXXXXXX -> CW
uint16_t calcEncoderRingLedWidth(uint8_t pct) {
    if (pct == 0) {
        return (1U << 7); // Setzt nur Bit 7
    }

    float scaled_value = (float)(pct - 1) / 99.0f; // Skaliert 1-100 auf 0-1

    // Anzahl der zusätzlichen LEDs, die auf der linken Seite von Bit 7 aus eingeschaltet werden sollen.
    // Max 7 LEDs (Bits 0-6).
    uint8_t num_left_additional_leds = (uint8_t)roundf(scaled_value * 7.0f);
    // Anzahl der zusätzlichen LEDs, die auf der rechten Seite von Bit 7 aus eingeschaltet werden sollen.
    // Max 8 LEDs (Bits 8-15).
    uint8_t num_right_additional_leds = (uint8_t)roundf(scaled_value * 8.0f);

    uint16_t led_mask = (1U << 7); // Starte mit Bit 7 gesetzt

    // Schalte die zusätzlichen LEDs auf der linken Seite ein
    for (int i = 0; i < num_left_additional_leds; ++i) {
        if ((7 - (i + 1)) >= 0) { // Sicherstellen, dass der Index nicht negativ wird
            led_mask |= (1U << (7 - (i + 1)));
        }
    }

    // Schalte die zusätzlichen LEDs auf der rechten Seite ein
    for (int i = 0; i < num_right_additional_leds; ++i) {
        if ((7 + (i + 1)) <= 15) { // Sicherstellen, dass der Index nicht über 15 hinausgeht
            led_mask |= (1U << (7 + (i + 1)));
        }
    }

    // Bei 100% sollen alle Bits gesetzt sein (0xFFFF).
    // Die Berechnung oben sollte dies bereits erreichen, aber eine explizite Prüfung schadet nicht.
    if (pct == 100) {
        return 0xFFFF; // Alle 16 Bits setzen
    }

    return led_mask;
}
