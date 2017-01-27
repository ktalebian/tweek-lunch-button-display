#include "display.h"

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int worldLunch[8][WORLD_LENGTH] =
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

uint16_t discoveryIndex;
uint16_t recoveryIndex;
float lunchOffset;
int brightness;
int matrixLength;

float birghtnessModulate;
int birghtnessModulateDirection;

void display_init() {
  birghtnessModulate = 0;
  birghtnessModulateDirection = 1;
  brightness = BRIGHTNESS;

  strip.begin();
  strip.setBrightness(brightness);
  strip.show();

  matrixLength = sqrt(strip.numPixels());
}

void display_update() {
  if (is_recovery()) {
    show_recovery();
  } else {
    recoveryIndex = 0;
  }

  if (is_lunch()) {
    show_lunch();
  } else {
    lunchOffset = 0;
  }

  if (is_confirm()) {
    show_confirm();
  }

  if (is_discovery()) {
    show_discovery();
  } else {
    discoveryIndex = 0;
  }

  if (is_idle()) {
    show_idle();
  }

  if (!is_confirm() && !is_lunch()) {
    strip.setBrightness(brightness);
    strip.show();
    birghtnessModulate = 0;
    birghtnessModulateDirection = 1;
  }
}

void show_recovery() {
  color_wipe(strip.Color(70, 130, 180), recoveryIndex);
  recoveryIndex++;
  if (recoveryIndex >= strip.numPixels()) {
    recoveryIndex = 0;
  }
}

void show_lunch() {
  draw_lunch(strip.Color(176, 23, 31), strip.Color(255, 215, 0), (int) lunchOffset);
  brightness_modulate(5);
  lunchOffset += 0.125;
  if (lunchOffset >= WORLD_LENGTH - matrixLength) {
    lunchOffset = 0;
  }

}

void show_confirm() {
  solid(strip.Color(102, 205, 0), true);
  brightness_modulate(50);
}

void show_discovery() {
  chase(strip.Color(139, 0, 139), discoveryIndex);
  discoveryIndex++;
  if (discoveryIndex >= strip.numPixels()) {
    discoveryIndex = 0;
    strip.clear();
  }
}

void show_idle() {
  strip.clear();
}

void brightness_modulate(int inc) {
  birghtnessModulate = birghtnessModulate + inc * birghtnessModulateDirection;
  if (birghtnessModulate >= 100.0) {
    birghtnessModulateDirection = -1 * birghtnessModulateDirection;
    birghtnessModulate = 100;
  }
  if (birghtnessModulate <= 0.0) {
    birghtnessModulateDirection = -1 * birghtnessModulateDirection;
    birghtnessModulate = 0;
  }

  strip.setBrightness((int) birghtnessModulate);
}

// Fill the dots one after the other with a color
void color_wipe(uint32_t c, uint16_t i) {
  if (i == 0) {
    strip.clear();
  }
  strip.setPixelColor(i, c);
  strip.show();
}


void chase(uint32_t c, uint16_t i) {
  strip.setPixelColor(i  , c); // Draw new pixel
  strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
  strip.show();
}

void draw_lunch(uint32_t color, uint32_t back, int offset) {
  solid(back, false);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    uint16_t r = i / matrixLength;
    uint16_t c = i % matrixLength + offset;
    if (worldLunch[r][c]) {
      strip.setPixelColor(i, color);
    }

  }

  strip.show();
}

void solid(uint32_t c, bool render) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c); // Draw new pixel
  }
  if (render) {
      strip.show();
  }
}
