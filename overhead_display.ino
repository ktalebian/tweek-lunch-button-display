#include "overhead_display.h"

void setup() {
  Particle.syncTime();
  display_init();
  state_init(STATE_RECOVERY);

  pinMode(LED_POWER, OUTPUT);
  digitalWrite(LED_POWER, HIGH);

  Particle.function("food", food);
  Particle.function("program", program);
  Particle.publish("systemOnline");

  Serial.begin(9600);
}

void loop() {
  state_update();
  display_update();

  if (!is_recovery() && should_backup()) {
    backup_device();
  }

  delay(DELAY);
}

int food(String id) {
  if (get_device_id() == id || id == get_room_id()) {
    set_state_lunch();
    return 0;
  }

  return 1;
}

int program(String payload) {
  String command = payload.substring(0, payload.indexOf(":"));
  String arg = payload.substring(payload.indexOf(":") + 1);
  Serial.println();
  Serial.println("---> PROGRAM:: ");
  Serial.println(payload);
  Serial.println(command);
  Serial.println(arg);
  Serial.println();

  if (command == "room") {
    set_room_id(arg);
    set_state_confirm();
  } else if (command == "duration") {
    set_lunch_duration((int) (arg.toFloat() * 60 * 1000));
    set_state_confirm();
  } else if (command == "light") {
    if (arg == "on") {
      set_state_discovery();
    } else if (arg == "off") {
      set_state_idle();
    }
  } else if (command == "rebootComplete") {
    set_state_idle();
  }

  return 0;
}

void backup_device() {
  String data = "r:" + get_room_id() + ",d:" + get_lunch_duration();
  Particle.publish("metaData", data);
}
