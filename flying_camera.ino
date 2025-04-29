#include "vars.h"
#include "layout.h"

void setup()
{
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
