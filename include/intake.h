#ifndef INTAKE_H
#define INTAKE_H
#include <vex.h>
#include <memory>

class Intake{
public:
	Intake();
	void Spin(double pctspeed, bool reverse);

private:
	std::unique_ptr<vex::motor> Intake1;
	std::unique_ptr<vex::motor> Intake2;

};

#endif