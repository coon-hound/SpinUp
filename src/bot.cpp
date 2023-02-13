#include "time.h"
#include "bot.h"
#include "port_config.h"
#include <vex.h>
#include <math.h>
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
	double initial1, initial2; // initial distance for PD controller

	//angle variables
	double initialAngle; // initial angle
	double angleError;
	double lastAngleError;
	double proportionalAngle, derivativeAngle;
	double turnSpeed;

	//PD controller constants
	const double kP = 0.1, kD = 0.1; 
	double abs(double k) {
		if (k > 0) return k;
		return -k;
	}
public:
	void adjustHeading(double x, double y, double degree) {
		theta = Gps.heading(rotationUnits::deg) * 3.1415926 / 180;
		sine = sin(theta);
		cosine = cos(theta);
		orthogonal1 = (cosine * x) - (sine * y);
		orthogonal2 = (sine * x) + (cosine * y);
		// PD controller needs work
		proportional1 = orthogonal1 * kP; // PD controller needs work
		proportional2 = orthogonal2 * kP; // PD controller needs work
		derivative1 = (orthogonal1 - lastError1) * kD; // PD controller needs work
		derivative2 = (orthogonal2 - lastError2) * kD; // PD controller needs work
		orthogonal1Speed = proportional1 + derivative1; // PD controller needs work
		orthogonal2Speed = proportional2 + derivative2; // PD controller needs work
		orthogonal1Speed /= initial1; // PD controller needs work
		orthogonal2Speed /= initial2; // PD controller needs work
		
		angleError = theta - (degree * 180 / 3.1415926);
		// PD controller needs work
		derivativeAngle = (angleError - lastAngleError) * kD; // PD controller needs work
		proportionalAngle = angleError * kP; // PD controller needs work
		turnSpeed = proportionalAngle + derivativeAngle; // PD controller needs work
		turnSpeed /= initialAngle; // PD controller needs work

		lastAngleError = angleError;
		lastError1 = orthogonal1;
		lastError2 = orthogonal2;
	}

	void spin() {
		// orthogonal1 axis
		LeftMotor1Speed = orthogonal1Speed + turnSpeed;
		RightMotor2Speed = orthogonal1Speed - turnSpeed;
		// orthogonal2 axis
		LeftMotor2Speed = orthogonal2Speed + turnSpeed;
		RightMotor1Speed = orthogonal2Speed - turnSpeed;
		//spinning
		LeftMotor1.spin(directionType::fwd, LeftMotor1Speed, percentUnits::pct);
		LeftMotor2.spin(directionType::fwd, LeftMotor2Speed, percentUnits::pct);
		RightMotor1.spin(directionType::fwd, RightMotor1Speed, percentUnits::pct);
		RightMotor2.spin(directionType::fwd, RightMotor2Speed, percentUnits::pct);
	}

	void move(double x, double y, double angle) {
		initialAngle = lastAngleError = Gps.heading(rotationUnits::deg) * 3.1415926 / 180;
		double initialcos = cos(initialAngle), initialsin = sin(initialAngle);
		initial1 = lastError1 = (initialcos * x) - (initialsin * y);
		initial2 = lastError2 = (initialsin * x) + (initialcos * y);
		while (!(abs(Gps.xPosition()-x) > 0.05 && abs(Gps.yPosition()-y) > 0.05 && abs(Gps.heading()-angle) > 0.05)) {
			adjustHeading(x, y, angle);
			spin();
		}
	}

	void turn(double angle) {
		double change = angle - Gps.heading(rotationUnits::deg);
		move(Gps.xPosition(), Gps.yPosition(), change);
	}

	void setHeading(double angle) {
		move(Gps.xPosition(), Gps.yPosition(), angle);
	}

	void shoot() {

	}
};
