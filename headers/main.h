#pragma once

// #define STEP_PIN 22              // Step pin
// #define DIR_PIN 24               // Direction pin
// #define EN_PIN 26                // Enable pin
// #define LOW_LIMIT_SWITCH_PIN 28  // Limit switch pin
// #define HIGH_LIMIT_SWITCH_PIN 30 // Limit switch pin

// convert defines to constexpr
constexpr int STEPS_PER_REV = 200;  // Number of steps per revolution
constexpr int CYLINDER_RADIUS = 36; // Radius of the cylinder in mm
constexpr float THREAD_PITCH = 1.5; // Thread pitch in mm
constexpr int HEIGHT = 300;         // Height of the cylinder in mm

constexpr long MAX_POS = (long)STEPS_PER_REV * HEIGHT / THREAD_PITCH; // Max number of steps to limit switch from 0_pos

#define SCREEN_WIDTH 480  // tft width
#define SCREEN_HEIGHT 320 // tft height

// touch screen
#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 110
#define TS_MAXX 913
#define TS_MINY 90
#define TS_MAXY 950

enum Actions
{
    IDLE,
    HOMMING,
    MOVING,
    LIMIT_SWITCH
};

enum Sign
{
    DECREMENT = -1,
    INCREMENT = 1
};