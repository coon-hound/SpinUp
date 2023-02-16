#include "bot.h"
#include "port_config.h"
#include <iostream>
#include <vex.h>
#include <chrono>
#include <math.h>
using namespace vex;

controller Controller = controller(primary);

class Clock {
public:
	Clock() {
		clockbirth = std::chrono::high_resolution_clock::now();
	}
	unsigned long long int Now() {
		auto timenow = std::chrono::high_resolution_clock::now();
		auto timecast = std::chrono::duration_cast<std::chrono::milliseconds> (timenow - clockbirth);
		unsigned long long int time = timecast.count();
		return time;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> clockbirth;
} globalClock;

double Bot::Abs(double k) {
	if (k > 0) return k;
	return -k;
}

void Bot::AdjustHeading(double x, double y, double degree) {
	// gets robot state
	double relativeX = x - Gps.xPosition();
	double relativeY = y - Gps.yPosition();
	theta = Gps.heading(rotationUnits::deg) * 3.1415926 / 180;
	
	// necessary trig functions
	sine = sin(theta);
	cosine = cos(theta);

	// matrix calculation
	orthogonal1 = (cosine * relativeX) + (sine * relativeY);
	orthogonal2 = - (sine * relativeX) + (cosine * relativeY);
	/*
	 * determinant for a rotation is 1 as the area contained in the unit square does not change
	 * reciprocal of the determinant is also 1
	 * a and d are the same; b and c just need to be negated
	*/

	// PD Controller for axis displacement
	proportional1 = orthogonal1 * kP; 
	proportional2 = orthogonal2 * kP; 
	derivative1 = (orthogonal1 - lastError1) * kD; 
	derivative2 = (orthogonal2 - lastError2) * kD; 
	orthogonal1Speed = proportional1 + derivative1; 
	orthogonal2Speed = proportional2 + derivative2; 
	
	// PD Controller for angle displacement
	angleError = theta - (degree * 180 / 3.1415926);
	derivativeAngle = (angleError - lastAngleError) * kD;
	proportionalAngle = angleError * kP; 
	turnSpeed = proportionalAngle + derivativeAngle; 

	// Updating Error values for the PD Controller
	lastAngleError = angleError;
	lastError1 = orthogonal1;
	lastError2 = orthogonal2;

	// Update orthogonal1 axis speeds
	LeftMotor1Speed = orthogonal1Speed + turnSpeed;
	RightMotor2Speed = orthogonal1Speed - turnSpeed;
	// Update orthogonal2 axis speeds
	LeftMotor2Speed = orthogonal2Speed + turnSpeed;
	RightMotor1Speed = orthogonal2Speed - turnSpeed;
}

void Bot::Spin() {
	LeftMotor1.spin(directionType::fwd, LeftMotor1Speed, percentUnits::pct);
	LeftMotor2.spin(directionType::fwd, LeftMotor2Speed, percentUnits::pct);
	RightMotor1.spin(directionType::fwd, RightMotor1Speed, percentUnits::pct);
	RightMotor2.spin(directionType::fwd, RightMotor2Speed, percentUnits::pct);
}

void Bot::Move(double x, double y, double angle, double tolerance) {
	lastAngleError = Gps.heading(rotationUnits::deg) * 3.1415926 / 180;
	double initialcos = cos(lastAngleError), initialsin = sin(lastAngleError);
	lastError1 = (initialcos * x) - (initialsin * y);
	lastError2 = (initialsin * x) + (initialcos * y);
	while (Bot::Abs(Gps.xPosition() - x) > tolerance or Bot::Abs(Gps.yPosition() - y) > tolerance or Bot::Abs(Gps.heading() - angle) > tolerance) {
		AdjustHeading(x, y, angle);
		Spin();
		vexDelay(1000);
	}
}

void Bot::Turn(double angle) {
	Move(Gps.xPosition(), Gps.yPosition(), Gps.heading(rotationUnits::deg) + angle);
}

void Bot::SetHeading(double angle) {
	Move(Gps.xPosition(), Gps.yPosition(), angle);
}

void Bot::Shoot(int seconds) {
	seconds *= 1000;
	unsigned long long start = globalClock.Now();
	unsigned long long current = start;
	while (current < start + 2000) {
		current = globalClock.Now();
		Flywheel1.spin(directionType::fwd, 100, percentUnits::pct);
		Flywheel2.spin(directionType::fwd, 100, percentUnits::pct);
	}
	unsigned long long end = current + seconds;
	while (current < end) {
		Flywheel1.spin(directionType::fwd, 100, percentUnits::pct);
		Flywheel2.spin(directionType::fwd, 100, percentUnits::pct);
		Intake1.spin(directionType::fwd, 100, percentUnits::pct);
		Intake2.spin(directionType::fwd, 100, percentUnits::pct);
	}
	Flywheel1.spin(directionType::fwd, 0, percentUnits::pct);
	Flywheel2.spin(directionType::fwd, 0, percentUnits::pct);
	Intake1.spin(directionType::fwd, 0, percentUnits::pct);
	Intake2.spin(directionType::fwd, 0, percentUnits::pct);
}