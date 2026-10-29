// ===============================================================
//  INCLUDES
// ===============================================================
#include <Arduino.h>
#include <Bounce2.h>
#include <RotaryEncoder.h>
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
static bool pendingClick = false;

// Istanza della libreria di Matthias Hertel (usa lo stile di latch a 4 step standard)
static RotaryEncoder encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, RotaryEncoder::LatchMode::FOUR3);

static volatile int pendingSteps = 0;                               // passi accumulati pronti per essere consumati
static long lastPosition = 0;                                        // tiene traccia dell'ultima posizione letta dall'encoder

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
        pendingClick = true;
    }
}

// *************************** public ****************************
bool encoderConsumeClick() {                                        // mandiamo se è cliccato con una variabile clone e lo risettiamo a false
    bool isClicked = pendingClick;
    pendingClick = false;
    return isClicked;
}

// ===============================================================
//  ENCODER ROTATION
// ===============================================================
// *************************** static ****************************
// ---------------------------- isr ------------------------------
// ISR che delega il controllo dei pin alla libreria Hertel
static void checkPositionISR() {
    encoder.tick();
}

// --------------------------- setup -----------------------------
static void encoderRotationSetup() {
    // Configurazione pin (la libreria raccomanda INPUT_PULLUP se non hai pull-up esterni)
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);                                
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);

    // Sincronizziamo la posizione iniziale
    encoder.tick();
    lastPosition = encoder.getPosition();

    // Agganciamo l'ISR ai pin per tracciare la rotazione in tempo reale
    attachInterrupt(digitalPinToInterrupt(ENCODER_CLK_PIN), checkPositionISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_DT_PIN), checkPositionISR, CHANGE);
}

// --------------------------- update ----------------------------
static void encoderRotationUpdate() {
    // Leggiamo la posizione attuale (calcolata in background tramite l'ISR)
    long currentPosition = encoder.getPosition();
    
    if (currentPosition != lastPosition) {
        // Calcoliamo la differenza di passi (positivo o negativo)
        int delta = currentPosition - lastPosition;
        
        noInterrupts();
        pendingSteps += delta;
        interrupts();
        
        lastPosition = currentPosition;
    }
}                           

// *************************** public ****************************
int encoderConsumeSteps() {
    noInterrupts();                                                 // blocca gli interrupts per evitare conflitti sulla variabile
    int steps = pendingSteps;      
    pendingSteps = 0;        
    interrupts();                                                   // sblocca gli interrupts
    return steps;
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
    encoderRotationUpdate();
}
