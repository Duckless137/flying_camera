#include <Servo.h>
#include <Arduino.h>

Servo servo;
void setup()
{
  servo.attach(13);
}

void loop()
{
  servo.write(10);
  delay(200);
  servo.write(100);
  delay(200);
}
