#include "constants.h"
#include "draw.h"
#include "global.h"

#include <EEPROM.h>

#define INC_DEC__(pos, sign) inc_dec_##pos##_##sign

// macro to create +/- buttons handlers
void INC_DEC__(0, NEGATIVE)();
void INC_DEC__(0, POSITIVE)();
void INC_DEC__(1, NEGATIVE)();
void INC_DEC__(1, POSITIVE)();

void apply();
void cancel();
void lock();
void ack();