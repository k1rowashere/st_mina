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
    FILLING,
    STOPPED
};

enum Sign
{
    DECREMENT = -1,
    INCREMENT = 1
};

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// (STEP_PIN, DIR_PIN, EN_PIN, LOW_LIMIT_SWITCH_PIN, HIGH_LIMIT_SWITCH_PIN)
Stepper stepper_0(22, 24, 26, 28, 30);
Stepper stepper_1(23, 25, 27, 29, 31);

long current_pos[2];                      // Current position in steps
Actions current_action[2] = {IDLE, IDLE}; // Current action

bool redraw_set_vol = true; // Flag for redrawing the volume
bool redraw_action = true;  // Flag for redrawing the action
bool not_saved = false;     // Flag for saving the volume to EEPROM

// forward declarations
int steps_to_volume(long steps);
long volume_to_steps(int volume);
