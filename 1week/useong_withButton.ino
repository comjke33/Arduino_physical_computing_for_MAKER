#include <TaskScheduler.h>
#include <Adafruit_NeoPixel.h>

int position = 60;
int direction = 1;
unsigned int delayval = 100;
void anime();

void button();

#define PIXEL_PIN 6

#define PIXEL_COUNT 60

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define BUTTON_PIN 2

Scheduler ts;
Task ani(delayval, TASK_FOREVER, anime, &ts, true);
Task setColor(0, TASK_FOREVER, button, &ts, true);


bool oldState = HIGH;
unsigned long Color = strip.Color(0, 150, 0);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show();
}


void loop() {
  ts.execute();
}

void button() {
  bool newState = digitalRead(BUTTON_PIN);
  if (newState == LOW && oldState == HIGH) {
    if (position < 5) {
      direction = 1;
      position = 2;
      if (delayval > 10) {
        ani.setInterval(delayval -= 20);
      } else {
        ani.abort();
        for (int i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(rand() % 256, rand() % 256, rand() % 256));
          strip.show();
        }
      }
    }
    delay(100);
  }
  oldState = newState;
}

void anime() {
  strip.clear();
  if (direction) {
    for (int i = 0; i < 3; i++) {
      strip.setPixelColor(position - i, Color - strip.Color(0, i * 50, 0));
    }
    position++;
    if (position > 60) {
      position = 57;
      direction = 0;
    }
  } else {
    for (int i = 0; i < 3; i++) {
      strip.setPixelColor(position - i, Color - strip.Color(0, i * 50, 0));
    }
    position--;
    if (position < 0) {
      for (int i = 1; i < 4; i++) {
        strip.setPixelColor(position + i, strip.Color(150, 0, 0));
      }
      ani.abort();
    }
  }
  strip.show();
}