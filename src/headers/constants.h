#pragma once

#include <Arduino.h>

constexpr uint16_t STEPS_PER_REV = 200;  // Number of steps per revolution
constexpr uint16_t CYLINDER_RADIUS = 36; // Radius of the cylinder in mm
constexpr float THREAD_PITCH = 1.5;      // Thread pitch in mm
constexpr uint16_t HEIGHT = 300;         // Height of the cylinder in mm

constexpr uint32_t MAX_POS = (uint32_t)STEPS_PER_REV * HEIGHT / THREAD_PITCH; // Max number of steps to limit switch from 0_pos

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

#define MINPRESSURE 10
#define MAXPRESSURE 1000

enum Actions
{
    IDLE,
    HOMMING,
    MOVING,
    FILLING,
    STOPPED
};

enum Sign
{
    DECREMENT = -1,
    INCREMENT = 1
};
