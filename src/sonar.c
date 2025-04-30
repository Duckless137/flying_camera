#include <Arduino.h>
#include "../vars.h"

void check_calibration(void)
{
    
}

void wait_for_landing(void) {
        
}


double ping_sonar(void) {
    digitalWrite(PIN_SONAR_TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONAR_TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_TRIG,LOW);
    int duration = pulseIn(PIN_SONAR_ECHO, HIGH, 0);
    double distance = duration*0.034/2;
    return distance;
}
