
#include "headers/filler.h"
#include "headers/global.h"

uint8_t Filler::instance_count = 0;

Filler::Filler(Pins pins)
    : pins(pins), instance_id(instance_count)
{
    pinMode(pins.LOAD_SOLENOID, OUTPUT);
    pinMode(pins.UNLOAD_SOLENOID, OUTPUT);
    pinMode(pins.FILL_LIMIT_FULL, INPUT_PULLUP);
    pinMode(pins.FILL_LIMIT_EMPTY, INPUT_PULLUP);

    instance_count++;
}

void Filler::handle_interrupt()
{
    // if peddal is pressed call fill_cycle for both fillers
    // if either filler_0 pins are pressed call fill_cycle for filler_0
    // if either filler_1 pins are pressed call fill_cycle for filler_1

    if (digitalRead(G::filler_0.pins.FILL_LIMIT_FULL) ||
        digitalRead(G::filler_0.pins.FILL_LIMIT_EMPTY))
        G::filler_0.fill_cycle();
    else if (digitalRead(G::filler_1.pins.FILL_LIMIT_FULL) ||
             digitalRead(G::filler_1.pins.FILL_LIMIT_EMPTY))
        G::filler_1.fill_cycle();
    else
    {
        G::filler_0.fill_cycle();
        G::filler_1.fill_cycle();
    }
}

void Filler::write(bool load_state, bool Unload_state)
{
    digitalWrite(pins.LOAD_SOLENOID, load_state);
    digitalWrite(pins.UNLOAD_SOLENOID, Unload_state);
}

void Filler::fill_cycle()
{
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
     * FILL_LIMIT_EMPTY    |    FILL_LIMIT_FULL    |    FILLING/READY    |    action
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

    if (G::current_action[instance_id] != FILLING && G::current_action[instance_id] != READY)
        return;

    bool fl_empty = digitalRead(pins.FILL_LIMIT_EMPTY);
    bool fl_full = digitalRead(pins.FILL_LIMIT_FULL);

    if (fl_empty)
    {
        // refill if at empty position
        G::current_action[instance_id] = FILLING;
        write(HIGH, LOW);
    }
    else
    {
        if (G::current_action[instance_id] == READY)
        {
            // empty at start of cycle (pedal pressed)
            G::current_action[instance_id] = FILLING;
            write(HIGH, LOW);
        }
        else if (fl_full)
        {
            // stop at end of cycle (full limit reached)
            G::current_action[instance_id] = READY;
            write(LOW, LOW);
        }
    }
}

void Filler::error()
{
    // If state is FILLING for more than FILL_TIMING_MAX, then set to READY
    static uint32_t fill_start_time = 0;
    if (G::current_action[instance_id] == FILLING)
    {
        if (fill_start_time == 0)
            fill_start_time = millis();
        else if (millis() - fill_start_time > FILL_TIME_MAX)
        {
            G::current_action[instance_id] = READY;
            digitalWrite(pins.LOAD_SOLENOID, 0);
            digitalWrite(pins.UNLOAD_SOLENOID, 0);
            fill_start_time = 0;
        }
    }
    else
        fill_start_time = 0;
}