#pragma once
#include "constants.h"

// Convert Volume (ml) of product to number of steps from 0-pos
uint32_t volume_to_steps(uint16_t volume);

// Convert number of steps from 0-pos to Volume (ml) of product
uint16_t steps_to_volume(uint32_t steps);

void reset();
void shutdown();