#include <EEPROM.h>

#define STEP_PIN 22              // Step pin
#define DIR_PIN 24               // Direction pin
#define EN_PIN 26                // Enable pin
#define LOW_LIMIT_SWITCH_PIN 28  // Limit switch pin
#define HIGH_LIMIT_SWITCH_PIN 30 // Limit switch pin

#define STEPS_PER_REV 200  // Number of steps per revolution
#define CYLINDER_RADIUS 10 // Radius of the cylinder in mm
#define THREAD_PITCH 1.25  // Thread pitch in mm
#define HEIGHT 100         // Height of the cylinder in mm
// #define STEPS_LIMIT (HEIGHT * STEPS_PER_REV / THREAD_PITCH) // Number of steps to limit switch from 0_pos

unsigned int current_pos = 0; // Current position in steps

void setup()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(13, OUTPUT);
}

void step(int dir, int steps)
{
    // HIGH = clockwise, LOW = counter-clockwise
    digitalWrite(DIR_PIN, dir);
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
}

// Convert Volume (ml) of product to number of steps from 0-pos
int volume_to_steps(int volume)
{
    // convert volume to mm^3
    volume *= 1000;

    // get height required to fill volume of the cylinder
    float height = volume / (PI * CYLINDER_RADIUS * CYLINDER_RADIUS);

    // calculate number of steps required to fill cylinder given the thread pitch
    return height * STEPS_PER_REV / THREAD_PITCH;
}

// Convert number of steps from 0-pos to Volume (ml) of product
int steps_to_volume(int steps)
{
    // calculate height of cylinder given the number of steps
    float height = steps * THREAD_PITCH / STEPS_PER_REV;

    // calculate volume of cylinder given the height
    float volume = PI * CYLINDER_RADIUS * CYLINDER_RADIUS * height;

    // convert volume to ml
    return volume / 1000;
}

// move to 0_pos then go to set_pos
void homming()
{
    // move in the opposite direction of the thread
    digitalWrite(DIR_PIN, LOW);
    while (digitalRead(LOW_LIMIT_SWITCH_PIN) == LOW)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
    // read steps from eeprom 2 bytes
    unsigned int steps = EEPROM.read(0) + (EEPROM.read(1) << 8);
    // check if steps is set (default is 0xFFFF)
    if (steps != 0xFFFF)
    {
        step(HIGH, steps);
    }
    else
    {
        // write 0 to eeprom
        EEPROM.write(0, 0);
        EEPROM.write(1, 0);
    }
}

void goto_pos(int set_pos)
{
    // check if set_pos is greater than current_pos
    if (set_pos > current_pos)
        step(HIGH, set_pos - current_pos);
    else
        step(LOW, current_pos - set_pos);

    // update current_pos
    current_pos = set_pos;
}

void loop()
{
    // put your main code here, to run repeatedly:
}