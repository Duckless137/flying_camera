/* Don't question this way of handling the inclusion
 * of files in the src directory... linking them externally
 * is almost impossible with the way arduino-cli works, and
 * don't even get me started on how arduino-ide does it...
 * Is there a way to link them as precompiled object files? 
 * I'm sure there is. Can I figure out how? Nah.
 */
#ifndef LAYOUT_HGUARD
#define LAYOUT_HGUARD
#include "src/altimiter.c"
#include "src/button.c"
#include "src/servo_motor.c"
#include "src/sonar.c"
#include "src/speaker.c"

void check_calibration(void);
void wait_for_button(void);
void wait_for_altitude(void);
void ping(int pitch);
void start_photo_taking(void);
void wait_for_landing(void);
#endif // !LAYOUT_HGUARD
