#include <Servo.h>
#include <Arduino.h>

Servo servo;
void setup()
{
  servo.attach(13);
}

void loop()
{
  servo.write(0);
  delay(1000);
  servo.write(180);
  delay(1000);
}
