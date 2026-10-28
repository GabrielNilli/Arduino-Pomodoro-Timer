// ===============================================================
//  INCLUDES
// ===============================================================
#include <Arduino.h>
#include "tick.h"

// ===============================================================
//  TICK CONST & VAR
// ===============================================================
static const unsigned long TICK_INTERVAL = 3000;    // intervallo tra un TICK e l'altro
static unsigned long lastTick = 0;                  // quando è avvemuto l'ultimo TICK

// ===============================================================
//  TICK FUNCs
// ===============================================================
// --------------------------- setup -----------------------------
void tickSetup() {}

// --------------------------- update ----------------------------
void tickUpdate() {
    unsigned long now = millis();                   // viene settato now in millisecondi

    if(now - lastTick >= TICK_INTERVAL) {           // se l'istante di adesso - il tempo dell'ultimo TICK è maggiore o uguale all'intervallo previsto
        lastTick = now;                             // settiamo l'ultimo TICK ad ora
        Serial.println("TICK");                     // e scriviamo TICK
    }
}
