#include "base.h"
#include "port_config.h"
#include <vex.h>
using namespace vex;

Base::Base()
{
	*LeftMotor1 = motor(LEFT_MOTOR1, ratio18_1, true);
	*LeftMotor2 = motor(LEFT_MOTOR2, ratio18_1, true);
	*RightMotor1 = motor(RIGHT_MOTOR1, ratio18_1, false);
	*RightMotor2 = motor(RIGHT_MOTOR2, ratio18_1, false);
}

void Base::Goto(int x, int y, double pctspeed)
{
	
}

void Base::Rotate(double angle)
{

}

void Base::SetHeading(double angle)
{

}