#include <EEPROM.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h> // Hardware-specific library

#include "headers/stepper.h"
#include "headers/main.h"
#include "headers/draw.h"

// Convert Volume (ml) of product to number of steps from 0-pos
long volume_to_steps(long volume)
{
    // convert volume to mm^3
    volume *= 1000;

    // get height required to fill volume of the cylinder
    float height = volume / (PI * CYLINDER_RADIUS * CYLINDER_RADIUS);

    // calculate number of steps required to fill cylinder given the thread pitch
    return height * STEPS_PER_REV / THREAD_PITCH;
}

// Convert number of steps from 0-pos to Volume (ml) of product
int steps_to_volume(long steps)
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

    // stepper setup
    current_action[0] = HOMMING;
    Draw::draw_action(current_action[0]);
    current_pos[0] = stepper_0.homming();
    current_action[0] = IDLE;
    Draw::draw_action(current_action[0]);

    current_action[1] = HOMMING;
    Draw::draw_action(current_action[1], SCREEN_WIDTH / 2);
    current_pos[1] = stepper_1.homming();
    current_action[1] = IDLE;
}

void loop()
{
    long temp;
    // if current_pos is not equal to the value in EEPROM, show save button
    if (current_pos[0] != EEPROM.get(0, temp) || current_pos[1] != EEPROM.get(4, temp))
    {
        if (!not_saved)
        {
            not_saved = true;
            tft.setTextSize(2);
            tft.setTextColor(TFT_WHITE);
            tft.fillRect(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 50, 150, 50, TFT_BLACK);
            tft.drawRoundRect(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 50, 150, 50, 10, TFT_WHITE);
            tft.setCursor(SCREEN_WIDTH / 2 - 12 * 2, SCREEN_HEIGHT - 50 + 25 - 8);
            tft.print("Save");
        }
    }
    else
        not_saved = false;

    if (redraw_action)
    {
        redraw_action = false;
        Draw::draw_action(current_action[0]);
        Draw::draw_action(current_action[1], SCREEN_WIDTH / 2);
    }

    if (redraw_set_vol)
    {
        redraw_set_vol = false;
        Draw::draw_fill_v(current_pos[0]);
        Draw::draw_fill_v(current_pos[1], SCREEN_WIDTH / 2);
    }

    Draw::draw_plus_minus_buttons();
    Draw::draw_plus_minus_buttons(SCREEN_WIDTH / 2);

    // touch screen handling
    {
        static int hold_counter = 0;     // Counter for holding the button
        static int no_touch_counter = 0; // Counter for not touching the screen
        TSPoint tp = read_ts();

        // increment/decrement buttons
        auto inc_dec_button = [tp](int x_s, int x_e, int y_s, int y_e, long &value, Sign sign)
        {
            if (tp.x > x_s &&
                tp.x < x_e &&
                tp.y > y_s &&
                tp.y < y_e)
            {
                if (hold_counter > 20)
                    value += sign * volume_to_steps(40);
                else if (hold_counter > 10)
                    value += sign * volume_to_steps(10);
                else
                    value += sign * volume_to_steps(1);

                // clamp value 0-MAX_POS
                if (value > MAX_POS)
                    value = MAX_POS;
                else if (value < 0)
                    value = 0;

                hold_counter++;
                delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
                redraw_set_vol = true;
            }
        };

        // if a touch is detected
        if (tp.z > 0)
        {
            inc_dec_button(SCREEN_WIDTH / 4 - 75,
                           SCREEN_WIDTH / 4 - 25,
                           SCREEN_HEIGHT / 2 - 25,
                           SCREEN_HEIGHT / 2 + 25,
                           current_pos[0],
                           DECREMENT);
            inc_dec_button(SCREEN_WIDTH / 4 + 25,
                           SCREEN_WIDTH / 4 + 75,
                           SCREEN_HEIGHT / 2 - 25,
                           SCREEN_HEIGHT / 2 + 25,
                           current_pos[0],
                           INCREMENT);
            inc_dec_button(SCREEN_WIDTH / 4 * 3 - 75,
                           SCREEN_WIDTH / 4 * 3 - 25,
                           SCREEN_HEIGHT / 2 - 25,
                           SCREEN_HEIGHT / 2 + 25,
                           current_pos[1],
                           DECREMENT);
            inc_dec_button(SCREEN_WIDTH / 4 * 3 + 25,
                           SCREEN_WIDTH / 4 * 3 + 75,
                           SCREEN_HEIGHT / 2 - 25,
                           SCREEN_HEIGHT / 2 + 25,
                           current_pos[1],
                           INCREMENT);

            // save current_pos to eeprom button if not_saved is true
            if (not_saved &&
                tp.x > SCREEN_WIDTH / 2 - 75 &&
                tp.x < SCREEN_WIDTH / 2 + 75 &&
                tp.y > SCREEN_HEIGHT - 50 &&
                tp.y < SCREEN_HEIGHT)
            {
                Serial.println("Saving...");
                EEPROM.put(0, current_pos[0]);
                EEPROM.put(4, current_pos[1]);
                // draw saved
                tft.fillRect(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 50, 150, 50, TFT_BLACK);
                tft.drawRoundRect(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 50, 150, 50, 10, TFT_GREEN);
                tft.setCursor(SCREEN_WIDTH / 2 - 12 * 2, SCREEN_HEIGHT - 50 + 25 - 8);
                tft.setTextColor(TFT_GREEN);
                tft.print("Saved");
                not_saved = false;
                delay(1000);
                // remove saved
                tft.fillRect(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 50, 150, 50, TFT_BLACK);
                // redraw vertical line
                tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);
            }

            no_touch_counter = 0;
        }
        else
        {
            if (no_touch_counter > 30)
            {
                hold_counter = 0;
                no_touch_counter = 0;
            }
            else
                no_touch_counter++;
        }
    }
}