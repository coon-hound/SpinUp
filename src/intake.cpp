#include "time.h"
#include "intake.h"
#include "port_config.h"
#include <vex.h>
using namespace vex;

void _spin(double pctspeed, double time) {
	int _startTime = clock.now(), _endTime = _startTime + time, _currentTime = _startTime;
	double _speed = 0.0;
	while (speed < pctspeed) {
		speed += 0.025;
		Intake1 -> spin(directionType::fwd, speed, velocityUnits::pct);
		Intake2 -> spin(directionType::fwd, speed, velocityUnits::pct);
		_currentTime = clock.now();
	}
	while (_currentTime < _endTime) {
		_currentTime = clock.now();
		Intake1 -> spin(directionType::fwd, pctspeed, velocityUnits::pct);
		Intake2 -> spin(directionType::fwd, pctspeed, velocityUnits::pct);
	}
	while (speed > 0) {
		speed -= 0.25;
		Intake1 -> spin(directionType::fwd, speed, velocityUnits::pct);
		Intake2 -> spin(directionType::fwd, speed, velocityUnits::pct);
	}
}

Intake::Intake(int PORT1, int PORT2) {
	*Intake1 = vex::motor(PORT1, true);
	*Intake2 = vex::motor(PORT2, false);
}

Intake::Spin(double pctspeed, double time) {
	std::thread Thread1(_spin, pctspeed, time);
	std::thread Thread2(_spin, pctspeed, time);
}
