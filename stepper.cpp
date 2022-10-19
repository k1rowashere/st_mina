#include <EEPROM.h>
#include <Arduino.h>

#include "headers/stepper.h"

// constructor
Stepper::Stepper(int STEP_PIN, int DIR_PIN, int EN_PIN, int LOW_LIMIT_SWITCH_PIN, int HIGH_LIMIT_SWITCH_PIN)
    : STEP_PIN(STEP_PIN), DIR_PIN(DIR_PIN), EN_PIN(EN_PIN), LOW_LIMIT_SWITCH_PIN(LOW_LIMIT_SWITCH_PIN), HIGH_LIMIT_SWITCH_PIN(HIGH_LIMIT_SWITCH_PIN)
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(LOW_LIMIT_SWITCH_PIN, INPUT);
    pinMode(HIGH_LIMIT_SWITCH_PIN, INPUT);
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

// move to 0_pos then go to set_pos
void Stepper::homming()
{
    // move in the opposite direction of the thread
    digitalWrite(DIR_PIN, LOW);
    while (digitalRead(LOW_LIMIT_SWITCH_PIN) == LOW)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
    // read steps from eeprom 2 bytes
    unsigned int steps = EEPROM.read(0) + (EEPROM.read(1) << 8);
    // check if steps is set (default is 0xFFFF)
    if (steps != 0xFFFF)
    {
        step(HIGH, steps);
    }
    else
    {
        // write 0 to eeprom
        EEPROM.write(0, 0);
        EEPROM.write(1, 0);
    }
}

int Stepper::goto_pos(int set_pos, int current_pos)
{
    // check if set_pos is greater than current_pos
    if (set_pos > current_pos)
        step(HIGH, set_pos - current_pos);
    else
        step(LOW, current_pos - set_pos);

    // update current_pos
    return set_pos;
}
