#ifndef FLYWHEEL_H
#define FLYWHEEL_H
#include "time.h"
#include <vex.h>
#include <memory>
#include <thread>


class Flywheel {
friend void _spin(double pctspeed, double time);
public:
	Flywheel(int PORT1, int PORT2);
	void Spin(double pctspeed, double time);
private:
	std::unique_ptr<vex::motor> Flywheel1;
	std::unique_ptr<vex::motor> Flywheel2;
};

#endif
