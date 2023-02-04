#include "control.h"
#include "port_config.h"
#include <vex.h>
#include <unistd.h>

using namespace vex;

int control() {
  //initialization

  //controller
  controller Controller = controller(primary);
  //motors
  motor LeftMotor1(LEFT_MOTOR1, ratio18_1, true);
  motor LeftMotor2(LEFT_MOTOR2, ratio18_1, true);
  motor RightMotor1(RIGHT_MOTOR1, ratio18_1, false);
  motor RightMotor2(RIGHT_MOTOR2, ratio18_1, false);

  motor Intake1(INTAKE1, ratio18_1, true);
  motor Intake2(INTAKE2, ratio18_1, false);

  double intakespeed = 0.0;
  double div = 1.0;

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

    //more precise movement
    if (Controller.ButtonY.pressing()) {
      div = 20.0;
    }
    if (Controller.ButtonB.pressing()) {
      div = 1.0;
    }

    //configure motor speeds
    left_motor1_speed = (axis3_value + axis4_value + axis1_value) / div;
    left_motor2_speed = (axis3_value - axis4_value + axis1_value) / div;
    right_motor1_speed = (axis3_value - axis4_value - axis1_value) / div;
    right_motor2_speed = (axis3_value + axis4_value - axis1_value) / div;

    //spin motor
    LeftMotor1.spin(fwd, left_motor1_speed, percentUnits::pct);
    LeftMotor2.spin(fwd, left_motor2_speed, percentUnits::pct);
    RightMotor1.spin(fwd, right_motor1_speed, percentUnits::pct);
    RightMotor2.spin(fwd, right_motor2_speed, percentUnits::pct);


    //intake 
    if(Controller.ButtonA.pressing() && intakespeed < 100) {
      intakespeed += 0.025;
    } else if (intakespeed > 0) {
      intakespeed -= 0.05;
    }
    if (Controller.ButtonX.pressing() && intakespeed > -100) {
      intakespeed -= 0.025;
      // Intake.spin(fwd, intakespeed, percentUnits::pct);
    } else if (intakespeed < 0) {
      intakespeed += 0.005;
    }

    Intake1.spin(fwd, intakespeed, percentUnits::pct);
    Intake2.spin(fwd, intakespeed, percentUnits::pct);
    brain Brain;
    Brain.Screen.print("%lf\n", intakespeed);
  }
  return 0;
}