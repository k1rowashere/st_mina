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
    void fill_v(uint32_t current_volume_pos, uint16_t start_pos = 0);
    void plus_minus_buttons(uint16_t start_pos = 0);
    void apply_cancel_buttons();
    void apply_success();
    void clear_buttons();
}
