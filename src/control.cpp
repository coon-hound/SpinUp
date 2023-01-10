#include "control.h"
#include "port_config.h"
#include <vex.h>

using namespace vex;

int control() {
  //initialization

  //controller
  controller Controller = controller(primary);
  //motors
  motor LeftMotor1 = motor(LEFT_MOTOR1, ratio18_1, true);
  motor LeftMotor2 = motor(LEFT_MOTOR2, ratio18_1, true);
  motor RightMotor1 = motor(RIGHT_MOTOR1, ratio18_1, false);
  motor RightMotor2 = motor(RIGHT_MOTOR2, ratio18_1, false);

  //drive variables

  double left_motor1_speed, left_motor2_speed;
  double right_motor1_speed, right_motor2_speed;

  //main drive loop
  while (true) {

    //drive forward backward
    double axis3_value = 1.0 * Controller.Axis3.position();

    //turn left right
    double axis1_value = 1.0 * Controller.Axis1.position();

    //drive left right
    double axis4_value = 1.0 * Controller.Axis4.position();

    //configure motor speeds
    left_motor1_speed = axis3_value + axis4_value + axis1_value;
    left_motor2_speed = axis3_value - axis4_value + axis1_value;
    right_motor1_speed = axis3_value - axis4_value - axis1_value;
    right_motor2_speed = axis3_value + axis4_value - axis1_value;

    //spin motor
    LeftMotor1.spin(fwd, left_motor1_speed, percentUnits::pct);
    LeftMotor2.spin(fwd, left_motor2_speed, percentUnits::pct);
    RightMotor1.spin(fwd, right_motor1_speed, percentUnits::pct);
    RightMotor2.spin(fwd, right_motor2_speed, percentUnits::pct);
  }
  return 0;
}