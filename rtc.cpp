// ===============================================================
//  INCLUDES
// ===============================================================
#include <Arduino.h>
#include <RTClib.h>
#include "rtc.h"

// ===============================================================
//  RTC CONST & VAR
// ===============================================================
static uint8_t currentSeconds = 0;
static uint8_t currentMinutes = 0;
static uint8_t currentHours = 0;

static unsigned long previousMillis = 0;

// ===============================================================
//  CONFIG
// ===============================================================
// ----------------------------- rtc -----------------------------
static RTC_DS1307 rtc;

// ===============================================================
//  RTC FUNCs
// ===============================================================
// ---------------------------- time -----------------------------
uint8_t rtcGetSeconds() {return currentSeconds;}
uint8_t rtcGetMinutes() {return currentMinutes;}
uint8_t rtcGetHours() {return currentHours;}

// --------------------------- setup -----------------------------
// iDS1307: indirizzo I2C 0x68, verificato con lo scanner
void rtcSetup() {
  if(rtc.begin() == false) {
    Serial.println("RTC NON RISPONDE.");
    return;
  }

  if(rtc.isrunning() == false) {
    Serial.println("RTC IS RUNNING FALSE.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

}

// --------------------------- update ----------------------------
void rtcUpdate() {
  unsigned long currentMillis = millis();

  if((currentMillis - previousMillis) >= 1000) {
    previousMillis = currentMillis;
    DateTime now = rtc.now();                                  // ora presa dal modulo

    currentSeconds = now.second();
    currentMinutes = now.minute();
    currentHours   = now.hour();
  }
  
}
