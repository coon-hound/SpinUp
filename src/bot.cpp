#include "bot.h"
#include "port_config.h"
#include <iostream>
#include <vex.h>
#include <chrono>
#include <math.h>
using namespace vex;

// controller Controller = controller(primary);

// gps BotGps(GPS, 0, left);

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

double Bot::getSine(double angle)
{	
	return sin(angle / 180 * M_PI);
}
double Bot::getCosine(double angle)
{
	return cos(angle / 180 * M_PI);
}

void Bot::AdjustHeading(double x, double y, double degree) 
{
  
	// gets robot state
	double relativeX = 0-(x - BotGps.xPosition(mm)); //
	double relativeY = 0-(y - BotGps.yPosition(mm)); //
	angleError = degree - BotGps.heading(deg); //
	currentHeading = BotGps.heading(deg) - 45; //
	
	// necessary trig functions
	sine = getSine(currentHeading); //
	cosine = getCosine(currentHeading); //

	// matrix calculation
	orthogonal1 = (cosine * relativeX) + (sine * relativeY); //
	orthogonal2 = (cosine * relativeY) - (sine * relativeX); //

	// PD Controller for axis displacement
 	proportional1 = orthogonal1 * kP; 
	proportional2 = orthogonal2 * kP; 
	derivative1 = (orthogonal1 - lastError1) * kD; 
	derivative2 = (orthogonal2 - lastError2) * kD; 
	orthogonal1Speed = proportional1 + derivative1; 
	orthogonal2Speed = proportional2 + derivative2;
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

	RightMotor1Speed = 0 - orthogonal1Speed;
	LeftMotor2Speed = 0 - orthogonal1Speed;
	LeftMotor1Speed = orthogonal2Speed;
	RightMotor2Speed = orthogonal2Speed;

	LeftMotor1Speed -= turnSpeed;
	LeftMotor2Speed -= turnSpeed;
	RightMotor1Speed += turnSpeed;
	RightMotor2Speed += turnSpeed;

	std::cout << "Coords: " << BotGps.xPosition(mm) << ", " << BotGps.yPosition(mm) << "\n";
	vexDelay(1);
	std::cout << "Heading: " << BotGps.heading(deg) << "\n";
	vexDelay(1);
	std::cout << "Relative X: " << relativeX << " Relative Y: " << relativeY << "\n";
	vexDelay(1);
	std::cout << "Angle Error: " << angleError << "\n";
	vexDelay(1);
	std::cout << "Current heading: " << currentHeading << "\n";
	vexDelay(1);
	std::cout << "O1speed: " << orthogonal1Speed << " O2speed: " << orthogonal2Speed << " turnspeed: " << turnSpeed << "\n";
	vexDelay(1);
	std::cout << "O1Error: " << orthogonal1 << " O2Error: " << orthogonal2 << " AngleError: " << angleError << "\n\n";
	vexDelay(50);
	Brain.Screen.print("%lf %lf", BotGps.xPosition(mm), BotGps.yPosition(mm));
	if (BotGps.xPosition(mm) == 0.0 and BotGps.yPosition(mm) == 0.0)
	{
		Brain.Screen.print("GPS is reading 0, 0. Reboot the brain.");
	}
	Brain.Screen.clearScreen();
}



void Bot::Spin() 
{
 	LeftMotor1.spin(fwd, LeftMotor1Speed, pct);
 	LeftMotor2.spin(fwd, LeftMotor2Speed, pct);
 	RightMotor1.spin(fwd, RightMotor1Speed, pct);
 	RightMotor2.spin(fwd, RightMotor2Speed, pct);
}

void Bot::Move(double x, double y, double angle, double lengthTolerance = 100, double angleTolerance = 10, double tickLength = 0) 
{
	double askdjfkj = BotGps.xPosition(mm);
	double fiuqiowie = BotGps.yPosition(mm);
	double ajfjqrwuer = BotGps.heading(deg);
	std::cout << askdjfkj << fiuqiowie << ajfjqrwuer << " Done calibrating\n\n";
	

	lastAngleError = angle - BotGps.heading(deg);
	double initialcos = cos((BotGps.heading(deg) - 45) / 180 * M_PI), initialsin = sin((BotGps.heading(deg) - 45) / 180 * M_PI);
	lastError1 = (initialcos * (y - BotGps.yPosition(mm))) - (initialsin * (x - BotGps.xPosition(mm)));
	lastError2 = (initialsin * (y - BotGps.yPosition(mm))) + (initialcos * (x - BotGps.xPosition(mm)));
	while (Abs(BotGps.xPosition(mm) - x) > lengthTolerance || 
		   Abs(BotGps.yPosition(mm) - y) > lengthTolerance || 
		   Abs(BotGps.heading(deg) - angle) > angleTolerance) 
	{
		AdjustHeading(x, y, angle);
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
	Move(BotGps.xPosition(mm), BotGps.yPosition(mm), BotGps.heading(deg) + angle, 25, 1, 20);
}

void Bot::SetHeading(double angle) 
{
	Move(BotGps.xPosition(mm), BotGps.yPosition(mm), angle, 25, 1, 20);
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
		Intake1.spin(fwd, -100, pct);
		Intake2.spin(fwd, -100, pct);
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
  LeftMotor1.spin(fwd, -10, pct);
  LeftMotor2.spin(fwd, -10, pct);
  RightMotor1.spin(fwd, -10, pct);
  RightMotor2.spin(fwd, -10, pct);
	while (Abs(Intake1.position(deg) - 180) > 5 and Abs(Intake2.position(deg) - 180) > 5)
	{
		vexDelay(10);
	}
	Intake1.stop();
	Intake2.stop();
}

void Bot::Intake()
{
	// intake speed -100
	unsigned long long start = timeKeeper.Now();
	unsigned long long end = start + 6000ULL;
	unsigned long long current = start;
	while (current < end)
	{
		current = timeKeeper.Now();
		Intake1.spin(fwd, -100, pct);
		Intake2.spin(fwd, -100, pct);
		LeftMotor1.spin(fwd, 25, pct);
		LeftMotor2.spin(fwd, 25, pct);
		RightMotor1.spin(fwd, 25, pct);
		RightMotor2.spin(fwd, 25, pct);
	}
	Intake1.stop();
	Intake2.stop();
	LeftMotor1.stop();
	LeftMotor2.stop();
	RightMotor1.stop();
	RightMotor2.stop();
}