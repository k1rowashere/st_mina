#pragma once
#include "constants.h"
#include <EEPROM.h>

// Convert Volume (ml) of product to number of steps from 0-pos
const inline uint16_t volume_to_steps(uint16_t volume)
{
    // convert volume to mm^3
    volume *= 1000;

    // get height required to fill volume of the cylinder
    float height = volume / (PI * CYLINDER_RADIUS * CYLINDER_RADIUS);

    // calculate number of steps required to fill cylinder given the thread pitch
    return height * STEPS_PER_REV / THREAD_PITCH;
}

// Convert number of steps from 0-pos to Volume (ml) of product
const inline uint16_t steps_to_volume(uint16_t steps)
{
    // calculate height of cylinder given the number of steps
    float height = steps * THREAD_PITCH / STEPS_PER_REV;

    // calculate volume of cylinder given the height
    float volume = PI * CYLINDER_RADIUS * CYLINDER_RADIUS * height;

    // convert volume to ml
    return volume / 1000;
}

template <typename T>
T eeprom_read(size_t addr)
{
    T out = 0;
    EEPROM.get(addr, out);
    return out;
}

// fixed queue implementation
template <typename T, size_t N>
class Queue
{
    T data[N + 1];
    size_t head = 0;
    size_t tail = 0;

public:
    // add an item to the queue
    T push(T item)
    {
        data[tail] = item;
        tail = (tail + 1) % (N + 1);
        return item;
    }

    // remove an item from the queue
    T pop()
    {
        T item = data[head];
        head = (head + 1) % (N + 1);
        return item;
    }

    // return the number of items in the queue
    size_t size()
    {
        return (tail - head + (N + 1)) % (N + 1);
    }

    constexpr size_t capacity()
    {
        return N;
    }

    bool is_full()
    {
        return size() == N;
    }
};


// rgb led functions
struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

inline RGB hue_to_rgb(uint16_t hue)
{
    RGB rgb = { 0, 0, 0 };

    if (hue < 60)
    {
        rgb.r = 255;
        rgb.g = hue * 255 / 60;
    }
    else if (hue < 120)
    {
        rgb.r = (120 - hue) * 255 / 60;
        rgb.g = 255;
    }
    else if (hue < 180)
    {
        rgb.g = 255;
        rgb.b = (hue - 120) * 255 / 60;
    }
    else if (hue < 240)
    {
        rgb.g = (240 - hue) * 255 / 60;
        rgb.b = 255;
    }
    else if (hue < 300)
    {
        rgb.r = (hue - 240) * 255 / 60;
        rgb.b = 255;
    }
    else
    {
        rgb.r = 255;
        rgb.b = (360 - hue) * 255 / 60;
    }

    return rgb;
}
inline void rgb(RGB color)
{
    constrain(color.r, 0, 255);
    constrain(color.g, 0, 255);
    constrain(color.b, 0, 255);

    analogWrite(LED_R, color.r);
    analogWrite(LED_G, color.g);
    analogWrite(LED_B, color.b);
}

