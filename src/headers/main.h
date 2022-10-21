#pragma once

// includes
#include <EEPROM.h>
#include "constants.h"
#include "stepper.h"
#include "draw.h"
#include "touch.h"
#include "../touch_handles.cpp"
// #include "touch.h"
// global state
// TODO: reduce global state

uint32_t current_pos[2];                        // Current position in steps
Actions current_action[2] = {HOMMING, HOMMING}; // Current action

bool redraw_set_vol = true; // Flag for redrawing the volume
bool redraw_action = true;  // Flag for redrawing the action
bool not_saved = false;     // Flag for saving the volume to EEPROM

// (STEP_PIN, DIR_PIN, EN_PIN, LOW_LIMIT_SWITCH_PIN, HIGH_LIMIT_SWITCH_PIN)
Stepper stepper_0(22, 24, 26, 28, 30);
Stepper stepper_1(23, 25, 27, 29, 31);

void temp()
{
    Serial.println("temp");
}
