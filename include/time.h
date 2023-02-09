#ifndef TIME_H
#define TIME_H

#include <chrono>

class Clock {
public:
	int timeNow() {
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
	}
};

#endif
