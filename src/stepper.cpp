#include "headers/stepper.h"

Stepper::Stepper(Pins pins)
    : pins(pins)
{
    pinMode(pins.STEP, OUTPUT);
    pinMode(pins.DIR, OUTPUT);
    pinMode(pins.EN, OUTPUT);
    pinMode(pins.LOW_LIMIT_SWITCH, INPUT_PULLUP);
    pinMode(pins.HIGH_LIMIT_SWITCH, INPUT_PULLUP);

    digitalWrite(pins.EN, HIGH);    // disable stepper
}

Status Stepper::update(Status curr_status)
{
    // only update if 500us have passed since last update
    if (micros() - last_step_time < 500)
        return curr_status;


    // clamp set_pos to 0 and MAX_POS
    set_pos = constrain((int32_t)set_pos, 0, MAX_POS);

    // home the stepper on startups
    if (curr_status == Status::HOMING)
    {
        if (digitalRead(pins.LOW_LIMIT_SWITCH) == LOW) // inactive on HIGH
        {
            actual_pos = 0;
            return Status::MOVING;
        }
        // enable stepper
        digitalWrite(pins.EN, LOW);
        // set direction     
        digitalWrite(pins.DIR, BACKWARD);

        digitalWrite(pins.STEP, HIGH);
        delayMicroseconds(500);
        digitalWrite(pins.STEP, LOW);

        return Status::HOMING;
    }
    else if (curr_status == Status::READY || curr_status == Status::MOVING)
    {
        // limit switch handling
        if (digitalRead(pins.HIGH_LIMIT_SWITCH) == LOW)
            actual_pos = MAX_POS;
        if (digitalRead(pins.LOW_LIMIT_SWITCH) == LOW)
            actual_pos = 0;

        // if-guard: return DONE if set_pos achieved
        if (set_pos == actual_pos)
        {
            digitalWrite(pins.EN, HIGH);        // disable stepper
            return (curr_status == Status::MOVING) ? Status::DONE : Status::READY;
        }

        Direction dir = set_pos > actual_pos ? FORWARD : BACKWARD;

        // enable stepper
        digitalWrite(pins.EN, LOW);
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
    else
        return curr_status;
}

// void error()
// {
//     if (dir == FORWARD && digitalRead(pins.HIGH_LIMIT_SWITCH) == HIGH)
//         return;
//     if (dir == BACKWARD && digitalRead(pins.LOW_LIMIT_SWITCH) == HIGH)
//         return;
// }