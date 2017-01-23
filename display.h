#ifndef DISPLAY_H
#define DISPLAY_H

#include "Particle.h"

#define STATE_LUNCH_DONE 0
#define STATE_LUNCH_FIRST_CALL 1
#define STATE_LUNCH_SECOND_CALL 2

#define PIN_LUNCH_FIRST_CALL D0
#define PIN_LUNCH_LAST_CALL D1

void lunchDone(void);
void lunchFirstCall(void);
void lunchLastCall(void);
void display(void);

#endif
