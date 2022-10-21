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
    static uint16_t hold_counter(0);
    static uint16_t no_touch_counter(0);

    TSPoint tp = Touch::read();

    // if the touch is pressed
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {
        // call all handlers
        for (uint8_t i = 0; i < count; i++)
            handler[i](tp, hold_counter);
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
