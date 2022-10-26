#pragma once

#include "constants.h"

class Filler
{
public:
    struct Pins
    {
        uint8_t LOAD_SOLENOID;
        uint8_t UNLOAD_SOLENOID;
        uint8_t FILL_LIMIT_FULL;
        uint8_t FILL_LIMIT_EMPTY;
    };
    const Pins pins;

    Filler(Pins pins);
    void error();
    void fill_cycle();
    static void handle_interrupt();

private:
    const uint8_t instance_id;
    static uint8_t instance_count;

    void write(bool load_state, bool Unload_state);
};