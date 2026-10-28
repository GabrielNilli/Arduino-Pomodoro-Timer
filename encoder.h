// ===============================================================
//  PUBLIC ENCODER INTERFACE
// ===============================================================
#pragma once

// ===============================================================
//  PROTOTYPES
// ===============================================================
void encoderSetup();
void encoderUpdate();

int encoderConsumeSteps();
bool encoderConsumeClick();