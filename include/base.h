#ifndef BASE_H
#define BASE_H
#include "time.h"
#include <vex.h>
#include <thread>
#include <memory>
#include "intake.h"
#include "flywheel.h"

class Base {
public:
	Base(int PORT1, int PORT2, int PORT3, int PORT4);
	void Goto(int x, int y, double pctspeed);
	void SetHeading(double angle);
	void Turn(double angle);
	void Shoot();
private:
	
	std::unique_ptr<vex::motor> LeftMotor1;
	std::unique_ptr<vex::motor> LeftMotor2;
	std::unique_ptr<vex::motor> RightMotor1;
	std::unique_ptr<vex::motor> RightMotor2;
};

#endif
