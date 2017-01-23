#include "overhead_display.h"
#include "display.h"

unsigned long food_time_served;
bool food_is_served;

void setup() {
  pinMode(PIN_LUNCH_FIRST_CALL, OUTPUT);
  pinMode(PIN_LUNCH_LAST_CALL, OUTPUT);

  digitalWrite(PIN_LUNCH_FIRST_CALL, LOW);
  digitalWrite(PIN_LUNCH_LAST_CALL, LOW);

  Particle.function("food.display", food);
  food_time_served = 0;
  food_is_served = false;

  pinMode(D2, INPUT_PULLUP);
}

void loop() {
  int pushButtonState = digitalRead(D2);
  if (pushButtonState == LOW) {
    food_is_served = true;
    food_time_served = millis();
  }

  if (food_is_served) {
    unsigned long delta = millis() - food_time_served;
    if (delta >= FOOD_FIRT_CALL_MS + FOOD_LAST_CALL_MS) {
      lunchDone();
      food_time_served = false;
    } else if (delta >= FOOD_FIRT_CALL_MS) {
      lunchLastCall();
    } else {
      lunchFirstCall();
    }
  }
  display();

  delay(INTERVAL_DELAY_MS);
}

int food(String command) {
  char id[20];
  strcpy(id, "is_servied_");
  strcpy(id, ROOM_ID);

  if (command == id) {
    food_is_served = true;
    food_time_served = millis();

    return 0;
  }

  return 1;
}
