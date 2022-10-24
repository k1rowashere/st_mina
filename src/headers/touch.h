#pragma once
#include <TouchScreen.h>
#include "constants.h"

namespace Touch
{
    struct Handle
    {
        uint16_t x_s;
        uint16_t y_s;
        uint16_t w;
        uint16_t h;
        void (*callback)();
        bool &condition;
    };

    TSPoint read();

    // run all handlers
    void run_handles(const Touch::Handle handler[], uint8_t count);
}