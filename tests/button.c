#include <Arduino.h>
#include <stdint.h>
#include <vars.h>

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SPEAKER, OUTPUT);
}

uint8_t led = LOW;
void loop() {
  while (digitalRead(PIN_BUTTON) == HIGH);
  while (digitalRead(PIN_BUTTON) == LOW);
  if (led == LOW) {
    led = HIGH;
  } else {
    led = LOW;
  }
  digitalWrite(PIN_LED, led);
}

