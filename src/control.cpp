#include "control.h"
#include "port_config.h"
#include <vex.h>

using namespace vex;

int control()
{
  controller c(primary);

  // initialize motors
  motor left(LEFT_MOTOR, ratio18_1, false);
  motor right(RIGHT_MOTOR, ratio18_1, true);

  int leftspeed = 0, rightspeed = 0;
  while (true) {
    int axis3_value = c.Axis3.position();
    int axis1_value = c.Axis1.position();
    leftspeed = axis3_value+axis1_value;
    rightspeed = axis3_value-axis1_value;
    left.spin(directionType::fwd, leftspeed, percentUnits::pct);
    right.spin(directionType::fwd, rightspeed, percentUnits::pct);
  }
  return 0;
}
