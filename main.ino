
#include <EEPROM.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h> // Hardware-specific library

#include "stepper.h"

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

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Stepper stepper_1(22, 24, 26, 28, 30);
Stepper stepper_2(23, 25, 27, 29, 31);

int current_pos = 0;    // Current position in steps
int current_action = 0; // TODO: 0 - idle, 1 - homming, 2 - moving, 3 - limit switch...

int hold_counter = 0;     // Counter for holding the button
int no_touch_counter = 0; // Counter for not touching the screen
bool redraw_flag = true;  // Flag for redrawing the screen

void setup()
{
    // lcd setup
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(-45);
    tft.fillScreen(TFT_BLACK);
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

TSPoint read_ts(void)
{
    TSPoint tp = ts.getPoint();
    pinMode(YP, OUTPUT); // restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH); // because TFT control pins
    digitalWrite(XM, HIGH);

    // calibration
    tp.x = map(tp.x, TS_MINX, TS_MAXX, 0, 320);
    tp.y = map(tp.y, TS_MINY, TS_MAXY, 0, 480);

    // swap x and y because of rotation
    int temp = tp.x;
    tp.x = tp.y;
    tp.y = temp;

    return tp;
}

void loop()
{
    tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);

    // title
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 4 - 19, 0);
    tft.print("(1)");
    tft.setCursor(SCREEN_WIDTH / 4 * 3 - 19, 0);
    tft.print("(2)");

    // draw first half
    tft.setTextSize(2);
    tft.setCursor(18, 24 * 2);

    switch (current_action)
    {
    case 0:
        tft.setTextColor(TFT_WHITE);
        tft.print("Idle");
        break;
    case 1:
        tft.setTextColor(TFT_YELLOW);
        tft.print("Homing");
        break;
    case 2:
        tft.setTextColor(TFT_GREEN);
        tft.print("Moving");
        break;
    }

    tft.setCursor(18, tft.getCursorY() + 24);

    tft.setTextColor(TFT_LIGHTGREY);
    tft.print("Fill V: ");
    // draw volume
    if (redraw_flag)
    {
        tft.fillRect(tft.getCursorX(), tft.getCursorY(), SCREEN_WIDTH / 2 - tft.getCursorX(), 18, TFT_BLACK);
        redraw_flag = false;
    }
    // tft.print(steps_to_volume(current_pos));
    tft.print(current_pos);
    tft.print(" ml");

    // + / - buttons
    {
        int height = SCREEN_HEIGHT / 2 - 25;

        tft.drawRoundRect(SCREEN_WIDTH / 4 - 75, height, 50, 50, 10, TFT_WHITE);
        tft.drawRoundRect(SCREEN_WIDTH / 4 + 25, height, 50, 50, 10, TFT_WHITE);
        tft.setCursor(SCREEN_WIDTH / 4 - 50 - 6, height + 25 - 8);
        tft.print("-");
        tft.setCursor(SCREEN_WIDTH / 4 + 50 - 6, height + 25 - 8);
        tft.print("+");
    }

    // read touch screen

    TSPoint p = read_ts();
    if (p.z > 100 && p.z < 1000)
    {
        // check if +/- button is pressed
        // accelerated increment/ decrement of current_pos
        if (
            p.x > SCREEN_WIDTH / 4 - 75 &&
            p.x < SCREEN_WIDTH / 4 - 25 &&
            p.y > SCREEN_HEIGHT / 2 - 25 &&
            p.y < SCREEN_HEIGHT / 2 + 25)
        {
            if (hold_counter > 10)
                // current_pos -= volume_to_steps(10);
                current_pos -= 10;
            else
                // current_pos -= volume_to_steps(1);
                current_pos--;

            if (current_pos < 0)
                current_pos = 0;

            hold_counter++;
            delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));

            redraw_flag = true;
        }
        else if (
            p.x > SCREEN_WIDTH / 4 + 25 &&
            p.x < SCREEN_WIDTH / 4 + 75 &&
            p.y > SCREEN_HEIGHT / 2 - 25 &&
            p.y < SCREEN_HEIGHT / 2 + 25)
        {
            if (hold_counter > 10)
            {
                // current_pos += volume_to_steps(10);
                current_pos += 10;
            }
            else
            {
                // current_pos += volume_to_steps(1);
                current_pos++;
            }

            if (current_pos > MAX_POS)
                current_pos = MAX_POS;

            hold_counter++;
            delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));

            redraw_flag = true;
        }
    }
    else
    {
        // reset hold_counter if touch is released for some time
        if (no_touch_counter > 50)
        {
            hold_counter = 0;
            no_touch_counter = 0;
        }
        else
            no_touch_counter++;
    }
}