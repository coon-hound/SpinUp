#ifndef INTAKE_H
#define INTAKE_H
#include <vex.h>
#include <memory>
#include <thread>
#include "port_config.h"
#include "time.h"


class Intake{
public:
	Intake();
	void Spin(double pctspeed, double time, bool reverse);

private:
	std::unique_ptr<vex::motor> Intake1;
	std::unique_ptr<vex::motor> Intake2;

};

Intake::Intake() {
	*Intake1 = vex::motor(INTAKE1);
	*Intake2 = vex::motor(INTAKE2);
}

void accelerate(double fromspeed, double tospeed, vex::motor * Motor) {
	double speed = fromspeed;
	if (fromspeed > tospeed) {
		while (speed > tospeed) {
			speed -= 0.025;
			Motor.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
		}
		return;
	}
	while (speed < tospeed) {
		speed += 0.025;
		Motor.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
	}
}

void rotate(double speed, vex::motor * Motor, double time) {
	Clock clock;
	int start = clock.timeNow();
	int current = start, end = start + time;
	while (current < end) {
		Motor.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
		current = clock.timeNow();
	}
}

void spin(double speed, double time, vex::motor * Motor) {
	Clock clock;
	int start = clock.timeNow();
	int current, end = start + time;
	accelerate(0, speed, Motor);
	current = clock.timeNow();
	rotate(speed, Motor, end - current);
	accelerate(speed, 0, Motor);
}

Intake::Spin(double pctspeed, double time, bool reverse) {
	std::thread Thread1(spin, pctspeed, time, Intake1);
	std::thread Thread2(spin, pctspeed, time, Intake2);
}

#endif
