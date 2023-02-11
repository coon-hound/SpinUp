#include "time.h"
#include <vex.h>
#include <memory>
#include <thread>
#include "flywheel.h"

void _spin(double pctspeed, double time) {
	int _startTime = clock.now(), _endTime = _startTime + time, _currentTime = _startTime;
	double _speed = 0.0;
	while (speed < pctspeed) {
		speed += 0.025;
		Flywheel1 -> spin(directionType::fwd, speed, velocityUnits::pct);
		Flywheel2 -> spin(directionType::fwd, speed, velocityUnits::pct);
		_currentTime = clock.now();
	}
	while (_currentTime < _endTime) {
		_currentTime = clock.now();
		Flywheel1 -> spin(directionType::fwd, pctspeed, velocityUnits::pct);
		Flywheel2 -> spin(directionType::fwd, pctspeed, velocityUnits::pct);
	}
	while (speed > 0) {
		speed -= 0.25;
		Flywheel1 -> spin(directionType::fwd, speed, velocityUnits::pct);
		Flywheel2 -> spin(directionType::fwd, speed, velocityUnits::pct);
	}
}

Flywheel::Flywheel(int PORT1, int PORT2) {
	*Flywheel1 = vex::motor(PORT1, true);
	*Flywheel2 = vex::motor(PORT2, false);
}

Flywheel::Spin(double pctspeed, double time) {
	std::thread Thread1(_spin, pctspeed, time);
	std::thread Thread2(_spin, pctspeed, time);
}
