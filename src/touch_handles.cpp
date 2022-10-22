#include "headers/touch_handles.h"

void inc_dec(uint16_t hold_counter, uint32_t &value, Sign sign)
{
    if (hold_counter > 20)
        stepper_0.set_pos -= volume_to_steps(40);
    else if (hold_counter > 10)
        current_pos[0] -= volume_to_steps(10);
    else
        current_pos[0] -= volume_to_steps(1);

    // clamp value 0-MAX_POS
    if (current_pos[0] > MAX_POS)
        current_pos[0] = MAX_POS;
    else if (current_pos[0] < 0)
        current_pos[0] = 0;

    delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
    redraw_set_vol = true;
}

// TODO: remove duplicate code
void inc_dec_0_0(uint16_t hold_counter)
{
    if (hold_counter > 20)
        stepper_0.set_pos -= volume_to_steps(40);
    else if (hold_counter > 10)
        current_pos[0] -= volume_to_steps(10);
    else
        current_pos[0] -= volume_to_steps(1);

    // clamp value 0-MAX_POS
    if (current_pos[0] > MAX_POS)
        current_pos[0] = MAX_POS;
    else if (current_pos[0] < 0)
        current_pos[0] = 0;

    delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
    redraw_set_vol = true;
}

void inc_dec_0_1(uint16_t hold_counter)
{
    if (hold_counter > 20)
        current_pos[0] += volume_to_steps(40);
    else if (hold_counter > 10)
        current_pos[0] += volume_to_steps(10);
    else
        current_pos[0] += volume_to_steps(1);

    // clamp value 0-MAX_POS
    if (current_pos[0] > MAX_POS)
        current_pos[0] = MAX_POS;
    else if (current_pos[0] < 0)
        current_pos[0] = 0;

    delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
    redraw_set_vol = true;
}

void inc_dec_1_0(uint16_t hold_counter)
{
    if (hold_counter > 20)
        current_pos[1] -= volume_to_steps(40);
    else if (hold_counter > 10)
        current_pos[1] -= volume_to_steps(10);
    else
        current_pos[1] -= volume_to_steps(1);

    // clamp value 0-MAX_POS
    if (current_pos[1] > MAX_POS)
        current_pos[1] = MAX_POS;
    else if (current_pos[1] < 0)
        current_pos[1] = 0;

    delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
    redraw_set_vol = true;
}

void inc_dec_1_1(uint16_t hold_counter)
{
    if (hold_counter > 20)
        current_pos[1] += volume_to_steps(40);
    else if (hold_counter > 10)
        current_pos[1] += volume_to_steps(10);
    else
        current_pos[1] += volume_to_steps(1);

    // clamp value 0-MAX_POS
    if (current_pos[1] > MAX_POS)
        current_pos[1] = MAX_POS;
    else if (current_pos[1] < 0)
        current_pos[1] = 0;

    delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
    redraw_set_vol = true;
}

// apply button handler
void apply(uint16_t hold_counter)
{
    EEPROM.put(0, current_pos[0]);
    EEPROM.put(4, current_pos[1]);

    stepper_0.set_pos = current_pos[0];
    stepper_1.set_pos = current_pos[1];

    Draw::clear_buttons();
}

// cancel button handler
void cancel(uint16_t hold_counter)
{
    EEPROM.get(0, current_pos[0]);
    EEPROM.get(4, current_pos[1]);

    stepper_0.set_pos = current_pos[0];
    stepper_1.set_pos = current_pos[1];

    Draw::clear_buttons();
}
