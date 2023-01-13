#include "headers/touch_handles.h"

// apply button handler (save to eeprom)
void apply(Touch::Args args)
{
    // save volume to EEPROM
    EEPROM.put(0, args.vis_set_pos[0]);
    EEPROM.put(2, args.vis_set_pos[1]);

    Draw::apply_success();
}

// cancel button handler
void cancel(Touch::Args args)
{
    // reset vis_set_pos
    args.vis_set_pos[0] = eeprom_read<uint16_t>(0);
    args.vis_set_pos[1] = eeprom_read<uint16_t>(2);
}

// lock button handler
void lock(Touch::Args args)
{
    // cooldown
    static uint32_t last_call = 0;
    if (millis() - last_call < 500)
        return;
    last_call = millis();

    args.pos_unlock = !args.pos_unlock;
}

void ack(Touch::Args args)
{
    // cooldown
    static uint32_t last_call = 0;
    if (millis() - last_call < 500)
        return;
    last_call = millis();

    // clear buttons
    Draw::clear_error();

    // reset status
    args.clear_error = true;
}