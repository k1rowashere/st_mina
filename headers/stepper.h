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
    void step(int dir, int steps);

public:
    Stepper(int STEP_PIN, int DIR_PIN, int EN_PIN, int LOW_LIMIT_SWITCH_PIN, int HIGH_LIMIT_SWITCH_PIN);
    void homming();
    int goto_pos(int set_pos, int current_pos);
};

#endif