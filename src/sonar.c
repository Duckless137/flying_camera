#include <Arduino.h>
#include "../vars.h"
#include "../layout.h"

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
        
}


double ping_sonar(void) {
    digitalWrite(PIN_SONAR_TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONAR_TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_TRIG,LOW);
    
    int duration = pulseIn(PIN_SONAR_ECHO, HIGH, 1000UL);
    
    // Distance is in cm
    double distance = duration * 0.0343 / 2;
    return distance;
}
