#ifndef DISPLAY_H
#define DISPLAY_H

#include "math.h"
#include "Particle.h"
#include "neopixel.h"
#include "state_machine.h"

void display_init(void);
void display_update(void);

#define PIXEL_PIN D2
#define PIXEL_COUNT 64
#define PIXEL_TYPE WS2812B
#define BRIGHTNESS 30
#define WORLD_LENGTH 57

void set_brightness(int);
int get_brightness(void);

void color_wipe(uint32_t, uint16_t);
void chase(uint32_t, uint16_t);
void solid(uint32_t, bool);
void draw_lunch(uint32_t, uint32_t, int);
void brightness_modulate(int);

void show_recovery(void);
void show_confirm(void);
void show_idle(void);
void show_lunch(void);
void show_discovery(void);

#endif
