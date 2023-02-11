#ifndef TIME_H
#define TIME_H

#include <chrono>

class Clock {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> clockbirth;
public:
	Clock() {
		clockbirth = std::chrono::high_resolution_clock::now();
	}
	unsigned long long int now() {
		auto timenow = std::chrono::high_resolution_clock::now();
		auto timecast = std::chrono::duration_cast<std::chrono::milliseconds> (timenow - clockbirth);
		unsigned long long int time = timecast.count();
		return time;
	}
};

Clock globalClock;

#endif
