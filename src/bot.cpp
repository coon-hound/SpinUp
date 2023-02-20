#include "bot.h"
#include "port_config.h"
#include <iostream>
#include <vex.h>
#include <chrono>
#include <math.h>
using namespace vex;

controller Controller = controller(primary);

class Clock 
{
public:
	Clock() 
	{
		clockbirth = std::chrono::high_resolution_clock::now();
	}
	unsigned long long int Now() 
	{
		auto timenow = std::chrono::high_resolution_clock::now();
		auto timecast = std::chrono::duration_cast<std::chrono::milliseconds> (timenow - clockbirth);
		unsigned long long int time = timecast.count();
		return time;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> clockbirth;
} timeKeeper;

double Bot::Abs(double k) 
{
	if (k > 0) return k;
	return -k;
}

void Bot::AdjustHeading(double x, double y, double degree, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg) 
{
	// gets robot state
	double relativeX = x - Gps.xPosition(lengthUnit);
	double relativeY = y - Gps.yPosition(lengthUnit);
	theta = Gps.heading(angleUnit) * 3.1415926 / 180;
	
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
	derivativeAngle = (angleError - lastAngleError) * kD_angle;
	proportionalAngle = angleError * kP_angle; 
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

void Bot::Spin() 
{
	LeftMotor1.spin(fwd, LeftMotor1Speed, pct);
	LeftMotor2.spin(fwd, LeftMotor2Speed, pct);
	RightMotor1.spin(fwd, RightMotor1Speed, pct);
	RightMotor2.spin(fwd, RightMotor2Speed, pct);
}

void Bot::Move(double x, double y, double angle, double lengthTolerance = 25, double angleTolerance = 1, 
			   double tickLength = 20, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg) 
{
	lastAngleError = Gps.heading(deg) * 3.1415926 / 180;
	double initialcos = cos(lastAngleError), initialsin = sin(lastAngleError);
	lastError1 = (initialcos * x) - (initialsin * y);
	lastError2 = (initialsin * x) + (initialcos * y);
	while (Abs(Gps.xPosition(lengthUnit) - x) > lengthTolerance || 
		   Abs(Gps.yPosition(lengthUnit) - y) > lengthTolerance || 
		   Abs(Gps.heading(angleUnit) - angle) > angleTolerance) 
	{
		AdjustHeading(x, y, angle, lengthUnit, angleUnit);
		Spin();
		vexDelay(tickLength);
	}
	LeftMotor1.stop();
	RightMotor1.stop();
	LeftMotor2.stop();
	RightMotor2.stop();
}

void Bot::Turn(double angle) 
{
	Move(Gps.xPosition(mm), Gps.yPosition(mm), Gps.heading(deg) + angle, 25, 1, 20, mm, deg);
}

void Bot::SetHeading(double angle) 
{
	Move(Gps.xPosition(mm), Gps.yPosition(mm), angle, 25, 1, 20, mm, deg);
}

void Bot::Shoot(int seconds) 
{
	seconds *= 1000;
	auto start = timeKeeper.Now();
	auto current = start;
	while (current < start + 2000) 
	{
		current = timeKeeper.Now();
		Flywheel1.spin(fwd, 100, pct);
		Flywheel2.spin(fwd, 100, pct);
	}
	auto end = current + seconds;
	while (current < end) 
	{
		current = timeKeeper.Now();
		Flywheel1.spin(fwd, 100, pct);
		Flywheel2.spin(fwd, 100, pct);
		Intake1.spin(fwd, 100, pct);
		Intake2.spin(fwd, 100, pct);
	}
	Flywheel1.stop();
	Flywheel2.stop();
	Intake1.stop();
	Intake2.stop();
}

void Bot::Roll() 
{
	Intake1.setPosition(0, deg);
	Intake2.setPosition(0, deg);
	Intake1.spin(fwd, 50, pct);
	Intake2.spin(fwd, 50, pct);
	while (Abs(Intake1.position(deg) - 180) > 5 and Abs(Intake2.position(deg) - 180) > 5)
	{
		vexDelay(20);
	}
	Intake1.stop();
	Intake2.stop();
}