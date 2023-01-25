#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 6

#define PIXEL_COUNT 60

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

int position = 60;
int direction = 1;
int delayval = 100;
unsigned long prevTime = 0;
void loop() {
  if (prevTime + delayval <= millis()) {
    strip.clear();
    if (direction) {
      for (int i = 0; i < 3; i++) {
        strip.setPixelColor(position - i, strip.Color(150 - i * 50, 0, 0));
      }
      position++;
      if (position > strip.numPixels()) {
        position = strip.numPixels() - 3;
        direction = 0;
      }
    } else {
      for (int i = 0; i < 3; i++) {
        strip.setPixelColor(position + i, strip.Color(150 - i * 50, 0, 0));
      }
      position--;
      if (position < 0) {
        position = 2;
        direction = 1;
      }
    }
    strip.show();
    prevTime = millis();
  }
}
