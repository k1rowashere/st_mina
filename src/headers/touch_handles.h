#include "constants.h"
#include "draw.h"
#include "global.h"

#define INC_DEC__(pos, sign) inc_dec_##pos##_##sign

// macro to create +/- buttons handlers
void INC_DEC__(0, NEGATIVE)();
void INC_DEC__(0, POSITIVE)();
void INC_DEC__(1, NEGATIVE)();
void INC_DEC__(1, POSITIVE)();

// apply button handler
void apply();

// cancel button handler
void cancel();

// lock button handler
void lock();