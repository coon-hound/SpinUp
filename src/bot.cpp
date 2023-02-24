#include "bot.h"
#include "port_config.h"
#include <iostream>
#include <vex.h>
#include <chrono>
#include <cmath>
using namespace vex;
using namespace std;

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

void Bot::Spin() 
{
	LeftMotor1.spin(fwd, LeftMotor1Speed, pct);
	LeftMotor2.spin(fwd, LeftMotor2Speed, pct);
	RightMotor1.spin(fwd, RightMotor1Speed, pct);
	RightMotor2.spin(fwd, RightMotor2Speed, pct);
}

Bot::Bot()
{
	// Gps.setOrigin(0, 0, distanceUnits::mm);
}

void Bot::Move(double x, double y, double angle, double lengthTolerance = 25, double angleTolerance = 1, 
			   double tickLength = 20, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg) 
{
	cout << Gps.xPosition(lengthUnit);
	cout << Gps.yPosition(lengthUnit);
	while(1)
	{
		cout << "yes\n";
		cout << Gps.xPosition(lengthUnit) << " " << Gps.yPosition(lengthUnit) << "\n";
		// printf("%d %d\n", Gps.xPosition(lengthUnit), Gps.yPosition(lengthUnit));
		vexDelay(1000);
	}
	// angle += 1;
	double previous_x_error = 0;
	double previous_y_error = 0;
	double previous_angle_error = 0;
	angle *= (M_PI/180);
	while (Abs(Gps.xPosition(lengthUnit) - x) > lengthTolerance || 
		   Abs(Gps.yPosition(lengthUnit) - y) > lengthTolerance || 
		   Abs(Gps.heading(angleUnit) - angle) > angleTolerance) 
	{
		double curr_x = Gps.xPosition(lengthUnit);
		double curr_y = Gps.yPosition(lengthUnit);

		double e1_theta = Gps.heading(angleUnit) * (M_PI/180) - (M_PI/4);

		double new_x = (std::sin(e1_theta)*curr_x) - (std::cos(e1_theta) * y);
		double theta = Gps.heading(angleUnit) * (M_PI/180);
		double e2_theta = Gps.heading(angleUnit) * (M_PI/180) + (M_PI/4);
		double e1 [2] = {1*std::sin(e1_theta), 1*std::cos(e1_theta)};
		double e2 [2] = {1*std::sin(e2_theta), 1*std::cos(e2_theta)};
		double one_over_det = 1/(e1[0] * e2[1] - e2[0] * e1[1]);
		double x_diff = x - Gps.xPosition(lengthUnit);
		double y_diff = y - Gps.yPosition(lengthUnit);
		double deriv_y = kD_dist * (y_diff - previous_y_error);
		double deriv_x = kD_dist * (x_diff - previous_x_error);
		double theta_diff = angle - theta;
		double theta_output = kP_angle * theta_diff + kD_angle * (theta_diff - previous_angle_error);
		double e1_output = one_over_det * (e2[1] * x_diff - e2[0] * y_diff);
		double e2_output = one_over_det * (-e1[1] * x_diff + e1[0] * y_diff);
		LeftMotor1Speed = kP_dist * e2_output + deriv_x + deriv_y + theta_output;
		LeftMotor2Speed = kP_dist * e1_output + deriv_x + deriv_y + theta_output;
		RightMotor1Speed = kP_dist * e1_output + deriv_x + deriv_y - theta_output;
		RightMotor2Speed = kP_dist * e2_output + deriv_x + deriv_y - theta_output;
		cout << "curr x: " << Gps.xPosition(lengthUnit) << "\n";
		cout << "curr y: " << Gps.yPosition(lengthUnit) << "\n";
		cout << "curr angle: " << Gps.heading(angleUnit) << "\n";
		cout << endl;
		cout << "e1_output: " << e1_output << "\n";
		cout << "e2_output: " << e2_output << "\n";
		cout << endl;
		cout << "deriv_x: " << deriv_x << "\n";
		cout << "deriv_y: " << deriv_y << "\n";
		cout << endl;
		cout << "LeftM1 Speed: " << LeftMotor1Speed << "\n";
		cout << "LeftM2 Speed: " << LeftMotor2Speed << "\n";
		cout << "RightM1 Speed: " << LeftMotor1Speed << "\n";
		cout << "RightM2 Speed: " << RightMotor2Speed << "\n";
		cout << endl;
		cout << endl;
		Spin();
		previous_angle_error = theta_diff;
		previous_x_error = x_diff;
		previous_y_error = y_diff;
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