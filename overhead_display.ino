#include "overhead_display.h"
#include "display.h"

// Broadcast identity every INTERVAL_BROADCAST_MS time
unsigned long broadcast_time;
// Time food was served
unsigned long food_time_served;
// How long the first call should last
unsigned long foodFirstCallDurationMS = FOOD_FIRT_CALL_MS;
// How long the second call should be last
unsigned long foodSecondCallDurationMS = FOOD_LAST_CALL_MS;
// How long the loop function should delay
unsigned long delayMS = INTERVAL_DELAY_MS;
// Food is being served
bool food_is_served;
// System is in detection mode (indicator only)
bool is_detecting;
// Unique device id
String deviceID;
// The room this device belongs to
String roomID = "NOT_SET_ROOM_ID";
bool sytem_recovery;

void setup() {
  Particle.syncTime();

  pinMode(PIN_LUNCH_FIRST_CALL, OUTPUT);
  pinMode(PIN_LUNCH_LAST_CALL, OUTPUT);

  digitalWrite(PIN_LUNCH_FIRST_CALL, LOW);
  digitalWrite(PIN_LUNCH_LAST_CALL, LOW);

  Particle.function("food", food);
  Particle.function("program", program);

  food_time_served = 0;
  food_is_served = false;
  is_detecting = false;
  sytem_recovery = true;
  broadcast_time = millis();
  deviceID = System.deviceID();

  Serial.begin(9600);

  Particle.publish("systemOnline");
}

void loop() {
  if (sytem_recovery) {
    recover();
  } else {
    if (is_detecting) {
      digitalWrite(PIN_LUNCH_FIRST_CALL, HIGH);
      digitalWrite(PIN_LUNCH_LAST_CALL, HIGH);
    } else {
      if (food_is_served) {
        unsigned long delta = millis() - food_time_served;
        if (delta >= FOOD_FIRT_CALL_MS + FOOD_LAST_CALL_MS) {
          lunchDone();
          food_is_served = false;
        } else if (delta >= FOOD_FIRT_CALL_MS) {
          lunchLastCall();
        } else {
          lunchFirstCall();
        }
      }
      display();
    }

    if (millis() - broadcast_time >= INTERVAL_BROADCAST_MS) {
      broadcast_time = millis();
      broadcastSelf();
    }

    delay(INTERVAL_DELAY_MS);
  }
}

int food(String id) {
  if (deviceID == id) {
    food_is_served = true;
    food_time_served = millis();

    return 0;
  }

  return 1;
}

int program(String payload) {
  String command = payload.substring(0, payload.indexOf(":"));
  String arg = payload.substring(payload.indexOf(":") + 1);

  if (command == "room") {
    roomID = arg;
    confirm();
  } else if (command == "firstCall") {
    foodFirstCallDurationMS = (int) (arg.toFloat() * 60 * 1000);
    confirm();
  } else if (command == "secondCall") {
    foodSecondCallDurationMS = (int) (arg.toFloat() * 60 * 1000);
    confirm();
  } else if (command == "delay") {
    delayMS = (int) (arg.toFloat() * 60 * 1000);
    confirm();
  } else if (command == "light") {
    if (arg == "on") {
      is_detecting = true;
    } else if (arg == "off") {
      is_detecting = false;
    }
  } else if (command == "rebootComplete") {
    sytem_recovery = false;
  }

  return 0;
}

void confirm() {
  for(int i = 0; i < 8; i++) {
    digitalWrite(PIN_LUNCH_FIRST_CALL, HIGH);
    digitalWrite(PIN_LUNCH_LAST_CALL, HIGH);
    delay(50);
    digitalWrite(PIN_LUNCH_FIRST_CALL, LOW);
    digitalWrite(PIN_LUNCH_LAST_CALL, LOW);
    delay(50);
  }
}

void broadcastSelf() {
  String data = "r:" + roomID + ",f:" + foodFirstCallDurationMS + ",s:" + foodSecondCallDurationMS + ",d:" + delayMS;
  Particle.publish("metaData", data);
}

void recover() {
  digitalWrite(PIN_LUNCH_FIRST_CALL, HIGH);
  digitalWrite(PIN_LUNCH_LAST_CALL, HIGH);

  if (millis() > MAX_RECOVERY_TIME_MS) {
    sytem_recovery = false;
  }
}
