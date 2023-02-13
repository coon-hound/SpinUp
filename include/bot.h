#ifndef BASE_H
#define BASE_H
#include "time.h"
#include <vex.h>
#include <thread>
#include <memory>

class Bot{
public:
	void AdjustHeading(int x, int y, double pctspeed);
	void SetHeading(double angle);
	void Turn(double angle);
	void Shoot(bool start);
	void Intake(bool start);


};
#endif
