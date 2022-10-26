#pragma once

// includes
#include <EEPROM.h>
#include <Arduino.h>

#include "constants.h"
#include "global.h"
#include "stepper.h"
#include "filler.h"
#include "draw.h"
#include "touch.h"
#include "touch_handles.h"

// global state
// TODO: reduce global state

// (STEP_PIN, DIR_PIN, EN_PIN, LOW_LIMIT_SWITCH_PIN, HIGH_LIMIT_SWITCH_PIN)
Stepper G::stepper_0({STEP_PIN_0, DIR_PIN_0, EN_PIN_0, LOW_LIMIT_SWITCH_PIN_0, HIGH_LIMIT_SWITCH_PIN_0});
Stepper G::stepper_1({STEP_PIN_1, DIR_PIN_1, EN_PIN_1, LOW_LIMIT_SWITCH_PIN_1, HIGH_LIMIT_SWITCH_PIN_1});

// (LOAD_SOLENOID, UNLOAD_SOLENOID, FILL_LIMIT_FULL, FILL_LIMIT_EMPTY)
Filler G::filler_0({LOAD_SOLENOID_0, UNLOAD_SOLENOID_0, FILL_LIMIT_FULL_0, FILL_LIMIT_EMPTY_0});
Filler G::filler_1({LOAD_SOLENOID_1, UNLOAD_SOLENOID_1, FILL_LIMIT_FULL_1, FILL_LIMIT_EMPTY_1});

uint32_t G::vis_set_pos[2];
Actions G::current_action[2]; // Current action

bool G::redraw_set_vol = true; // Flag for redrawing the volume
bool G::not_saved = false;     // Flag for saving the volume to EEPROM
bool G::pos_unlock = false;    // Flag for locking the position

// init touchscreen event handlers
bool always_true = true;
// x0, x1, y0, y1, callback, condition
constexpr Touch::Handle handles[] = {
    {SCREEN_WIDTH / 4 - 70, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(0, NEGATIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 + 20, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(0, POSITIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 * 3 - 70, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(1, NEGATIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 * 3 + 20, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(1, POSITIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 - 50, SCREEN_HEIGHT - 50, 100, 50, apply, G::not_saved},
    {SCREEN_WIDTH / 4 * 3 - 50, SCREEN_HEIGHT - 50, 100, 50, cancel, G::not_saved},
    {SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50, lock, always_true},
};