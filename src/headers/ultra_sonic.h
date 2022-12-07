#pragma once

#include "utils.h"

#define SPEED_OF_SOUND 343
#define INVALID_MAX 2000
#define ULTASONIC_POLLING_INTERVAL 500

class Ultrasonic
{
private:
    uint8_t TRIG_PIN;
    uint8_t ECHO_PIN;
    Queue<uint16_t, 20> queue;

    uint16_t get_distance();

public:
    Ultrasonic(uint8_t trig_pin, uint8_t echo_pin);
    uint16_t get_avg_dist();
};
