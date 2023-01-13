#include "headers/touch_handles.h"

// apply button handler (save to eeprom)
void TH::apply(const Touch::Args& args)
{
    // save volume to EEPROM
    EEPROM.put(0, args.vis_set_pos[0]);
    EEPROM.put(2, args.vis_set_pos[1]);

    Draw::btn_applied();
}

// cancel button handler
void TH::cancel(const Touch::Args& args)
{
    // reset vis_set_pos
    args.vis_set_pos[0] = eeprom_read<uint16_t>(0);
    args.vis_set_pos[1] = eeprom_read<uint16_t>(2);
}

// lock button handler
void TH::lock(const Touch::Args& args)
{
    // cooldown
    static uint32_t last_call = 0;
    if (millis() - last_call < 500)
        return;
    last_call = millis();

    args.pos_unlock = !args.pos_unlock;
}

// acknoledge error button handler
void TH::ack(const Touch::Args& args)
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

// empty cylinder button handler
void TH::empty(const Touch::Args& args)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        if (args.current_status[i] != Status::READY)
            continue;

        args.current_status[i] = Status::CLEANING;
        args.fillers[i].empty();
    }
}

// fill cylinder button handler
void TH::fill(const Touch::Args& args)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        if (args.current_status[i] != Status::READY && args.current_status[i] != Status::CLEANING)
            continue;

        args.current_status[i] = args.fillers[i].fill();
    }
}