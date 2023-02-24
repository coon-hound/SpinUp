#ifndef BASE_H
#define BASE_H
#include "port_config.h"
#include <vex.h>
#include <memory>

using namespace vex;

class Bot 
{
private:
	//PD controller constants (need to be tuned)
	const double kP_dist = 45, kD_dist = 12; // tune value
	const double kP_angle = 45, kD_angle = 8; // tune value

	// devices
	motor LeftMotor1 = motor(LEFT_MOTOR1, ratio18_1, true); // Orthogonal 1
	motor LeftMotor2 = motor(LEFT_MOTOR2, ratio18_1, true); // Orthogonal 2
	motor RightMotor1 = motor(RIGHT_MOTOR1, ratio18_1, false); // Orthogonal 2
	motor RightMotor2 = motor(RIGHT_MOTOR2, ratio18_1, false); // Orthogonal 1

	motor Flywheel1 = motor(FLYWHEEL1, ratio18_1, true);
	motor Flywheel2 = motor(FLYWHEEL2, ratio18_1, false);

	motor Intake1 = motor(INTAKE1, ratio18_1, true);
	motor Intake2 = motor(INTAKE2, ratio18_1, false);

	gps Gps = gps(GPS, 0, left);

	double LeftMotor1Speed, LeftMotor2Speed, RightMotor1Speed, RightMotor2Speed;

	// drive variables
	double orthogonal1, orthogonal2; // defining orthogonal axis values
	double orthogonal1Speed, orthogonal2Speed; // orthogonal axis speeds
	double theta, sine, cosine; // defining angle values
	double lastError1, lastError2; // defining derivative values
	double proportional1, derivative1, proportional2, derivative2; // PD controller variables

	//angle variables
	double angleError;
	double lastAngleError;
	double proportionalAngle, derivativeAngle;
	double turnSpeed;

    double Abs(double k);
	// void AdjustHeading(double x, double y, double degree, distanceUnits lengthUnit, rotationUnits angleUnit);
	void Spin();

public:
	Bot();
	void Move(double x, double y, double angle, double lengthTolerance, double angleTolerance, 
			  double tickLength, distanceUnits lengthUnit, rotationUnits angleUnit);
	void Turn(double angle);
	void SetHeading(double angle);
	void Shoot(int seconds);
	void Roll();
};

#endif
