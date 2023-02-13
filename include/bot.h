#ifndef BASE_H
#define BASE_H
#include "time.h"
#include <vex.h>
#include <thread>
#include <memory>

namespace Bot{
	extern void adjustHeading(int x, int y, double pctspeed);
	extern void setHeading(double angle);
	extern void turn(double angle);
	extern void shoot();
}
#endif
