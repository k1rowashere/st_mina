#pragma once

#include "constants.h"
#include "draw.h"
#include "filler.h"
#include "stepper.h"
#include "touch.h"
#include "touch_handles.h"
#include "utils.h"

Stepper steppers[] = {
    (Stepper::Pins){STEP_PIN_0, DIR_PIN_0, EN_PIN_0, LOW_LIMIT_SWITCH_PIN_0,
                    HIGH_LIMIT_SWITCH_PIN_0},
    (Stepper::Pins){STEP_PIN_1, DIR_PIN_1, EN_PIN_1, LOW_LIMIT_SWITCH_PIN_1,
                    HIGH_LIMIT_SWITCH_PIN_1},
};

Filler fillers[] = {
    (Filler::Pins){LOAD_SOLENOID_0, UNLOAD_SOLENOID_0, FILL_LIMIT_FULL_0,
                   FILL_LIMIT_EMPTY_0, FILL_PEDAL_PIN},
    (Filler::Pins){LOAD_SOLENOID_1, UNLOAD_SOLENOID_1, FILL_LIMIT_FULL_1,
                   FILL_LIMIT_EMPTY_1, FILL_PEDAL_PIN},
};

uint16_t vis_set_pos[2];
Status current_status[2]; // Current status

// init touchscreen event handlers
/// Flags
bool not_saved = false;   // Flag for saving the volume to EEPROM
bool pos_unlock = true;   // Flag for locking the position
bool clear_error = false; // Flag for clearing error
bool ack_flag = false;    // Flag for showing ack button

// args to pass to touch handlers
constexpr struct Touch::Args touch_args PROGMEM = {
    fillers,
    vis_set_pos,
    current_status,
    not_saved,
    pos_unlock,
    clear_error,
};

using namespace TH;
// x, y, w, h, callback, condition
constexpr Touch::Handle handles[] = {
    {SCREEN_WIDTH / 4 - 70, SCREEN_HEIGHT / 2 - 25, 50, 50, inc_dec<0, -1>, pos_unlock},
    {SCREEN_WIDTH / 4 + 20, SCREEN_HEIGHT / 2 - 25, 50, 50, inc_dec<0, 1>, pos_unlock},
    {SCREEN_WIDTH / 4 * 3 - 70, SCREEN_HEIGHT / 2 - 25, 50, 50, inc_dec<1, -1>, pos_unlock},
    {SCREEN_WIDTH / 4 * 3 + 20, SCREEN_HEIGHT / 2 - 25, 50, 50, inc_dec<1, 1>, pos_unlock},
    {SCREEN_WIDTH / 4 - 50, SCREEN_HEIGHT - 50, 100, 50, apply, not_saved},
    {SCREEN_WIDTH / 4 * 3 - 50, SCREEN_HEIGHT - 50, 100, 50, cancel, not_saved},
    {SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50, lock, true},
    {SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT - 50, 50, 50, ack, ack_flag},
    {SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT / 2 + 50, 100, 50, empty, true},
    {SCREEN_WIDTH / 2 + 25, SCREEN_HEIGHT / 2 + 50, 100, 50, fill, true},
};
