#include <Arduino.h>


void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  
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
  digitalWrite(12,LOW);
  delayMicroseconds(2);
  digitalWrite(12,HIGH);
  delayMicroseconds(10);
  digitalWrite(12,LOW);
  
  int duration = pulseIn(13, HIGH);
  
  // Distance is in cm
  double distance = duration * 0.0343 / 2;
  return distance;
}
