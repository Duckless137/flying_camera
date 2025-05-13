#define DEBUG

#include "HardwareSerial.h"
#include <Arduino.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include <vars.h>
#include <Servo.h>
#include <logging.h>

// Function & class symbols
void wait_for_button(void);
void wait_for_altitude(void);
void ping(int pitch);
void start_photo_taking(void);
char check_calibration(void);
char start_readings(void);
double ping_sonar(void);
SFE_BMP180 barometer;
Servo camera_pusher_2000;
char status;
double temp, pressure, starting_point_pressure;
double altitude = 0.0;


// ---MAIN CODE---
void setup()
{
  Serial.begin(9600);
  LOG_INFO("Serial started!");

  LOG_TASK("Starting barometer");
  CHECK_STATUS(barometer.begin(), "Failed to start barometer!");
 
  LOG_TASK("Getting initial pressure readings");
  CHECK_STATUS(start_readings(), "Failed to get inital pressure readings!");
 
  // Attach servo MOTOR
  LOG_TASK("Attaching servo_motor");
  CHECK_STATUS(camera_pusher_2000.attach(PIN_SERVO), "Failed to attach servo motor!");
 
  // Sets servo motor to correct position, and waits
  // for it to get there just in case.
  LOG_TASK("Writing start pos to servo motor");
  camera_pusher_2000.write(SERVO_START);
  delay(500);
  LOG_TASK_SUCCESS;

  // This operation is infalible, BUT the
  // LOG_TASK method looks cool as fuck so...
  LOG_TASK("Setting pins");

  // Set pin modes
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_SPEAKER, OUTPUT); 

  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_SONAR_ECHO, INPUT);
  
  // Make sure the LED is off
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  LOG_TASK_SUCCESS;

  LOG_TASK("Checking check_calibration");
  CHECK_STATUS(check_calibration(), "Calibration error!");

  LOG_INFO("STARTING...");
}

void loop()
{
  LOG_INFO("Waiting for user...");
  wait_for_button();

  LOG_INFO("--STARTING ASCENT--");
  wait_for_altitude();

  LOG_INFO("--REACHED TARGET ALTITUDE--");
  LOG_INFO("Pinging...");
  ping(DESCENT_START_NOTE);

  LOG_INFO("Starting photo taking...");
  start_photo_taking();

  LOG_INFO("Landed! Waiting for user input.");
  while (digitalRead(PIN_BUTTON) == HIGH) ping(LANDED_NOTE); // Hehe this might get annoying
  
  LOG_INFO("Stopping beeps. Have a great day! (Jk kill yourself)");
  while (1); // The button press is just to stop the pinging. After this, the user has to reset the Arduino
}





// ---ALTIMITER CODE---
double get_altitude(void);

// NOTE:
// A4 -> SCL
// A5 -> SDA
void wait_for_altitude(void)
{
  do 
  {
    get_altitude();

    #ifdef DEBUG 
    Serial.print("Tmp: ");
    Serial.print(temp);
    Serial.print(", prs: ");  
    Serial.print(pressure);
    Serial.print(", init: ");  
    Serial.print(starting_point_pressure);
    Serial.print(", alt: ");  
    Serial.print(altitude);
    Serial.print(", trgt: ");  
    Serial.print(TARGET_ALTITUDE);
    #endif // DEBUG
  }
  while (altitude > TARGET_ALTITUDE);
  
}

char start_readings(void)
{ 
  status = barometer.startTemperature();
  if (status == 0) return 0.0;
  delay(status);
  status = barometer.getTemperature(temp);
  if (status == 0) return 0.0;
  
  status = barometer.startPressure(PRESSURE_OVERSAMPLING);
  if (status == 0) return 0.0;
  delay(status);
  status = barometer.getPressure(starting_point_pressure, temp);
  if (status == 0) return 0.0;
  
  return 1.0;
}

double get_altitude(void)
{
  status = barometer.startTemperature();
  // If this fails... try, try again
  if (status == 0) return -1.0;
  // According to the library, the `startTemperature` and `startPressure`
  // methods return an ammount of time to delay, which is always 5ms.
  delay(status);

  // This actually passes in a pointer to
  // temp??? C++ is so weird.
  status = barometer.getTemperature(temp);
  if (status == 0) return -1.0;
  
  // Repeat but with pressure
  status = barometer.startPressure(PRESSURE_OVERSAMPLING);
  if (status == 0) return -1.0;
  delay(status);
  status = barometer.getPressure(pressure, temp);
  // Resisting the urge to just
  // write a macro for this rn
  if (status == 0) return -1.0;
    
  // This just does some funny math so no status is returned.
  altitude = barometer.altitude(pressure, starting_point_pressure);
  return altitude;
}






// ---BUTTON CODE---
void wait_for_button(void)
{
   while (digitalRead(PIN_BUTTON) == HIGH); // Wait for button to be pressed
   while (digitalRead(PIN_BUTTON) == LOW); // Wait for button to be let go    
}





// ---SERVO MOTOR CODE---
void start_photo_taking(void)
{
  double dist;
  #ifdef DEBUG
  size_t count = 0;
  #endif // DEBUG
  do {
    // There's no multithreading :(
    delay(200);
    camera_pusher_2000.write(SERVO_START);
    delay(200);
    camera_pusher_2000.write(SERVO_END);

    dist = ping_sonar();
    #ifdef DEBUG
    count++;
    Serial.print("Cycle ");
    Serial.print(count);
    Serial.print(". Dist:");
    Serial.print(dist);
    Serial.println("cm");
    #endif // DEBUG

  } while (dist > LANDING_RANGE);

  #ifdef DEBUG
  Serial.print("Reached target after ");
  Serial.print(count);
  Serial.println(" cycles.");
  #endif // DEBUG
}





// ---SONAR CODE---
char check_calibration(void)
{
    double should_be_zero = ping_sonar();
    
    // If it's less than 10cm, good enough
    if (should_be_zero < 10.0) return 1;
    else return 0;  
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
  // Turn on the LED
  digitalWrite(PIN_BUTTON, HIGH);
  
  // Play lil beep :-)
  tone(PIN_SPEAKER, pitch, 150);
  delay(160);                
  tone(PIN_SPEAKER, pitch, 150);
  delay(800);
}
