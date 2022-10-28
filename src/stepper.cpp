#include "headers/stepper.h"
#include "headers/global.h"
#include "headers/draw.h"

// instance count
uint8_t Stepper::instance_count = 0;

Stepper::Stepper(Pins pins)
    : pins(pins), instance_id(instance_count++)
{
    // pin setup
    pinMode(pins.STEP, OUTPUT);
    pinMode(pins.DIR, OUTPUT);
    pinMode(pins.EN, OUTPUT);
    pinMode(pins.LOW_LIMIT_SWITCH, INPUT_PULLUP);
    pinMode(pins.HIGH_LIMIT_SWITCH, INPUT_PULLUP);
}

void Stepper::update()
{
    // TODO: error handling

    // use micros() to create a delay between steps
    if (micros() - last_step_time < 500)
        return;

    // clamp set_pos to 0 and MAX_POS
    set_pos = constrain(set_pos, 0, MAX_POS);

    // home the stepper on startup
    if (G::current_action[instance_id] == Actions::HOMING)
    {
        if (digitalRead(pins.LOW_LIMIT_SWITCH) == LOW)
        {
            digitalWrite(pins.DIR, BACKWARD);
            digitalWrite(pins.STEP, HIGH);
            delayMicroseconds(500);
            digitalWrite(pins.STEP, LOW);
            return;
        }

        G::current_action[instance_id] = Actions::READY;
        first_after_homing = true;
    }
    if (G::current_action[instance_id] == Actions::READY ||
        G::current_action[instance_id] == Actions::MOVING)
    {
        // if-guard: READY if set_pos achieved
        if (set_pos == current_pos)
        {
            G::current_action[instance_id] = Actions::READY;
            if (first_after_homing)
            {
                first_after_homing = false;
                if (instance_id == 0)
                    G::filler_0.fill_cycle();
                else if (instance_id == 1)
                    G::filler_1.fill_cycle();
            }
            return;
        }

        G::current_action[instance_id] = MOVING;

        bool dir = set_pos > current_pos ? FORWARD : BACKWARD;

        // set direction
        digitalWrite(pins.DIR, dir);

        // step once
        digitalWrite(pins.STEP, HIGH);
        delayMicroseconds(500);
        digitalWrite(pins.STEP, LOW);
        current_pos += dir == FORWARD ? 1 : -1;

        // redraw the volume indicator every 200th step (to reduce drawing time)
        G::redraw_set_vol = !(current_pos % 200) || current_pos == set_pos;

        last_step_time = micros();
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