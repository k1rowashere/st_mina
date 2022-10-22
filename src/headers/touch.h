#pragma once
#include <TouchScreen.h>
#include "constants.h"

namespace Touch
{
    struct Handle
    {
        uint16_t x_s;
        uint16_t x_e;
        uint16_t y_s;
        uint16_t y_e;
        void (*callback)();
    };

    TSPoint read();

    // run all handlers
    void run_handles(const Touch::Handle handler[], uint8_t count);
}