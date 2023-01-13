#pragma once
#include "constants.h"
#include <TouchScreen.h>

// pins
#define YP A3
#define XM A2
#define YM 9
#define XP 8

// calibration
#define TS_MINX 110
#define TS_MAXX 913
#define TS_MINY 90
#define TS_MAXY 950

#define MINPRESSURE 10
#define MAXPRESSURE 1000

namespace Touch
{
    struct Args
    {
        uint16_t *vis_set_pos;
        Status *current_status;
        bool &not_saved;
        bool &ack_flag;
        bool &pos_unlock;
        bool &clear_error;
    };

    struct Handle
    {
        uint16_t x_s;
        uint16_t y_s;
        uint16_t w;
        uint16_t h;
        void (*callback)(Touch::Args args);
        bool &condition;
    };

    TSPoint read();

    // run all handlers
    void run_handles(const Touch::Handle handler[], uint8_t count, Touch::Args args);
}