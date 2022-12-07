
#include "headers/filler.h"
#include "headers/global.h"

uint8_t Filler::instance_count = 0;

Filler::Filler(Pins pins)
    : pins(pins), instance_id(instance_count++)
{
    pinMode(pins.LOAD_SOLENOID, OUTPUT);
    pinMode(pins.UNLOAD_SOLENOID, OUTPUT);
    pinMode(pins.PEDAL, INPUT_PULLUP);
    pinMode(pins.FILL_LIMIT_FULL, INPUT_PULLUP);
    pinMode(pins.FILL_LIMIT_EMPTY, INPUT_PULLUP);
}

Status Filler::empty()
{
    digitalWrite(pins.LOAD_SOLENOID, LOW);
    digitalWrite(pins.UNLOAD_SOLENOID, HIGH);
    return Status::EMPTYING;
}

Status Filler::fill()
{
    digitalWrite(pins.LOAD_SOLENOID, HIGH);
    digitalWrite(pins.UNLOAD_SOLENOID, LOW);
    return Status::FILLING;
}

void Filler::stop()
{
    digitalWrite(pins.LOAD_SOLENOID, LOW);
    digitalWrite(pins.UNLOAD_SOLENOID, LOW);
}

/*
 * Will run on interrupts: FILL_LIMIT_EMPTY (RISING), FILL_LIMIT_FULL (RISING), FILL_PEDAL (RISING)
 *
 * Only runs if at READY or FILLING state.
 *
 * Logical paths:
 * 1. If the cylinder is empty, then it will get filled (FILL_LIMIT_EMPTY).
 * 2. If the cylinder is either full or partially filled, then it will get emptied [if was called in READY state. (FILL_PEDAL/start of the cycle)]
 * 3. If the cylinder is full, then STOP [if was called in FILLING state. (FILL_LIMIT_FULL / end of the cycle)]
 * 4. Invalid states:
 *      - both FILL_LIMIT_EMPTY and FILL_LIMIT_FULL are HIGH (should never happen)
 *      - The function is called in FILLING state, but neither FILL_LIMIT_EMPTY or FILL_LIMIT_FULL are HIGH (pedal is pressed mid cycle)
 *
 * 5. If the piston is stuck mid cycle (FILLING state for too long): the state will be set to READY, after a timeout.
 *
 * FILL_LIMIT_EMPTY    |    FILL_LIMIT_FULL    |    FILLING/READY    |    status
 * --------------------|-----------------------|---------------------|-----------------------------
 * 1                   |    1                  |    READY            |    ERROR
 * 1                   |    1                  |    FILLING          |    ERROR
 * 1                   |    0                  |    READY            |    FILL + set state to FILLING
 * 1                   |    0                  |    FILLING          |    FILL + set state to FILLING
 * 0                   |    1                  |    READY            |    EMPTY + set state to FILLING
 * 0                   |    1                  |    FILLING          |    STOP + set state to READY
 * 0                   |    0                  |    READY            |    EMPTY + set state to FILLING
 * 0                   |    0                  |    FILLING          |    Do nothing
 */
Status Filler::fill_cycle(Status status)
{
    if (!(status == FILLING || status == READY || status == EMPTYING))
        return status;

    bool fl_empty = digitalRead(pins.FILL_LIMIT_EMPTY);
    bool fl_full = digitalRead(pins.FILL_LIMIT_FULL);

    if (fl_empty)
    {
        // refill if at empty position
        return fill();
    }
    else
    {
        if (status == READY && digitalRead(pins.PEDAL))
        {
            // empty at start of cycle (pedal pressed)
            return empty();
        }
        else if (fl_full && status == FILLING)
        {
            // stop at end of cycle (full limit reached)
            stop();
            return Status::READY;
        }
    }
}

// void Filler::error()
// {
//     // If state is FILLING for more than FILL_TIMING_MAX, then set to READY
//     static uint32_t fill_start_time = 0;
//     if (status == FILLING)
//     {
//         if (fill_start_time == 0)
//             fill_start_time = millis();
//         else if (millis() - fill_start_time > FILL_TIME_MAX)
//         {
//             status = READY;
//             digitalWrite(pins.LOAD_SOLENOID, 0);
//             digitalWrite(pins.UNLOAD_SOLENOID, 0);
//             fill_start_time = 0;
//         }
//     }
//     else
//         fill_start_time = 0;
// }

Status Filler::update(Status status)
{
#ifndef DEBUG
    // error handling
    error();
#endif

    // Rising edge detection
    uint8_t curr_state = 0;

    curr_state |= digitalRead(pins.PEDAL) << 1;
    curr_state |= digitalRead(pins.FILL_LIMIT_FULL) << 2;
    curr_state |= digitalRead(pins.FILL_LIMIT_EMPTY) << 3;

    if (~prev_state & curr_state)
        status = fill_cycle(status);

    prev_state = curr_state;
    return status;
}