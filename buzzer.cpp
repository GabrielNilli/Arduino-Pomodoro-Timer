// ===============================================================
//  INCLUDES
// ===============================================================
#include <Arduino.h>
#include "buzzer.h"

// ===============================================================
//  BUZZER CONST & VAR
// ===============================================================
uint8_t BUZZER_PIN = 5;                             // pin del buzzer
static unsigned long lastBeep = 0;                  // quando ha suonato l'ultima volta
static const unsigned long BEEP_INTERVAL = 1000;    // quanto dura l'intervallo tra un suono e l'altro

// ===============================================================
//  BUZZER FUNCs
// ===============================================================
// --------------------------- setup -----------------------------
void buzzerSetup() {
    pinMode(BUZZER_PIN, OUTPUT);                    // pin settato a OUTPUT
}

// --------------------------- update ----------------------------
void buzzerUpdate() {
    unsigned long now = millis();                   // viene settato now in millisecondi

    if(now - lastBeep >= BEEP_INTERVAL) {           // se l'istante di adesso - il tempo dell'ultimo suono è maggiore o uguale all'intervallo previsto
        lastBeep = now;                             // settiamo l'ultimo suono ad ora
        tone(BUZZER_PIN, 500, 200);                 // e facciamo suonare ad una frequenza di 500 per 200 millisecondi
    }
}
