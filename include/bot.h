#ifndef BASE_H
#define BASE_H
#include "port_config.h"
#include <vex.h>
#include <memory>

using namespace vex;

class Bot {
private:
	// devices
	motor LeftMotor1 = motor(PORT1, ratio18_1, true); // Orthogonal 1
	motor LeftMotor2 = motor(PORT2, ratio18_1, true); // Orthogonal 2
	motor RightMotor1 = motor(PORT3, ratio18_1, false); // Orthogonal 2
	motor RightMotor2 = motor(PORT4, ratio18_1, false); // Orthogonal 1
	motor Flywheel1 = motor(FLYWHEEL1, ratio18_1, true);
	motor Flywheel2 = motor(FLYWHEEL2, ratio18_1, false);
	motor Intake1 = motor(INTAKE1, ratio18_1, true);
	motor Intake2 = motor(INTAKE2, ratio18_1, false);
	gps Gps = gps(GPS, 0, turnType::left);

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

	//PD controller constants
	const double kP = 45, kD = 10; 
	
    double Abs(double k);
    void AdjustHeading(double x, double y, double degree);
	void Spin();

public:
	void Move(double x, double y, double angle);
	void Turn(double angle);
	void SetHeading(double angle);
	void Shoot(int seconds);
};

#endif
