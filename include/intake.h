#ifndef INTAKE_H
#define INTAKE_H
#include <vex.h>
#include <memory>
#include <thread>
#include "time.h"


class Intake {
friend void _spin(double pctspeed, double time);
public:
	Intake(int PORT1, int PORT2);
	void Spin(double pctspeed, double time);
private:
	std::unique_ptr<vex::motor> Intake1;
	std::unique_ptr<vex::motor> Intake2;
};

#endif
