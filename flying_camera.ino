#include <Arduino.h>
#include <Wire.h>
#include <vars.h>

// Function symbols
void check_calibration(void);
void wait_for_button(void);
void wait_for_altitude(void);
void ping(int pitch);
void start_photo_taking(void);
void wait_for_landing(void);

// Some other file already has a non-static
// function called `init`...
// You win some you lose some, I guess.
void im_going_to_murder_whoever_stole_the_init_symbol(void)
{
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_SPEAKER, OUTPUT);
  
  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_SONAR_ECHO, INPUT);
}

void setup()
{
  im_going_to_murder_whoever_stole_the_init_symbol();

  check_calibration();
  wait_for_button();
  wait_for_altitude();
  ping(DESCENT_START_NOTE);
  start_photo_taking();
  wait_for_landing();
  ping(LANDED_NOTE);
}

void loop()
{
  // Do nothing
}






// ---ALTIMITER CODE---
void wait_for_altitude(void)
{
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
    
}

// ---SONAR CODE---
void ping(int pitch);
static double ping_sonar(void);

void check_calibration(void)
{
    double should_be_zero = ping_sonar();
    
    // If it's less than 10cm, good enough
    if (should_be_zero < 10.0)
    {
        return;
    } else {
        ping(ERROR_NOTE);
        exit(10);
    }
}

void wait_for_landing(void) {
    while (ping_sonar() > LANDING_RANGE); // Dilly dally
}


double ping_sonar(void) {
    digitalWrite(PIN_SONAR_TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONAR_TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_TRIG,LOW);
    
    int duration = pulseIn(PIN_SONAR_ECHO, HIGH, 100UL);
    
    // Distance is in cm
    double distance = duration * 0.0343 / 2;
    return distance;
}

// ---SPEAKER CODE---
void ping(int pitch)
{
    tone(PIN_SPEAKER, pitch, 150);
    delay(160);                
	tone(PIN_SPEAKER, pitch, 150);
    delay(800);
}