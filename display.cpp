
#include "display.h"

int state = 0;

void lunchDone() {
  state = STATE_LUNCH_DONE;
}

void lunchFirstCall() {
  state = STATE_LUNCH_FIRST_CALL;
}

void lunchLastCall() {
  state = STATE_LUNCH_SECOND_CALL;
}

void display() {
  switch (state) {
    case STATE_LUNCH_DONE:
      digitalWrite(PIN_LUNCH_FIRST_CALL, LOW);
      digitalWrite(PIN_LUNCH_LAST_CALL, LOW);
    break;

    case STATE_LUNCH_FIRST_CALL:
      digitalWrite(PIN_LUNCH_FIRST_CALL, HIGH);
      digitalWrite(PIN_LUNCH_LAST_CALL, LOW);
    break;

    case STATE_LUNCH_SECOND_CALL:
      digitalWrite(PIN_LUNCH_FIRST_CALL, LOW);
      digitalWrite(PIN_LUNCH_LAST_CALL, HIGH);
    break;

    default:
      digitalWrite(PIN_LUNCH_FIRST_CALL, LOW);
      digitalWrite(PIN_LUNCH_LAST_CALL, LOW);
    break;
  }
}
