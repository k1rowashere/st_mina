#include <EEPROM.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h> // Hardware-specific library

#include "headers/stepper.h"
#include "headers/main.h"

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Stepper stepper_1(22, 24, 26, 28, 30);
Stepper stepper_2(23, 25, 27, 29, 31);

int current_pos = 0; // Current position in steps
Actions current_action = IDLE;

int hold_counter = 0;      // Counter for holding the button
int no_touch_counter = 0;  // Counter for not touching the screen
bool redraw_volume = true; // Flag for redrawing the volume
bool redraw_action = true; // Flag for redrawing the action

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

void fill()
{
}

void setup()
{
    // lcd setup
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(-45);
    tft.fillScreen(TFT_BLACK);
    tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);
    // title
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 4 - 19, 0);
    tft.print("(1)");
    tft.setCursor(SCREEN_WIDTH / 4 * 3 - 19, 0);
    tft.print("(2)");
}

void loop()
{

    // draw first half
    tft.setTextSize(2);
    tft.setCursor(18, 24 * 2);

    if (redraw_action)
    {
        tft.fillRect(0, 24 * 2, SCREEN_WIDTH / 2, 24, TFT_BLACK);
        redraw_action = false;
    }

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
    if (redraw_volume)
    {
        tft.fillRect(tft.getCursorX(), tft.getCursorY(), SCREEN_WIDTH / 2 - tft.getCursorX(), 18, TFT_BLACK);
        redraw_volume = false;
    }
    tft.print(steps_to_volume(current_pos));
    // tft.print(current_pos);
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

        auto handle_hold_count = []()
        {
            hold_counter++;
            delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
            redraw_volume = true;
        };

        // check if +/- button is pressed
        // accelerated increment/ decrement of current_pos
        if (
            p.x > SCREEN_WIDTH / 4 - 75 &&
            p.x < SCREEN_WIDTH / 4 - 25 &&
            p.y > SCREEN_HEIGHT / 2 - 25 &&
            p.y < SCREEN_HEIGHT / 2 + 25)
        {
            if (hold_counter > 10)
                current_pos -= volume_to_steps(10);
            else
                current_pos -= volume_to_steps(1);

            if (current_pos < 0)
                current_pos = 0;

            handle_hold_count();
        }
        else if (
            p.x > SCREEN_WIDTH / 4 + 25 &&
            p.x < SCREEN_WIDTH / 4 + 75 &&
            p.y > SCREEN_HEIGHT / 2 - 25 &&
            p.y < SCREEN_HEIGHT / 2 + 25)
        {
            if (hold_counter > 10)
                current_pos += volume_to_steps(10);
            else
                current_pos += volume_to_steps(1);

            if (current_pos > MAX_POS)
                current_pos = MAX_POS;

            handle_hold_count();
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