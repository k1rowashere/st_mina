#include "constants.h"
#include "draw.h"
#include "touch.h"
#include <EEPROM.h>

// +/- buttons handlers, must be in header file
template <const uint8_t i, const int8_t sign>
void inc_dec(Touch::Args args)
{
    static uint16_t hold_counter(0);
    static uint32_t last_call_ms(0);

    auto vis_set_pos = args.vis_set_pos;
    auto current_status = args.current_status;

    if (millis() - last_call_ms > 1000)
        hold_counter = 0;
    if ((millis() - last_call_ms) > 200)
    {
        if (hold_counter > 20)
            vis_set_pos[i] += sign * volume_to_steps(50);
        else if (hold_counter > 10)
            vis_set_pos[i] += sign * volume_to_steps(10);
        else
            vis_set_pos[i] += sign * volume_to_steps(1);

        // Clamp value workaround, overflow prevention
        if (vis_set_pos[i] > UINT16_MAX - 70)
            vis_set_pos[i] = 0;
        else if (vis_set_pos[i] > MAX_POS)
            vis_set_pos[i] = MAX_POS;

        hold_counter++;
        last_call_ms = millis();
    }
}

void apply(Touch::Args args);
void cancel(Touch::Args args);
void lock(Touch::Args args);
void ack(Touch::Args args);