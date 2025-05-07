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
#define PIN_BUTTON 3
#define PIN_SPEAKER 4
#define PIN_LED 5
#define PIN_SONAR_TRIG 8
#define PIN_SONAR_ECHO 9
#define PIN_SERVO 10

// As soon as the Arduino detects it's 
// this far from the ground (in cm), it will
// have considered itself as landed.  
#define LANDING_RANGE 1000

// Our target altitude, measured by
// pressure, in hPa.
#define TARGET_ALTITUDE 1013.25

// C in octave 3
#define DESCENT_START_NOTE 131
// E in octave 6
#define LANDED_NOTE 1319
// F in octave 1
#define ERROR_NOTE 44

#endif // !VARS_HGUARD
