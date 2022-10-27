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
    void fill();
    void empty();
    void stop();
    void update();

private:
    void error();
    void fill_cycle();
    const uint8_t instance_id;
    static uint8_t instance_count;
};