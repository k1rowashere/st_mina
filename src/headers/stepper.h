#pragma once
#include <EEPROM.h>
#include <Arduino.h>
#include "constants.h"
#include "helpers.h"

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
    void update();
    void fill_cycle();

private:
    const Pins pins;
    uint64_t last_step_time = 0;

    const uint8_t instance_id;
    static uint8_t instance_count;
    bool first_after_homing = false;
};
