#include "headers/touch_handles.h"

// +/- buttons macro
#define INC_DEC(pos, sign)                                         \
    void inc_dec_##pos##_##sign()                                  \
    {                                                              \
        static uint16_t hold_counter(0);                           \
        static uint32_t last_call_ms(0);                           \
                                                                   \
        if (millis() - last_call_ms > 1000)                        \
            hold_counter = 0;                                      \
        if ((millis() - last_call_ms) > 200)                       \
        {                                                          \
            if (hold_counter > 20)                                 \
                G::vis_set_pos[pos] += sign * volume_to_steps(50); \
            else if (hold_counter > 10)                            \
                G::vis_set_pos[pos] += sign * volume_to_steps(10); \
            else                                                   \
                G::vis_set_pos[pos] += sign * volume_to_steps(1);  \
                                                                   \
            /* Clamp value workaround, overflow prevention */      \
            if (G::vis_set_pos[pos] > UINT16_MAX - 70)             \
                G::vis_set_pos[pos] = 0;                           \
            else if (G::vis_set_pos[pos] > MAX_POS)                \
                G::vis_set_pos[pos] = MAX_POS;                     \
                                                                   \
            hold_counter++;                                        \
            G::redraw_set_vol = true;                              \
            last_call_ms = millis();                               \
        }                                                          \
    }

// define +/- buttons handlers
INC_DEC(0, NEGATIVE)
INC_DEC(0, POSITIVE)
INC_DEC(1, NEGATIVE)
INC_DEC(1, POSITIVE)

// apply button handler (save to eeprom)
void apply()
{
    G::stepper_0.set_pos = G::vis_set_pos[0];
    G::stepper_1.set_pos = G::vis_set_pos[1];

    // save volume to EEPROM
    EEPROM.put(0, G::vis_set_pos[0]);
    EEPROM.put(4, G::vis_set_pos[1]);
    Draw::apply_success();
}

// cancel button handler
void cancel()
{
    // reset set_pos to position in EEPROM
    G::vis_set_pos[0] = G::stepper_0.set_pos;
    G::vis_set_pos[1] = G::stepper_1.set_pos;

    G::redraw_set_vol = true;
}

// lock button handler
void lock()
{
    static uint32_t last_call = 0;
    // cooldown
    if (millis() - last_call < 500)
        return;
    G::pos_unlock = !G::pos_unlock;
    last_call = millis();
}
