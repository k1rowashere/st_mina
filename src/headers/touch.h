#pragma once
#include <TouchScreen.h>
#include "constants.h"

namespace Touch
{
    // typedef an array of functions that take a TSPoint and return void
    typedef void (*Handle)(TSPoint, uint16_t);

    TSPoint read();

    // must be implemented here to avoid linker errors
    // template function that will create a handler
    template <uint16_t x_s, uint16_t y_s, uint16_t x_e, uint16_t y_e, void (*callback)(uint16_t)>
    void handle(TSPoint tp, uint16_t hold_counter)
    {
        if (tp.x > x_s && tp.x < x_e && tp.y > y_s && tp.y < y_e)
            callback(hold_counter);
    }

    // run all handlers
    void run_handles(const Touch::Handle handler[], uint8_t count);
}