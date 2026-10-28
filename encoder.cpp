// ===============================================================
//  INCLUDES
// ===============================================================
#include <Arduino.h>
#include <Bounce2.h>
#include "encoder.h"

// ===============================================================
//  ENCODER CONFIGURATION & STATE
// ===============================================================
// ---------------------------- pins -----------------------------
static const uint8_t ENCODER_CLK_PIN = 2;
static const uint8_t ENCODER_DT_PIN = 3;
static const uint8_t ENCODER_BUTTON_PIN = 4;

// ---------------------------- utils ----------------------------
static Bounce2::Button button;                                      // oggetto button per il debounce
static bool awaitingClick = false;
static volatile int rotationCounter = 0;                            // volatile per impedire l'ottimizzare in memoria dato che viene modificat dall'ISR ed in qualsiasi momento

// ===============================================================
//  ENCODER BUTTON
// ===============================================================
// *************************** static ****************************
// --------------------------- setup -----------------------------
static void encoderButtonSetup() {
    button.attach (ENCODER_BUTTON_PIN , INPUT_PULLUP);              // settiamo il pin del bottone al tipo di INPUT
    button.interval(5);                                             // quanto tempo di debounce
    button.setPressedState(LOW);                                    // a che stato (LOW o HIGH) si deve attivare il bottone
}

// --------------------------- update ----------------------------
static void encoderButtonUpdate() {
    button.update();                                                // aggiorniamo lo stato ad inizio di ogni loop

    if(button.pressed()) {
        awaitingClick = true;
    }
}

// *************************** public ****************************
bool encoderConsumeClick() {                                        // mandiamo se è cliccato con una variabile clone e lo risettiamo a false
    bool isClicked = awaitingClick;
    awaitingClick = false;
    return isClicked;
}

// ===============================================================
//  ENCODER ROTATION
// ===============================================================
// *************************** static ****************************
// ---------------------------- isr ------------------------------
static void onClkFalling() {                                        // ISR per la rotazione
    int currentDt = digitalRead(ENCODER_DT_PIN);
    
    if(currentDt == HIGH) {
      rotationCounter--;
    } else {
        rotationCounter++;
    }
}

// --------------------------- setup -----------------------------
static void encoderRotationSetup() {
    pinMode(ENCODER_CLK_PIN, INPUT);                                // settiamo le modalità dei pin
    pinMode(ENCODER_DT_PIN, INPUT);

    attachInterrupt(                                                // automatizza
        digitalPinToInterrupt(ENCODER_CLK_PIN),                     // fa capire alla Nano che il pin restituisce interrupt 0
        onClkFalling,                                               // la funzione richiamata
        FALLING                                                     // per i pin che vanno da HIGH a LOW
    );
}

// --------------------------- update ----------------------------
// static void encoderRotationUpdate() {}                           // tenuta ma commentata solo per coerenza strutturale con altri file

// *************************** public ****************************
int encoderConsumeSteps() {
    noInterrupts();                                                 // blocca gli interrupts per evitare errori 
    int counter = rotationCounter;      
    rotationCounter = 0;        
    interrupts();                                                   // sblocca gli interrupts
    return counter;
}

// ===============================================================
//  ENCODER EXPORTs
// ===============================================================
void encoderSetup() {
    encoderButtonSetup();
    encoderRotationSetup();
}

void encoderUpdate() {
    encoderButtonUpdate();
    // encoderRotationUpdate();
}
