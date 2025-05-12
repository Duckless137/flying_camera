#include "vars.h"
#include <Arduino.h>


void setup() {
  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_SONAR_ECHO, INPUT);
  
  Serial.begin(9600);
}

double ping_sonar();

void loop() {
  Serial.print("Cm: ");
  Serial.println(ping_sonar());
  delay(100);
}

double ping_sonar(void) {
  // This has to happen for some reason...
  digitalWrite(PIN_SONAR_TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_SONAR_TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_SONAR_TRIG,LOW);
  
  int duration = pulseIn(PIN_SONAR_ECHO, HIGH);
  
  // Distance is in cm
  double distance = duration * 0.0343 / 2;
  return distance;
}
