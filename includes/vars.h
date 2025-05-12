/* Hey hardware team!
 * Feel free to edit the
 * values of anything in this
 * file to suit the hardware.
 * Ignore `VARS_HGUARD
 */
#ifndef VARS_HGUARD
#define VARS_HGUARD

// Here I've defined the pins 
// our parts are on.
#define PIN_BUTTON 13
#define PIN_SPEAKER 12
#define PIN_LED 11
#define PIN_SONAR_TRIG 2
#define PIN_SONAR_ECHO 3
#define PIN_SERVO 10

// As soon as the Arduino detects it's 
// this far from the ground (in cm), it will
// have considered itself as landed.  
#define LANDING_RANGE 1000


#define TARGET_ALTITUDE 90.0 // Target altitude in meters above sea level

// C in octave 3
#define DESCENT_START_NOTE 131
// E in octave 6
#define LANDED_NOTE 1319
// F in octave 1
#define ERROR_NOTE 44

// Number from 0-3 determining
// accuracy of altimiter
#define PRESSURE_OVERSAMPLING 2

#endif // !VARS_HGUARD
