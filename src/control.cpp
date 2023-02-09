#include "control.h"
#include "port_config.h"
#include <algorithm>
#include <cmath>
#include <vex.h>
#include <unistd.h>

using namespace vex;

double throttlefactor(double tmp, const double maximumtmp) { 
  return sqrt(1 - (pow(tmp, 10) / pow(maximumtmp, 10)));
}

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

  motor Flywheel1(FLYWHEEL1, ratio18_1, true);
  motor Flywheel2(FLYWHEEL2, ratio18_1, false);

  gps Gps(GPS, 0, turnType::right);



  double intakespeed = 0.0;
  double flywheelspeed = 0.0;
  double div = 1.0;
  double axis3_value, axis1_value, axis4_value;
  double botx, boty;
  botx = Gps.xPosition();
  boty = Gps.yPosition();

  //drive variables

  double left_motor1_speed, left_motor2_speed;
  double right_motor1_speed, right_motor2_speed;

  double tmp[20];
  const double maxTmp = 100;
  double maxMotorTmp, maxIntakeTmp, maxFlywheelTmp;
  for (int i = 0; i < 20; ++i) tmp[i] = 0;

  //main drive loop
  while (true) {
    //drive forward backward
    axis3_value = 1.0 * Controller.Axis3.position();

    //turn left right
    axis1_value = 1.0 * Controller.Axis1.position();

    //drive left right
    axis4_value = 1.0 * Controller.Axis4.position();

    //more precise movement
    if (Controller.ButtonY.pressing()) {
      div = 20.0;
    }
    if (Controller.ButtonB.pressing()) {
      div = 1.0;
    }

    //motor temperature sensing
    tmp[LEFT_MOTOR1]  = LeftMotor1.temperature(temperatureUnits::celsius);
    tmp[LEFT_MOTOR2]  = LeftMotor2.temperature(temperatureUnits::celsius);
    tmp[RIGHT_MOTOR1] = RightMotor1.temperature(temperatureUnits::celsius);
    tmp[RIGHT_MOTOR2] = RightMotor2.temperature(temperatureUnits::celsius);
    tmp[INTAKE1]      = Intake1.temperature(temperatureUnits::celsius);
    tmp[INTAKE2]      = Intake2.temperature(temperatureUnits::celsius);
    tmp[FLYWHEEL1]    = Flywheel1.temperature(temperatureUnits::celsius);
    tmp[FLYWHEEL2]    = Flywheel2.temperature(temperatureUnits::celsius);

    //configure motor speeds
    left_motor1_speed = (axis3_value + axis4_value + axis1_value) / div;
    left_motor2_speed = (axis3_value - axis4_value + axis1_value) / div;
    right_motor1_speed = (axis3_value - axis4_value - axis1_value) / div;
    right_motor2_speed = (axis3_value + axis4_value - axis1_value) / div;

    //throttling based on temperature
    maxMotorTmp = std::max(tmp[LEFT_MOTOR1], 
                  std::max(tmp[LEFT_MOTOR2], 
                  std::max(tmp[RIGHT_MOTOR1], tmp[RIGHT_MOTOR2])));
    left_motor1_speed *= throttlefactor(maxMotorTmp, maxTmp);
    left_motor2_speed *= throttlefactor(maxMotorTmp, maxTmp);
    right_motor1_speed *= throttlefactor(maxMotorTmp, maxTmp);
    right_motor2_speed *= throttlefactor(maxMotorTmp, maxTmp);
    // spin motor
    LeftMotor1.spin(fwd, left_motor1_speed, percentUnits::pct);
    LeftMotor2.spin(fwd, left_motor2_speed, percentUnits::pct);
    RightMotor1.spin(fwd, right_motor1_speed, percentUnits::pct);
    RightMotor2.spin(fwd, right_motor2_speed, percentUnits::pct);


    //intake speed management
    if(Controller.ButtonA.pressing() && intakespeed < 100) {
      intakespeed += 0.25;
    } else if (intakespeed > 0) {
      intakespeed -= 0.5;
    }
    if (Controller.ButtonX.pressing() && intakespeed > -100) {
      intakespeed -= 0.25;
      // Intake.spin(fwd, intakespeed, percentUnits::pct);
    } else if (intakespeed < 0) {
      intakespeed += 0.5;
    }
    //flywheel speed management
    if(Controller.ButtonUp.pressing() && flywheelspeed < 100) {
      flywheelspeed += 2.5;
    } else if (flywheelspeed > 0) {
      flywheelspeed -= 5;
    }

    //throttling based on temperature
    maxIntakeTmp = std::max(tmp[INTAKE1], tmp[INTAKE2]);
    maxFlywheelTmp = std::max(tmp[FLYWHEEL1], tmp[FLYWHEEL2]);
    intakespeed *= throttlefactor(maxIntakeTmp, maxTmp);
    flywheelspeed *= throttlefactor(maxFlywheelTmp, maxTmp);
    
    //spinning
	if (intakespeed >= 5.0) {
    	Intake1.spin(fwd, intakespeed, percentUnits::pct);
    	Intake2.spin(fwd, intakespeed, percentUnits::pct);
	}
    if (flywheelspeed >= 5.0) {
		Flywheel1.spin(fwd, flywheelspeed, percentUnits::pct);
    	Flywheel2.spin(fwd, flywheelspeed, percentUnits::pct);
	}
    //controller, brain alerts and information output
    if (maxIntakeTmp / maxTmp > 0.75) {
      Controller.Screen.print("ONE OR MORE BASE MOTORS OVERHEATING");
      Controller.Screen.newLine();
    }
    if (maxIntakeTmp / maxTmp > 0.75) {
      Controller.Screen.print("ONE OR MORE INTAKE MOTORS OVERHEATING");
      Controller.Screen.newLine();
    }
    if (maxFlywheelTmp / maxTmp > 0.75) {
      Controller.Screen.print("ONE OR MORE FLYWHEEL MOTORS OVERHEATING");
      Controller.Screen.newLine();
    }
    Controller.Screen.clearScreen();

  }
  return 0;
}
