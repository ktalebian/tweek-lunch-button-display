#include "Particle.h"

#define STATE_IDLE 0
#define STATE_LUNCH 1
#define STATE_DISCOVERY 2
#define STATE_RECOVERY 3
#define STATE_CONFIRM 4

void state_init(int);
int get_state(void);
void state_update();

void set_state_recovery(void);
void set_state_idle(void);
void set_state_lunch(void);
void set_state_confirm(void);
void set_state_discovery(void);

bool is_recovery(void);
bool is_lunch(void);
bool is_confirm(void);
bool is_discovery(void);
bool is_idle(void);
bool should_backup(void);

void set_room_id(String);
void set_lunch_duration(unsigned long);
String get_room_id(void);
String get_device_id(void);
unsigned long get_lunch_duration(void);

#define LUNCH_DURATION_MS 30000
#define RECOVERY_DURATION_MS 10000
#define CONFIRM_DURATION_MS 1000
#define BACKUP_INTERVAL_MS 10000
#define DELAY 10
