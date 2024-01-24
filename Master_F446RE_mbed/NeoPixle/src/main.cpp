#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define PIN 8
#define NUMPIXELS 37

Adafruit_NeoPixel pixels =
    Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 200;
int lp_cnt = 0;
uint32_t tmp_color = pixels.Color(0, 0, 0);

void setup() {
  pinMode(8, OUTPUT);
  pixels.begin();
}

void loop() {
  if (lp_cnt % 3 == 0) {
    tmp_color = pixels.Color(20, 20, 20);
  } else if (lp_cnt % 3 == 1) {
    tmp_color = pixels.Color(0, 250, 0);
  } else if (lp_cnt % 3 == 2) {
    tmp_color = pixels.Color(0, 0, 250);
  }

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, tmp_color);
    pixels.show();
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
  delay(500);

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(3);
  }
  delay(delayval);
  lp_cnt++;
}