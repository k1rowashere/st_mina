#pragma once

// includes
#include <EEPROM.h>
#include <Arduino.h>

#include "constants.h"
#include "global.h"
#include "stepper.h"
#include "filler.h"
#include "ultra_sonic.h"
#include "draw.h"
#include "touch.h"
#include "touch_handles.h"

Stepper steppers[] = {
    (Stepper::Pins){STEP_PIN_0, DIR_PIN_0, EN_PIN_0, LOW_LIMIT_SWITCH_PIN_0, HIGH_LIMIT_SWITCH_PIN_0},
    (Stepper::Pins){STEP_PIN_1, DIR_PIN_1, EN_PIN_1, LOW_LIMIT_SWITCH_PIN_1, HIGH_LIMIT_SWITCH_PIN_1},
};

Filler fillers[] = {
    (Filler::Pins){LOAD_SOLENOID_0, UNLOAD_SOLENOID_0, FILL_LIMIT_FULL_0, FILL_LIMIT_EMPTY_0, FILL_PEDAL_PIN},
    (Filler::Pins){LOAD_SOLENOID_1, UNLOAD_SOLENOID_1, FILL_LIMIT_FULL_1, FILL_LIMIT_EMPTY_1, FILL_PEDAL_PIN},
};

uint16_t G::vis_set_pos[2];
Status current_status[2]; // Current status

bool not_saved = false;    // Flag for saving the volume to EEPROM
bool G::pos_unlock = true; // Flag for locking the position

// init touchscreen event handlers
bool always_true = true;
// x0, x1, y0, y1, callback, condition
constexpr Touch::Handle handles[] = {
    {SCREEN_WIDTH / 4 - 70, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(0, NEGATIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 + 20, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(0, POSITIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 * 3 - 70, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(1, NEGATIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 * 3 + 20, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(1, POSITIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 - 50, SCREEN_HEIGHT - 50, 100, 50, apply, not_saved},
    {SCREEN_WIDTH / 4 * 3 - 50, SCREEN_HEIGHT - 50, 100, 50, cancel, not_saved},
    {SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50, lock, always_true},
};