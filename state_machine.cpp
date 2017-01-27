#include "state_machine.h"

int state;

/*
 * State start
 */
unsigned long recoveryStartMS;
unsigned long lunchStartMS;
unsigned long confirmStartMS;
unsigned long backupLastMS;

bool doBackup;

String roomID = "UNINITIALIZED_ROOM";
String deviceID;
unsigned long lunchDurationMS = LUNCH_DURATION_MS;

void state_init(int initial) {
  if (initial == STATE_RECOVERY) {
    set_state_recovery();
  } else {
    set_state_idle();
  }

  doBackup = false;
  deviceID = System.deviceID();
}

void state_update() {
  doBackup = false;
  unsigned long now = millis();

  if (is_recovery()) {
    if ((now - recoveryStartMS) > RECOVERY_DURATION_MS) {
      set_state_idle();
    }
  } else {
    if (is_lunch()) {
      if ((now - lunchStartMS) >= lunchDurationMS) {
        set_state_idle();
      }
    }

    if (now - backupLastMS >= BACKUP_INTERVAL_MS) {
      backupLastMS = now;
      doBackup = true;
    }
  }

  if (is_confirm() && (now - confirmStartMS) >= CONFIRM_DURATION_MS) {
    set_state_idle();
  }
}

int get_state() {
  return state;
}

void set_state_recovery() {
  recoveryStartMS = millis();
  state = STATE_RECOVERY;
}

void set_state_idle() {
  state = STATE_IDLE;
}

void set_state_lunch() {
  lunchStartMS = millis();
  state = STATE_LUNCH;
}

void set_state_confirm() {
  confirmStartMS = millis();
  state = STATE_CONFIRM;
}

void set_state_discovery() {
  state = STATE_DISCOVERY;
}

bool is_recovery() {
  return state == STATE_RECOVERY;
}

bool is_lunch() {
  return state == STATE_LUNCH;
}

bool is_confirm() {
  return state == STATE_CONFIRM;
}

bool is_discovery() {
  return state == STATE_DISCOVERY;
}

bool is_idle() {
  return state == STATE_IDLE;
}

bool should_backup() {
  return doBackup;
}

void set_room_id(String id) {
  roomID = id;
}

String get_room_id(void) {
  return roomID;
}

void set_lunch_duration(unsigned long duration) {
  lunchDurationMS = duration;
}

unsigned long get_lunch_duration() {
  return lunchDurationMS;
}

String get_device_id(void) {
  return deviceID;
}
