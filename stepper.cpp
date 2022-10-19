#include <EEPROM.h>
#include <Arduino.h>

#include "headers/stepper.h"

int Stepper::instance_count = 0;

Stepper::Stepper(int STEP_PIN, int DIR_PIN, int EN_PIN, int LOW_LIMIT_SWITCH_PIN, int HIGH_LIMIT_SWITCH_PIN)
    : STEP_PIN(STEP_PIN),
      DIR_PIN(DIR_PIN),
      EN_PIN(EN_PIN),
      LOW_LIMIT_SWITCH_PIN(LOW_LIMIT_SWITCH_PIN),
      HIGH_LIMIT_SWITCH_PIN(HIGH_LIMIT_SWITCH_PIN),
      // start EEPROM address at 0x00 and increment by 4 bytes for each instance
      EEPROM_ADDRESS(0x00 + instance_count * 4)
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(LOW_LIMIT_SWITCH_PIN, INPUT);
    pinMode(HIGH_LIMIT_SWITCH_PIN, INPUT);

    // increment instance count for next stepper
    instance_count++;
}

void Stepper::step(int dir, int steps)
{
    // HIGH = clockwise, LOW = counter-clockwise
    digitalWrite(DIR_PIN, dir);
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
}

long Stepper::goto_pos(long set_pos, long current_pos)
{
    // check if set_pos is greater than current_pos
    if (set_pos > current_pos)
        step(HIGH, set_pos - current_pos);
    else
        step(LOW, current_pos - set_pos);

    // update current_pos
    return set_pos;
}

long Stepper::homming()
{
    // move to 0_pos then go to set_pos (if set_pos is set)

    // move in the opposite direction of the thread
    digitalWrite(DIR_PIN, LOW);

    // while (digitalRead(LOW_LIMIT_SWITCH_PIN) == LOW)
    // {
    //     digitalWrite(STEP_PIN, HIGH);
    //     delayMicroseconds(1000);
    //     digitalWrite(STEP_PIN, LOW);
    //     delayMicroseconds(1000);
    // }

    // read steps from eeprom (4 bytes)
    long steps = EEPROM.get(EEPROM_ADDRESS, steps);

    // check if steps is set (default is 0xFFFF)
    if (steps != 0xFFFF)
    {
        step(HIGH, steps);
    }
    else
    {
        // write zeros to eeprom
        EEPROM.put(EEPROM_ADDRESS, 0L);

        steps = 0;
    }

    return goto_pos(steps, 0);
}
