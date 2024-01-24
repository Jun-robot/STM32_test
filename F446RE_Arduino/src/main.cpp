#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN,HIGH);
  delay(90);
  digitalWrite(LED_BUILTIN,LOW);
  delay(90);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(90);
  digitalWrite(LED_BUILTIN,LOW);
  delay(90);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(90);
  digitalWrite(LED_BUILTIN,LOW);
  delay(90);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(450);
  digitalWrite(LED_BUILTIN,LOW);
  delay(450);
}