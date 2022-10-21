#include "headers/stepper.h"

uint8_t Stepper::instance_count = 0;

Stepper::Stepper(uint8_t STEP_PIN, uint8_t DIR_PIN, uint8_t EN_PIN, uint8_t LOW_LIMIT_SWITCH_PIN, uint8_t HIGH_LIMIT_SWITCH_PIN)
    : STEP_PIN(STEP_PIN),
      DIR_PIN(DIR_PIN),
      EN_PIN(EN_PIN),
      LOW_LIMIT_SWITCH_PIN(LOW_LIMIT_SWITCH_PIN),
      HIGH_LIMIT_SWITCH_PIN(HIGH_LIMIT_SWITCH_PIN)
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(LOW_LIMIT_SWITCH_PIN, INPUT);
    pinMode(HIGH_LIMIT_SWITCH_PIN, INPUT);

    // home the stepper
    digitalWrite(DIR_PIN, BACKWARD);

    // goto 0_pos
    // while (digitalRead(LOW_LIMIT_SWITCH_PIN) == LOW)
    // {
    //     digitalWrite(STEP_PIN, HIGH);
    //     delayMicroseconds(1000);
    //     digitalWrite(STEP_PIN, LOW);
    //     delayMicroseconds(1000);
    // }

    // start EEPROM address at 0x00 and increment by sizeof long for each instance
    const uint16_t EEPROM_ADDRESS = 0x00 + instance_count * sizeof(uint32_t);
    // read steps from eeprom (4 bytes)
    uint32_t steps = EEPROM.get(EEPROM_ADDRESS, steps);
    // check if steps is not set (default is 0xFFFF)
    if (steps == 0xFFFF)
    {
        // write zeros to eeprom
        EEPROM.put(EEPROM_ADDRESS, 0L);

        steps = 0;
    }

    set_pos = steps;

    // increment instance count for next stepper
    instance_count++;
}

void Stepper::step(Direction dir)
{
    // set direction
    digitalWrite(DIR_PIN, dir);
    // step once
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
}

uint32_t Stepper::update()
{
    // use micros() to create a delay between steps
    static uint64_t last_step_time = 0;
    if (micros() - last_step_time > 1000)
    {
        // check if set_pos is greater than current_pos
        if (set_pos > current_pos)
            step(FORWARD);
        else
            step(BACKWARD);
    }
    last_step_time = micros();
    return current_pos;
}