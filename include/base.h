#ifndef BASE_H
#define BASE_H
#include <vex.h>
#include <memory>

class Base{
public:
	Base();
	void Goto(int x, int y, double pctspeed);
	void SetHeading(double angle);
	void Rotate(double angle);
private:
	std::unique_ptr<vex::motor> LeftMotor1;
	std::unique_ptr<vex::motor> LeftMotor2;
	std::unique_ptr<vex::motor> RightMotor1;
	std::unique_ptr<vex::motor> RightMotor2;
};


#endif