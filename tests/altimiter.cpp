#define DEBUG

#include "HardwareSerial.h"
#include <Arduino.h>
#include <vars.h>
#include <SFE_BMP180.h>
#include <logging.h>

SFE_BMP180 barometer;
double get_altitude(void);
char start_readings(void);
void ping(int);
char status;
double temp, pressure, starting_point_pressure;
double altitude = 0.0;

void setup() {
  Serial.begin(9600);
  LOG_INFO("Serial started!");

  LOG_TASK("Starting barometer");
  CHECK_STATUS(barometer.begin(), "Failed to start barometer!");
 
  LOG_TASK("Getting initial pressure readings");
  CHECK_STATUS(start_readings(), "Failed to get inital pressure readings!");
}

void loop() {
  double res = get_altitude();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" Pressure: ");  
  Serial.print(pressure);
  Serial.print(" Sea level: ");  
  Serial.print(starting_point_pressure);
  Serial.print(" altitude: ");  
  Serial.println(altitude);


  delay(200);
}

char start_readings(void)
{ 
  status = barometer.startTemperature();
  if (!status) return 0.0;
  delay(status);
  status = barometer.getTemperature(temp);
  if (!status) return 0.0;
  
  status = barometer.startPressure(PRESSURE_OVERSAMPLING);
  if (!status) return 0.0;
  delay(status);
  status = barometer.getPressure(starting_point_pressure, temp);
  if (!status) return 0.0;
  
  return 1.0;
}

double get_altitude(void)
{
  status = barometer.startTemperature();
  if (!status) return -1.0;
  // According to the library, the `startTemperature` method
  // returns an ammount of time to wait... for some reason.
  delay(status);

  // This actually passes in a pointer to
  // temp??? C++ is so weird.
  status = barometer.getTemperature(temp);
  if (!status) return -1.0;
  
  // Repeat but with pressure
  status = barometer.startPressure(PRESSURE_OVERSAMPLING);
  if (!status) return -1.0;
  delay(status);
  status = barometer.getPressure(pressure, temp);
  if (!status) return -1.0;
      
  altitude = barometer.altitude(pressure, starting_point_pressure);
  return altitude;
}
