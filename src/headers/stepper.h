#pragma once
#include <EEPROM.h>
#include <Arduino.h>
#include "constants.h"

class Stepper
{
public:
    uint32_t set_pos = 0;
    uint32_t current_pos = 0;

    Stepper(uint8_t STEP_PIN, uint8_t DIR_PIN, uint8_t EN_PIN, uint8_t LOW_LIMIT_SWITCH_PIN, uint8_t HIGH_LIMIT_SWITCH_PIN);
    uint32_t update();
    void home();

private:
    const uint8_t STEP_PIN;
    const uint8_t DIR_PIN;
    const uint8_t EN_PIN;
    const uint8_t LOW_LIMIT_SWITCH_PIN;
    const uint8_t HIGH_LIMIT_SWITCH_PIN;

    void step(Direction dir);
};
