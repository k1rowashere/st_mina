#include "headers/constants.h"
#include "headers/draw.h"

namespace Handlers
{
    // increment/decrement buttons
    // had to make template because of the way the compiler handles constexpr
    template <uint32_t *value, Sign sign>
    void inc_dec(uint16_t hold_counter)
    {
        if (hold_counter > 20)
            *value += sign * volume_to_steps(40);
        else if (hold_counter > 10)
            *value += sign * volume_to_steps(10);
        else
            *value += sign * volume_to_steps(1);

        // clamp value 0-MAX_POS
        if (*value > MAX_POS)
            *value = MAX_POS;
        else if (*value < 0)
            *value = 0;

        hold_counter++;
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
}
