#pragma once
#include <Arduino.h>

constexpr uint16_t STEPS_PER_REV = 200;  // Number of steps per revolution
constexpr uint16_t CYLINDER_RADIUS = 36; // Radius of the cylinder in mm
constexpr float THREAD_PITCH = 1.5;      // Thread pitch in mm
constexpr uint16_t HEIGHT = 300;         // Height of the cylinder in mm
constexpr int32_t MAX_POS = 39297;       // max position of stepper, = 1200 ml, determined experimentally

#define SCREEN_WIDTH 480  // tft width
#define SCREEN_HEIGHT 320 // tft height

#define FILL_TIME_MAX 5000 // max time to fill in ms

#define MAX_DISTANCE 1000 // minium level (max distance) ultrasonic can detect

// pins
#define STEP_PIN_0 22
#define DIR_PIN_0 24
#define EN_PIN_0 26
#define HIGH_LIMIT_SWITCH_PIN_0 28
#define LOW_LIMIT_SWITCH_PIN_0 30

#define STEP_PIN_1 23
#define DIR_PIN_1 25
#define EN_PIN_1 27
#define HIGH_LIMIT_SWITCH_PIN_1 29
#define LOW_LIMIT_SWITCH_PIN_1 31

#define LOAD_SOLENOID_0 32
#define UNLOAD_SOLENOID_0 34
#define FILL_LIMIT_FULL_0 36
#define FILL_LIMIT_EMPTY_0 38

#define LOAD_SOLENOID_1 33
#define UNLOAD_SOLENOID_1 35
#define FILL_LIMIT_FULL_1 37
#define FILL_LIMIT_EMPTY_1 39

#define FILL_PEDAL_PIN 40
#define ON_OFF_PIN 41
#define SELECTOR_PIN_0 42
#define SELECTOR_PIN_1 43

#define LED_R 44
#define LED_G 45
#define LED_B 46

enum Status
{
    READY,
    HOMING,
    MOVING,
    FILLING,
    EMPTYING,
    CLEANING,
    STOPPED,
    ERROR
};

// specifies the direction of the stepper
enum Direction
{
    FORWARD,
    BACKWARD,
};