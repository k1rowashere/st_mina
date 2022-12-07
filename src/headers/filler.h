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
        uint8_t PEDAL;
    };

    const Pins pins;

    Filler(Pins pins);
    Status fill();
    Status empty();
    void stop();
    Status update(Status status);
    Status fill_cycle(Status status);

private:
    void error();

    uint8_t prev_state = 0;

    const uint8_t instance_id;
    static uint8_t instance_count;
};