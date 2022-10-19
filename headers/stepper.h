#ifndef STEPPER_H
#define STEPPER_H

class Stepper
{
private:
    const int STEP_PIN;
    const int DIR_PIN;
    const int EN_PIN;
    const int LOW_LIMIT_SWITCH_PIN;
    const int HIGH_LIMIT_SWITCH_PIN;
    const int EEPROM_ADDRESS;

    static int instance_count;

    void step(int dir, int steps);

public:
    Stepper(int STEP_PIN, int DIR_PIN, int EN_PIN, int LOW_LIMIT_SWITCH_PIN, int HIGH_LIMIT_SWITCH_PIN);
    long homming();
    long goto_pos(long set_pos, long current_pos);
};

#endif