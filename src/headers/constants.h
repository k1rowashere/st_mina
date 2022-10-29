#pragma once

#define DEBUG
// #define DISABLE_SWITCHES

#include <Arduino.h>

constexpr uint16_t STEPS_PER_REV = 200;  // Number of steps per revolution
constexpr uint16_t CYLINDER_RADIUS = 36; // Radius of the cylinder in mm
constexpr float THREAD_PITCH = 1.5;      // Thread pitch in mm
constexpr uint16_t HEIGHT = 300;         // Height of the cylinder in mm

constexpr int32_t MAX_POS = (int32_t)STEPS_PER_REV * HEIGHT / THREAD_PITCH; // Max number of steps to limit switch from 0_pos

#define SCREEN_WIDTH 480  // tft width
#define SCREEN_HEIGHT 320 // tft height

// pins
#define STEP_PIN_0 22
#define DIR_PIN_0 24
#define EN_PIN_0 26
#define LOW_LIMIT_SWITCH_PIN_0 28
#define HIGH_LIMIT_SWITCH_PIN_0 30

#define STEP_PIN_1 23
#define DIR_PIN_1 25
#define EN_PIN_1 27
#define LOW_LIMIT_SWITCH_PIN_1 29
#define HIGH_LIMIT_SWITCH_PIN_1 31

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
#define SELECTOR_PIN_0 43
#define SELECTOR_PIN_1 45

// filler
#define FILL_TIME_MAX 5000 // max time to fill in ms

// touch screen
// pins
#define YP A3
#define XM A2
#define YM 9
#define XP 8
// calibration
#define TS_MINX 110
#define TS_MAXX 913
#define TS_MINY 90
#define TS_MAXY 950

#define MINPRESSURE 10
#define MAXPRESSURE 1000

enum Actions
{
    READY,
    HOMING,
    MOVING,
    FILLING,
    EMPTYING,
    STOPPED
};

enum Sign
{
    NEGATIVE = -1,
    POSITIVE = 1
};

// TODO: check if this is the correct direction
enum Direction
{
    FORWARD,
    BACKWARD
};