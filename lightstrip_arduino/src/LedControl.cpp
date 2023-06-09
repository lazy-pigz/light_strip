#include <Arduino.h>
#include <FastLED.h>

#include "LedControl.h"
#include "LedStorage.h"

const uint8_t NUM_LEDS = 29;
const uint8_t DATA_PIN = 2;

static CRGB leds[NUM_LEDS];
static uint8_t r, g, b, brightness;

static void updateAllLeds(uint8_t newR, uint8_t newG, uint8_t newB);
static void updateAllLeds(CRGB::HTMLColorCode newColor);

void LED_init(void)
{
   r = Storage_getEepromRed();
   g = Storage_getEepromGreen();
   b = Storage_getEepromBlue();
   brightness = Storage_getEepromBrightness();

   // Current lightstrip uses GRB ordering
   FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
   for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(r, g, b);
   }
   FastLED.setBrightness(brightness);
   FastLED.show();
}

void LED_changeColor(uint8_t newR, uint8_t newG, uint8_t newB, uint8_t newBrightness)
{
   FastLED.clear();
   updateAllLeds(newR, newG, newB);
   FastLED.setBrightness(newBrightness);
   FastLED.show();

   Storage_updateEepromRed(newR);
   Storage_updateEepromGreen(newG);
   Storage_updateEepromBlue(newB);
   Storage_updateEepromBrightness(newBrightness);

   // r = newR;
   // g = newG;
   // b = newB;
   // brightness = newBrightness;
}

void LED_ledsOn(void)
{
   FastLED.clear();
   updateAllLeds(r, g, b);
   FastLED.setBrightness(brightness);
   FastLED.show();
}

void LED_ledsOff(void)
{
   FastLED.clear();
   FastLED.show();
}

void LED_debugFlash(uint8_t timesToFlash, uint8_t delayMs, CRGB::HTMLColorCode hexColorCode)
{
   for (uint8_t i = 0; i < timesToFlash; i++) {
      FastLED.clear();
      updateAllLeds(hexColorCode);
      FastLED.show();
      delay(delayMs);

      FastLED.clear();
      updateAllLeds(CRGB::HTMLColorCode::Black);
      FastLED.show();
      delay(delayMs);
   }

   FastLED.clear();
   updateAllLeds(r, g, b);
   FastLED.show();
}

static void updateAllLeds(uint8_t newR, uint8_t newG, uint8_t newB)
{
   for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(newR, newG, newB);
   }
}

static void updateAllLeds(CRGB::HTMLColorCode newColor)
{
   for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(newColor);
   }
}
