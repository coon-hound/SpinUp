#include "control.h"
#include "port_config.h"
#include <vex.h>

using namespace vex;

int control() {
  //initialization
  //brain
  brain Brain;
  //controller
  controller Controller = controller(primary);
  //motors
  motor leftMotor1 = motor(PORT1, ratio18_1, true);
  motor leftMotor2 = motor(PORT2, ratio18_1, true);
  motor rightMotor1 = motor(PORT3, ratio18_1, false);
  motor rightMotor2 = motor(PORT4, ratio18_1, false);
  //motor groups
  motor_group LeftDriveSmart = 
  motor_group(leftMotor1, leftMotor2);
  motor_group RightDriveSmart = 
  motor_group(rightMotor1, rightMotor2);

  // motor_group SideLeft =
  // motor_group(leftMotor1, rightMotor1);
  // motor_group SideRight =
  // motor_group(leftMotor2, rightMotor2);
  //drive variables
  double drivetrainLeftSideSpeed, drivetrainRightSideSpeed;
  double horizontalSpeed;
  while (true) {
    drivetrainLeftSideSpeed = (double) Controller.Axis3.position();
    drivetrainRightSideSpeed = (double) Controller.Axis2.position();
    horizontalSpeed = (double) Controller.Axis4.position();
    LeftDriveSmart.spin(fwd, drivetrainLeftSideSpeed, percentUnits::pct);
    RightDriveSmart.spin(fwd, drivetrainRightSideSpeed, percentUnits::pct);
    // leftMotor1.spin(fwd, horizontalSpeed, percentUnits::pct);
    // leftMotor2.spin(fwd, 0-horizontalSpeed, percentUnits::pct);
    // rightMotor1.spin(fwd, 0-horizontalSpeed, percentUnits::pct);
    // rightMotor2.spin(fwd, horizontalSpeed, percentUnits::pct);


    /*
     * to do: implement horizontal movement (direct left and right without turning)
     * reference image: https://seas.yale.edu/sites/default/files/imce/other/HolonomicOmniWheelDrive.pdf (page 13)
     * basic idea:
     * "rotate" the holonomic drive 90˚ and treat each wheel as the image of the rotation. 
     * modify each motor individually without motor groups
     *
     * to go left, FL wheel spin back, BL wheel spin forward, FR wheel spin forward, BL wheel spin back
     * to go right, FL wheel spin forward, BL wheel spin back, FR wheel spin back, BL wheel spin forward
    */
  }
  return 0;
}