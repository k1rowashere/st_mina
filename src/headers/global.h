#pragma once

#include "constants.h"
#include "stepper.h"

// global state
// TODO: reduce global state
namespace G
{
    extern Stepper stepper_0;
    extern Stepper stepper_1;

    extern uint32_t set_pos[2];       // Current position in steps
    extern Actions current_action[2]; // Current action

    extern bool redraw_set_vol; // Flag for redrawing the volume
    extern bool redraw_action;  // Flag for redrawing the action
    extern bool not_saved;      // Flag for saving the volume to EEPROM
}
