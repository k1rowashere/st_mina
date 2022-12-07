#include "headers/ultra_sonic.h"

Ultrasonic::Ultrasonic(uint8_t trig_pin, uint8_t echo_pin)
    : TRIG_PIN(trig_pin), ECHO_PIN(echo_pin), queue()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW);
}

uint16_t Ultrasonic::get_distance()
{
    // send a 20us trigger pulse to the sensor
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG_PIN, LOW);

    // read the echo pulse width
    uint16_t distance_mm = pulseIn(ECHO_PIN, HIGH) * SPEED_OF_SOUND / 1000 / 2;

    return distance_mm;
}

uint16_t Ultrasonic::get_avg_dist()
{
    static uint32_t sum = 0;
    static uint64_t last = millis();

    if (millis() - last < ULTASONIC_POLLING_INTERVAL)
        return sum / queue.size();
    last = millis();

    // read the echo pulse width
    uint16_t distance_mm = get_distance();

    // ignore too large values
    if (distance_mm < INVALID_MAX)
    {
        // if the queue is full, remove the oldest reading from the sum
        if (queue.is_full())
            sum -= queue.pop();

        // add the new reading to the queue and the sum
        sum += queue.push(distance_mm);
    }

    // return the average of last 10 readings
    return sum / queue.size();
}