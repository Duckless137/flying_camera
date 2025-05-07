#include <Arduino.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include <vars.h>
#include <Servo.h>
// Function & class symbols
void check_calibration(void);
void wait_for_button(void);
void wait_for_altitude(void);
void ping(int pitch);
void start_photo_taking(void);
double ping_sonar(void);
SFE_BMP180 barometer;
Servo camera_pusher_2000;

// ---MAIN CODE---
void setup()
{
  if (!barometer.begin())
  {
    // Barometer failed - let the user know
    ping(ERROR_NOTE);
    while (1); // Ultimate dilly dally
  }
 
  // Attach servo MOTOR
  camera_pusher_2000.attach(PIN_SERVO);
  
  // Set pin modes
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_SPEAKER, OUTPUT); 

  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_SONAR_ECHO, INPUT);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

void loop()
{
  check_calibration();
  wait_for_button();
  wait_for_altitude();
  ping(DESCENT_START_NOTE);
  start_photo_taking();
  while (digitalRead(PIN_BUTTON) == HIGH) ping(LANDED_NOTE); // Hehe this might get annoying
  while (1); // The button press is just to stop the pinging. After this, the user has to reset the Arduino
}





// ---ALTIMITER CODE---
char status;
double temp;
double pressure;
double get_altitude(void);

void wait_for_altitude(void)
{
  do 
  {
    get_altitude();
  }
  while (temp < TARGET_ALTITUDE);
  
}

double get_altitude(void)
{
  status = barometer.startTemperature();
  if (status)
  {
    // According to the library, the `startTemperature` method
    // returns an ammount of time to wait... for some reason.
    delay(status);

    // This actually passes in a pointer to
    // temp??? C++ is so weird.
    status = barometer.getTemperature(temp);
    if (status)
    {
      return barometer.getPressure(pressure, temp);
    }
  }

  /* If the function gets here before 
   * returning, it means an error occured.
   * Not much the user can do about this while 
   * the device is several meters in the
   * air, so if measurement fails... just
   * try again.
   */
  return 0;  
}






// ---BUTTON CODE---
void wait_for_button(void)
{
   while (!digitalRead(PIN_BUTTON)); // Wait for button to be pressed    
   while (digitalRead(PIN_BUTTON)); // Wait for button to be let go    
}





// ---SERVO MOTOR CODE---
void start_photo_taking(void)
{
  // There's no multithreading :(
  camera_pusher_2000.write(0);
  check_calibration();
  delay(200);
  camera_pusher_2000.write(45);
  
  if (ping_sonar() < LANDING_RANGE) return;

  delay(200);
}





// ---SONAR CODE---
void check_calibration(void)
{
    double should_be_zero = ping_sonar();
    
    // If it's less than 10cm, good enough
    if (should_be_zero < 10.0)
    {
        return;
    } else {
        ping(ERROR_NOTE);
        while (1); // We really can't continue the program if this fails, so this just stops everything.
    }
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





// ---SPEAKER CODE---
void ping(int pitch)
{
    digitalWrite(PIN_BUTTON, HIGH);
    tone(PIN_SPEAKER, pitch, 150);
    delay(160);                
  	tone(PIN_SPEAKER, pitch, 150);
    delay(800);
}
