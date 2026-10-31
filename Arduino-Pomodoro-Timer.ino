// ===============================================================
//  INCLUDE
// ===============================================================
// -------------------------- libraries --------------------------
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ------------------------- components --------------------------
#include "buzzer.h"
#include "tick.h"
#include "encoder.h"
#include "rtc.h"

// ===============================================================
//  CONFIG
// ===============================================================
// --------------------------- display ---------------------------
// #define TFT_DC 9
// #define TFT_CS 10
// #define TEXT_SIZE 10

// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// ===============================================================
//  BEEP & TICK FUNCTION
// ===============================================================
void beepAndTick() {
    buzzerUpdate();
    tickUpdate();
}

// ===============================================================
//  SETUP
// ===============================================================
void setup() {
    Serial.begin(115200);
    buzzerSetup();
    tickSetup();
    encoderSetup();
    rtcSetup();
}

// ===============================================================
//  LOOP
// ===============================================================
void loop() {
    // ------------------------- encoder -------------------------
    encoderUpdate();
    int step = encoderConsumeSteps();

    if (encoderConsumeClick()) {
        Serial.println("CLICK");
    }

    if (step != 0) {
        Serial.println(step);
    }

    // --------------------------- rtc ---------------------------
    rtcUpdate();
    uint8_t secs = rtcGetSeconds();
    uint8_t mins = rtcGetMinutes();
    uint8_t hours = rtcGetHours();

    static uint8_t lastSecond = 99; 

    if (secs != lastSecond) {
        if (hours < 10) Serial.print("0");
        Serial.print(hours);
        Serial.print(":");
        
        if (mins < 10) Serial.print("0");
        Serial.print(mins);
        Serial.print(":");
        
        if (secs < 10) Serial.print("0");
        Serial.println(secs);

        lastSecond = secs;
    }
}
