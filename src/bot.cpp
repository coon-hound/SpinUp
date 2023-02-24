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

namespace _internal
{
	double getSine(double angle)
	{
		return sin(angle / 180 * M_PI);
	}
	double getCosine(double angle)
	{
		return cos(angle / 180 * M_PI);
	}
}

void Bot::AdjustHeading(double x, double y, double degree, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg) 
{
	// gets robot state
	double relativeX = x - Gps.xPosition(lengthUnit);
	double relativeY = y - Gps.yPosition(lengthUnit);

	angleError = degree - Gps.heading(angleUnit);
	heading = Gps.heading() - 45;
	
	// necessary trig functions
	sine = _internal::getSine(heading);
	cosine = _internal::getCosine(heading);

	// matrix calculation
	orthogonal1 = (cosine * relativeY) - (sine * relativeX);
	orthogonal2 = (cosine * relativeX) + (sine * relativeY);

	// PD Controller for axis displacement
 	proportional1 = orthogonal1 * kP; 
	proportional2 = orthogonal2 * kP; 
	derivative1 = (orthogonal1 - lastError1) * kD; 
	derivative2 = (orthogonal2 - lastError2) * kD; 
	orthogonal1Speed = proportional1 + derivative1; 
	orthogonal2Speed = proportional2 + derivative2;
	// orthogonal1Speed = proportional1;
	// orthogonal2Speed = proportional2; 
	
	// PD Controller for angle displacement
	proportionalAngle = angleError * kP_angle; 
	derivativeAngle = (angleError - lastAngleError) * kD_angle;
	turnSpeed = proportionalAngle + derivativeAngle; 
	// turnSpeed = proportionalAngle;

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
	std::cout << "Coords: " << Gps.xPosition() << ", " << Gps.yPosition() << "\n";
	std::cout << "X: " << relativeX << " Y: " << relativeY << "\n";
	std::cout << "Angle: " << angleError << "\n";
	std::cout << "O1speed: " << orthogonal1Speed << " O2speed: " << orthogonal2Speed << " turnspeed: " << turnSpeed << "\n";
	std::cout << "O1: " << orthogonal1 << " O2: " << orthogonal2 << " Angle: " << angleError << "\n\n";
}



void Bot::Spin() 
{
	LeftMotor1.spin(fwd, LeftMotor1Speed, pct);
	LeftMotor2.spin(fwd, LeftMotor2Speed, pct);
	RightMotor1.spin(fwd, RightMotor1Speed, pct);
	RightMotor2.spin(fwd, RightMotor2Speed, pct);
}

void Bot::Move(double x, double y, double angle, double lengthTolerance = 100, double angleTolerance = 10, 
			   double tickLength = 0, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg) 
{
	lastAngleError = angle - Gps.heading(deg);
	double initialcos = cos((Gps.heading(deg) - 45) / 180 * M_PI), initialsin = sin((Gps.heading(deg) - 45) / 180 * M_PI);
	lastError1 = (initialcos * (y - Gps.yPosition())) - (initialsin * (x - Gps.xPosition()));
	lastError2 = (initialsin * (y - Gps.yPosition())) + (initialcos * (x - Gps.xPosition()));
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