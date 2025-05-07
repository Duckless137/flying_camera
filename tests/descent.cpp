#include <Servo.h>
#include <Arduino.h>
#include <vars.h>

Servo servo;
double ping_sonar(void);

void setup()
{
    
  servo.attach(PIN_SERVO);

  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_SONAR_ECHO, INPUT);
}

void loop()
{
  servo.write(0);
  delay(200);
  servo.write(45);
  if (ping_sonar() < 10.0) {
    while (1);
  }
  delay(200);
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
