#include <TaskScheduler.h>
#include <Adafruit_NeoPixel.h>

int position = 30;
int direction = 0;
unsigned int delayval = 40;
void anime();

void judge1();
void judge2();
void win1();
void win2();

#define PIXEL_PIN 6

#define PIXEL_COUNT 70

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define player1 2
#define player2 3

Scheduler ts;
Task ani(delayval, TASK_FOREVER, anime, &ts, true);
Task p1(0, TASK_FOREVER, judge1, &ts, true);
Task p2(0, TASK_FOREVER, judge2, &ts, true);
Task w1(100, TASK_FOREVER, win1, &ts, false);
Task w2(100, TASK_FOREVER, win2, &ts, false);

unsigned long Color = strip.Color(0, 150, 0);

void setup() {
  pinMode(player1, INPUT_PULLUP);
  pinMode(player2, INPUT_PULLUP);
  strip.begin();
  strip.show();
}


void loop() {
  ts.execute();
}

void win1() {
  int i;
  for (i = 0; i < strip.numPixels() / 2; i++) {
    strip.setPixelColor(i, strip.Color(rand() % 256, rand() % 256, rand() % 256));
    strip.show();
  }
  for (i; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(150, 0, 0));
    strip.show();
  }
}

void win2() {
  int i;
  for (i = strip.numPixels() - 1; i >= strip.numPixels() / 2; i--) {
    strip.setPixelColor(i, strip.Color(rand() % 256, rand() % 256, rand() % 256));
    strip.show();
  }
  for (i; i >= 0; i--) {
    strip.setPixelColor(i, strip.Color(150, 0, 0));
    strip.show();
  }
}

void judge1() {
  static bool oldState = HIGH;
  bool newState = digitalRead(player1);
  if (newState == LOW && oldState == HIGH) {
    if (position < 5) {
      direction = 1;
      position = 2;
      if (delayval > 10) {
        ani.setInterval(delayval -= delayval / 10);
      } else {
        ani.abort();
        w1.enable();
      }
    }
    delay(20);
  }
  oldState = newState;
}

void judge2() {
  static bool oldState = HIGH;
  bool newState = digitalRead(player2);
  if (newState == LOW && oldState == HIGH) {
    if (position > 54) {
      direction = 0;
      position = 57;
      if (delayval > 10) {
        ani.setInterval(delayval -= delayval / 10);
      } else {
        ani.abort();

        w2.enable();
      }
    }
    delay(20);
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
      ani.abort();
      p1.abort();
      p2.abort();
      w1.enable();
    }
  } else {
    for (int i = 0; i < 3; i++) {
      strip.setPixelColor(position - i, Color - strip.Color(0, i * 50, 0));
    }
    position--;
    if (position < 0) {
      ani.abort();
      p1.abort();
      p2.abort();
      w2.enable();
    }
  }
  strip.show();
}