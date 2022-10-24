#include "headers/stepper.h"
#include "headers/global.h"
#include "headers/draw.h"

// instance count
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

    // set the instance id 0 for the first stepper, 1 for the second
    instance_id = instance_count;
    instance_count++;
}

void Stepper::home()
{
    G::current_action[instance_id] = Actions::HOMING;
    Draw::action();
    // home the stepper
    // digitalWrite(DIR_PIN, BACKWARD);

    // goto 0_pos
    // while (digitalRead(LOW_LIMIT_SWITCH_PIN) == LOW)
    // {
    //     digitalWrite(STEP_PIN, HIGH);
    //     delayMicroseconds(1000);
    //     digitalWrite(STEP_PIN, LOW);
    //     delayMicroseconds(1000);
    // }
    delay(10000);

    // set current_action to IDLE
    G::current_action[instance_id] = Actions::IDLE;
    Draw::action();
}

void Stepper::update()
{
    // clamp set_pos to 0 and MAX_POS
    if (set_pos > MAX_POS)
        set_pos = MAX_POS;
    else if (set_pos < 0)
        set_pos = 0;

    // use micros() to create a delay between steps
    if (micros() - last_step_time > 500 && set_pos != current_pos)
    {
        G::current_action[instance_id] = MOVING;

        bool dir = set_pos > current_pos ? FORWARD : BACKWARD;

        // if limit switch is pressed, don't step in that direction
        // TODO: set error flag
        if (dir == FORWARD && digitalRead(HIGH_LIMIT_SWITCH_PIN) == HIGH)
            return;
        if (dir == BACKWARD && digitalRead(LOW_LIMIT_SWITCH_PIN) == HIGH)
            return;

        // set direction
        digitalWrite(DIR_PIN, dir);

        // step once
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEP_PIN, LOW);
        current_pos += dir == FORWARD ? 1 : -1;

        // redraw the volume indicator every 200th step (to reduce drawing time)
        G::redraw_set_vol = !(current_pos % 200) || current_pos == set_pos;

        last_step_time = micros();
    }
    else if (set_pos == current_pos)
        G::current_action[instance_id] = IDLE;
}
