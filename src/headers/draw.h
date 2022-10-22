#pragma once
#include <Adafruit_GFX.h>  // Core graphics library
#include <MCUFRIEND_kbv.h> // Hardware-specific library
#include "constants.h"
#include "helpers.h"
namespace Draw
{
    void init();
    void title();
    void action(Actions action, uint16_t start_pos = 0);
    void volume_indicator(uint32_t curr_vol_pos, uint32_t set_vol_pos, uint16_t x_offset = 0);
    void plus_minus_buttons(uint16_t start_pos = 0);
    void apply_cancel_buttons();
    void apply_success();
    void clear_buttons();
}
