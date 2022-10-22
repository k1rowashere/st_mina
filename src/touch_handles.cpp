#include "headers/touch_handles.h"

// +/- buttons macro
#define INC_DEC(pos, sign)                                     \
    void inc_dec_##pos##_##sign()                              \
    {                                                          \
        static uint16_t hold_counter(0);                       \
        static uint32_t last_call_ms(0);                       \
                                                               \
        if (millis() - last_call_ms > 1000)                    \
            hold_counter = 0;                                  \
        if ((millis() - last_call_ms) > 200)                   \
        {                                                      \
            if (hold_counter > 20)                             \
                G::set_pos[pos] += sign * volume_to_steps(50); \
            else if (hold_counter > 10)                        \
                G::set_pos[pos] += sign * volume_to_steps(10); \
            else                                               \
                G::set_pos[pos] += sign * volume_to_steps(1);  \
                                                               \
            if (G::set_pos[pos] < 0)                           \
                G::set_pos[pos] = 0;                           \
            else if (G::set_pos[pos] > MAX_POS)                \
                G::set_pos[pos] = MAX_POS;                     \
                                                               \
            hold_counter++;                                    \
            G::redraw_set_vol = true;                          \
        }                                                      \
        last_call_ms = millis();                               \
    }

// define +/- buttons handlers
INC_DEC(0, NEGATIVE)
INC_DEC(0, POSITIVE)
INC_DEC(1, NEGATIVE)
INC_DEC(1, POSITIVE)

// apply button handler
void apply()
{

    G::stepper_0.set_pos = G::set_pos[0];
    G::stepper_1.set_pos = G::set_pos[1];

    // save volume to EEPROM
    EEPROM.put(0, G::set_pos[0]);
    EEPROM.put(4, G::set_pos[1]);
    Draw::apply_success();
}

// cancel button handler
void cancel()
{
    // reset set_pos to position in EEPROM
    EEPROM.get(0, G::set_pos[0]);
    EEPROM.get(4, G::set_pos[1]);

    G::redraw_set_vol = true;
}
