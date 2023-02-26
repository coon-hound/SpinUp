#ifndef BASE_H
#define BASE_H
#include "port_config.h"
#include <vex.h>
#include <memory>

using namespace vex;

class Bot 
{
friend int main();
private:
	//PD controller constants (need to be tuned)
	const double kP = 0.12, kD = 0.075; // tune value
	const double kP_angle = 1.75, kD_angle = 1; // tune value
	//const double kP = 0.1, kD = 0.25; // tune value
	//const double kP_angle = 0.05, kD_angle = 0.025; // tune value

	// devices
	brain Brain;
	motor LeftMotor1 = motor(LEFT_MOTOR1, ratio18_1, true); // Orthogonal 1
	motor LeftMotor2 = motor(LEFT_MOTOR2, ratio18_1, true); // Orthogonal 2
	motor RightMotor1 = motor(RIGHT_MOTOR1, ratio18_1, false); // Orthogonal 2
	motor RightMotor2 = motor(RIGHT_MOTOR2, ratio18_1, false); // Orthogonal 1

	motor Flywheel1 = motor(FLYWHEEL1, ratio18_1, true);
	motor Flywheel2 = motor(FLYWHEEL2, ratio18_1, false);

	motor Intake1 = motor(INTAKE1, ratio18_1, true);
	motor Intake2 = motor(INTAKE2, ratio18_1, false);

	gps BotGps = gps(GPS, 0, left);

	double LeftMotor1Speed, LeftMotor2Speed, RightMotor1Speed, RightMotor2Speed;

	// drive variables
	double orthogonal1, orthogonal2; // defining orthogonal axis values
	double orthogonal1Speed, orthogonal2Speed; // orthogonal axis speeds
	double angleError, currentHeading, sine, cosine; // defining angle values
	double lastError1, lastError2; // defining derivative values
	double proportional1, derivative1, proportional2, derivative2; // PD controller variables

	//angle variables
	double lastAngleError;
	double proportionalAngle, derivativeAngle;
	double turnSpeed;

    double Abs(double k);
	double getSine(double angle);
	double getCosine(double angle);
	void AdjustHeading(double x, double y, double degree);
	void Spin();
	void test() {
		LeftMotor1.spin(fwd, 10, pct);
		LeftMotor2.spin(fwd, 10, pct);
		RightMotor1.spin(fwd, 10, pct);
		RightMotor2.spin(fwd, 10, pct);
	}

public:
	void Move(double x, double y, double angle, double lengthTolerance, double angleTolerance, double tickLength);
	void Turn(double angle);
	void SetHeading(double angle);
	void Shoot(int seconds);
	void Roll();
	void Intake();
};

#endif
