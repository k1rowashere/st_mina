#include "headers/stepper.h"

Stepper::Stepper(Pins pins)
    : pins(pins)
{
    // pin setup
    pinMode(pins.STEP, OUTPUT);
    pinMode(pins.DIR, OUTPUT);
    pinMode(pins.EN, OUTPUT);
    pinMode(pins.LOW_LIMIT_SWITCH, INPUT_PULLUP);
    pinMode(pins.HIGH_LIMIT_SWITCH, INPUT_PULLUP);
}

Status Stepper::update(Status curr_status)
{
    // only update if 500us have passed since last update
    if (micros() - last_step_time < 500)
        return curr_status;

    // TODO: error handling

    // clamp set_pos to 0 and MAX_POS
    set_pos = constrain((int32_t)set_pos, 0, MAX_POS);

    // home the stepper on startups
    switch (curr_status)
    {
    case Status::HOMING:
    {
        if (digitalRead(pins.LOW_LIMIT_SWITCH) == LOW)
        {
            digitalWrite(pins.DIR, BACKWARD);
            digitalWrite(pins.STEP, HIGH);
            delayMicroseconds(500);
            digitalWrite(pins.STEP, LOW);
            return Status::HOMING;
        }

        return Status::MOVING;
    }

    case Status::READY:
    case Status::MOVING:
    {
        // if-guard: return true if set_pos achieved (aka READY)
        if (set_pos == actual_pos)
            return Status::DONE;

        bool dir = set_pos > actual_pos ? FORWARD : BACKWARD;

        // set direction
        digitalWrite(pins.DIR, dir);

        // step once
        digitalWrite(pins.STEP, HIGH);
        delayMicroseconds(500);
        digitalWrite(pins.STEP, LOW);
        actual_pos += dir == FORWARD ? 1 : -1;

        last_step_time = micros();

        return Status::MOVING;
    }

    default:
        return curr_status;
    }
}

// void error()
// {
//     // if limit switch is pressed, don't step in that direction
//     // TODO: set error flag
//     if (dir == FORWARD && digitalRead(pins.HIGH_LIMIT_SWITCH) == HIGH)
//         return;
//     if (dir == BACKWARD && digitalRead(pins.LOW_LIMIT_SWITCH) == HIGH)
//         return;
// }