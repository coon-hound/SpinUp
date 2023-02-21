#include "control.h"
#include "port_config.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vex.h>
#include <unistd.h>

using namespace vex;

int control() 
{
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

  motor Flywheel1(FLYWHEEL1, ratio18_1, true);
  motor Flywheel2(FLYWHEEL2, ratio18_1, false);

  gps Gps(GPS, 0, turnType::right);

  double intakespeed = 0.0;
  double flywheelspeed = 0.0;
  double precisionFactor = 1.0;
  double axis3_value, axis1_value, axis4_value;

  //drive variables

  double left_motor1_speed, left_motor2_speed;
  double right_motor1_speed, right_motor2_speed;

  //main drive loop
  while (!Controller.ButtonLeft.pressing()) 
  {
    //drive forward backward
    axis3_value = 1.0 * Controller.Axis3.position();

    //turn left right
    axis1_value = 1.0 * Controller.Axis1.position();

    //drive left right
    axis4_value = 1.0 * Controller.Axis4.position();

    //more precisionFactor movement
    if (Controller.ButtonY.pressing()) 
    {
      precisionFactor = 20.0;
    }
    if (Controller.ButtonB.pressing()) 
    {
      precisionFactor = 1.0;
    }

    //configure motor speeds
    left_motor1_speed = (axis3_value + axis4_value + axis1_value) / precisionFactor;
    left_motor2_speed = (axis3_value - axis4_value + axis1_value) / precisionFactor;
    right_motor1_speed = (axis3_value - axis4_value - axis1_value) / precisionFactor;
    right_motor2_speed = (axis3_value + axis4_value - axis1_value) / precisionFactor;

    // spin motor
    LeftMotor1.spin(fwd, left_motor1_speed, pct);
    LeftMotor2.spin(fwd, left_motor2_speed, pct);
    RightMotor1.spin(fwd, right_motor1_speed, pct);
    RightMotor2.spin(fwd, right_motor2_speed, pct);

    //intake speed management
    if(Controller.ButtonR1.pressing()) 
    {
      intakespeed = 100;
    }

    if (Controller.ButtonR2.pressing()) 
    {
      intakespeed = -50;
      // Intake.spin(fwd, intakespeed, pct);
    } 

    if((!Controller.ButtonR1.pressing()) and (!Controller.ButtonR2.pressing()))
    {
      intakespeed = 0;
    }
      
    //flywheel speed management
    if(Controller.ButtonL1.pressing()) 
    {
      Flywheel1.spin(fwd, 100, pct);
      Flywheel2.spin(fwd, 90, pct);
    } 
    else 
    {
      Flywheel1.spin(fwd, 0, pct);
      Flywheel2.spin(fwd, 0, pct);
    }
    
    //spinning
    Intake1.spin(fwd, intakespeed, pct);
    Intake2.spin(fwd, intakespeed, pct);

    std::cout << intakespeed << " " << flywheelspeed << "\n";
    Controller.Screen.print("%lf %lf", intakespeed, flywheelspeed);
    Controller.Screen.clearScreen();

  }
  // vexDelay(1000);
  // if (Controller.ButtonLeft.pressing()) 
  // {
  //   // activate pneumatics
  //   pneumatics Rope(Brain.ThreeWirePort.B);
  //   Rope.open();
  // }
  return 0;
}
