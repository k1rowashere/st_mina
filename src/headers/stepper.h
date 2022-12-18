#pragma once
#include "constants.h"
#include "utils.h"

class Stepper
{
public:
    uint16_t set_pos = 0;
    uint16_t actual_pos = 0;

    struct Pins
    {
        uint8_t STEP;
        uint8_t DIR;
        uint8_t EN;
        uint8_t LOW_LIMIT_SWITCH;
        uint8_t HIGH_LIMIT_SWITCH;
    };

    Stepper(Pins pins);
    // void init();

    // returns the current status if the stepper is ready
    Status update(Status curr_status);

private:
    const Pins pins;
    uint64_t last_step_time = 0;
};
