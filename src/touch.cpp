#include "headers/touch.h"

TSPoint Touch::read(void)
{
    static TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
    TSPoint tp = ts.getPoint();

    // restore shared pins
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);
    // TFT control pins
    digitalWrite(YP, HIGH);
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

// template <uint16_t x_s, uint16_t y_s, uint16_t x_e, uint16_t y_e, void (*callback)()>
// void Touch::handle(TSPoint tp)
// {
//     if (tp.x > x_s && tp.x < x_e && tp.y > y_s && tp.y < y_e)
//         callback();
// }

void Touch::run_handles(const Touch::Handle handler[], uint8_t count)
{
    TSPoint tp = Touch::read();

    // if the touch is pressed
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {
        // call all handlers
        for (uint8_t i = 0; i < count; i++)
            handler[i](tp);
    }
}

// void Touch::handle()
// {
//     // TODO: refactor inner functions.
//     static int hold_counter = 0;     // Counter for holding the button
//     static int no_touch_counter = 0; // Counter for not touching the screen
//     TSPoint tp = read();

//     // increment/decrement buttons
//     auto inc_dec_button = [](uint32_t &value, Sign sign)
//     {
//         if (hold_counter > 20)
//             value += sign * volume_to_steps(40);
//         else if (hold_counter > 10)
//             value += sign * volume_to_steps(10);
//         else
//             value += sign * volume_to_steps(1);

//         // clamp value 0-MAX_POS
//         if (value > MAX_POS)
//             value = MAX_POS;
//         else if (value < 0)
//             value = 0;

//         hold_counter++;
//         delay(500 / (hold_counter < 20 ? hold_counter % 10 + 1 : 10));
//         redraw_set_vol = true;
//     };

//     // if a touch is detected
//     if (tp.z > 0)
//     {
//         // left - button
//         if (SCREEN_WIDTH / 4 - 75,
//             SCREEN_WIDTH / 4 - 25,
//             SCREEN_HEIGHT / 2 - 25,
//             SCREEN_HEIGHT / 2 + 25)
//             inc_dec_button(current_pos[0], Sign::DECREMENT);
//         // left + button
//         else if (SCREEN_WIDTH / 4 + 25,
//                  SCREEN_WIDTH / 4 + 75,
//                  SCREEN_HEIGHT / 2 - 25,
//                  SCREEN_HEIGHT / 2 + 25)
//             inc_dec_button(current_pos[0], Sign::INCREMENT);
//         // right - button
//         else if (SCREEN_WIDTH / 4 * 3 - 75,
//                  SCREEN_WIDTH / 4 * 3 - 25,
//                  SCREEN_HEIGHT / 2 - 25,
//                  SCREEN_HEIGHT / 2 + 25)
//             inc_dec_button(current_pos[1], Sign::DECREMENT);
//         // right + button
//         else if (SCREEN_WIDTH / 4 * 3 + 25,
//                  SCREEN_WIDTH / 4 * 3 + 75,
//                  SCREEN_HEIGHT / 2 - 25,
//                  SCREEN_HEIGHT / 2 + 25)
//             inc_dec_button(current_pos[1], Sign::INCREMENT);
//         // Apply button (if not_saved == true)
//         else if (tp.x > SCREEN_WIDTH / 2 - 75 &&
//                  tp.x < SCREEN_WIDTH / 2 - 25 &&
//                  tp.y > SCREEN_HEIGHT - 50 &&
//                  tp.y < SCREEN_HEIGHT &&
//                  not_saved)
//         {
//             EEPROM.put(0, current_pos[0]);
//             EEPROM.put(4, current_pos[1]);

//             stepper_0.set_pos = current_pos[0];
//             stepper_1.set_pos = current_pos[1];

//             Draw::clear_buttons();
//         }
//         // Cancel button (if not_saved == true)
//         else if (tp.x > SCREEN_WIDTH / 2 + 25 &&
//                  tp.x < SCREEN_WIDTH / 2 + 75 &&
//                  tp.y > SCREEN_HEIGHT - 50 &&
//                  tp.y < SCREEN_HEIGHT &&
//                  not_saved)
//         {
//             // TODO:
//             Draw::clear_buttons();
//         }

//         no_touch_counter = 0;
//     }
//     else
//     {
//         if (no_touch_counter > 30)
//         {
//             hold_counter = 0;
//             no_touch_counter = 0;
//         }
//         else
//             no_touch_counter++;
//     }
// }
