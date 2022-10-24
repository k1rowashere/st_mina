#pragma once

// includes
#include <EEPROM.h>
#include <Arduino.h>

#include "constants.h"
#include "global.h"
#include "stepper.h"
#include "draw.h"
#include "touch.h"
#include "touch_handles.h"

// global state
// TODO: reduce global state

// (STEP_PIN, DIR_PIN, EN_PIN, LOW_LIMIT_SWITCH_PIN, HIGH_LIMIT_SWITCH_PIN)
Stepper G::stepper_0(22, 24, 26, 28, 30);
Stepper G::stepper_1(23, 25, 27, 29, 31);

uint32_t G::vis_set_pos[2];
Actions G::current_action[2]; // Current action

bool G::redraw_set_vol = true; // Flag for redrawing the volume
bool G::redraw_action = true;  // Flag for redrawing the action
bool G::not_saved = false;     // Flag for saving the volume to EEPROM
bool G::pos_unlock = true;     // Flag for locking the position
