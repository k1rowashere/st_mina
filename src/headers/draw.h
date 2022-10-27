#pragma once
#include <Adafruit_GFX.h>  // Core graphics library
#include <MCUFRIEND_kbv.h> // Hardware-specific library
#include "constants.h"
#include "helpers.h"

namespace Draw
{
    void init(const Actions (&current_action)[2], bool pos_unlock);
    void action(const Actions (&actions)[2]);
    void volume_indicator(uint32_t curr_vol_pos, uint32_t set_vol_pos, uint16_t x_offset = 0);
    void plus_minus_buttons(uint16_t start_pos = 0, uint16_t color = TFT_WHITE);
    void lock_button(uint16_t color = TFT_WHITE);
    void apply_cancel_buttons();
    void apply_success();
    void clear_buttons();
}

const uint8_t lock_bmp[] PROGMEM = {
    // 'lock, 24x30px
    0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x01, 0xff, 0x80, 0x03, 0xff, 0xc0, 0x07, 0xc3, 0xe0, 0x0f,
    0x81, 0xf0, 0x0f, 0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x0e, 0x00, 0x70, 0x0e, 0x00, 0x70, 0x0e, 0x00,
    0x70, 0x3f, 0xff, 0xfc, 0x7f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff,
    0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xfe};

const uint8_t open_lock_bmp[] PROGMEM = {
    // 'open_lock, 24x30px
    0x00, 0xff, 0x00, 0x01, 0xff, 0x80, 0x03, 0xff, 0xc0, 0x07, 0xc3, 0xe0, 0x0f, 0x81, 0xf0, 0x0f,
    0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x0e, 0x00, 0x30, 0x0e, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x0e, 0x00,
    0x00, 0x3f, 0xff, 0xfc, 0x7f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff,
    0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xfe};