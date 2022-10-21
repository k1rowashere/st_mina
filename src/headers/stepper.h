#pragma once
#include <EEPROM.h>
#include <Arduino.h>

// TODO: check if this is the correct direction
enum Direction
{
    FORWARD,
    BACKWARD
};

class Stepper
{
public:
    Stepper(uint8_t STEP_PIN, uint8_t DIR_PIN, uint8_t EN_PIN, uint8_t LOW_LIMIT_SWITCH_PIN, uint8_t HIGH_LIMIT_SWITCH_PIN);
    uint32_t set_pos = 0;
    uint32_t update();

private:
    const uint8_t STEP_PIN;
    const uint8_t DIR_PIN;
    const uint8_t EN_PIN;
    const uint8_t LOW_LIMIT_SWITCH_PIN;
    const uint8_t HIGH_LIMIT_SWITCH_PIN;

    uint32_t current_pos = 0;
    static uint8_t instance_count;

    void step(Direction dir);
};
