
// #define STEP_PIN 22              // Step pin
// #define DIR_PIN 24               // Direction pin
// #define EN_PIN 26                // Enable pin
// #define LOW_LIMIT_SWITCH_PIN 28  // Limit switch pin
// #define HIGH_LIMIT_SWITCH_PIN 30 // Limit switch pin

#define STEPS_PER_REV 200  // Number of steps per revolution
#define CYLINDER_RADIUS 10 // Radius of the cylinder in mm
#define THREAD_PITCH 1.25  // Thread pitch in mm
#define HEIGHT 100         // Height of the cylinder in mm
// #define STEPS_LIMIT (HEIGHT * STEPS_PER_REV / THREAD_PITCH) // Number of steps to limit switch from 0_pos
#define MAX_POS 1000

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