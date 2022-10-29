#pragma once

#include "constants.h"
#include "stepper.h"
#include "filler.h"

// global state
// TODO: reduce global state
namespace G
{
    extern Stepper stepper_0;
    extern Stepper stepper_1;

    extern Filler filler_0;
    extern Filler filler_1;

    extern uint16_t vis_set_pos[2];   // Current position in steps
    extern Actions current_action[2]; // Current action

    extern bool redraw_set_vol; // Flag for redrawing the volume
    extern bool not_saved;      // Flag for saving the volume to EEPROM
    extern bool pos_unlock;     // Flag for locking the position
}
