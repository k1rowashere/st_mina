#pragma once
#include <EEPROM.h>
#include <Arduino.h>
#include "constants.h"
#include "helpers.h"

class Stepper
{
public:
    uint32_t set_pos = 0;
    uint32_t current_pos = 0;

    Stepper(uint8_t STEP_PIN, uint8_t DIR_PIN, uint8_t EN_PIN, uint8_t LOW_LIMIT_SWITCH_PIN, uint8_t HIGH_LIMIT_SWITCH_PIN);
    void update();
    void home();

private:
    const uint8_t STEP_PIN;
    const uint8_t DIR_PIN;
    const uint8_t EN_PIN;
    const uint8_t LOW_LIMIT_SWITCH_PIN;
    const uint8_t HIGH_LIMIT_SWITCH_PIN;
    uint64_t last_step_time = 0;

    static uint8_t instance_count;
    uint8_t instance_id;
};
