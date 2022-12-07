#pragma once
#include "constants.h"

// Convert Volume (ml) of product to number of steps from 0-pos
const uint16_t volume_to_steps(uint16_t volume);

// Convert number of steps from 0-pos to Volume (ml) of product
const uint16_t steps_to_volume(uint16_t steps);

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
