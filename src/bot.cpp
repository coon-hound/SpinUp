#include "time.h"
#include "bot.h"
#include "port_config.h"
#include <vex.h>
using namespace vex;

motor LeftMotor1(PORT1, ratio18_1, true);
motor LeftMotor2(PORT2, ratio18_1, true);
motor RightMotor1(PORT3, ratio18_1, false);
motor RightMotor2(PORT4, ratio18_1, false);

motor Flywheel1(FLYWHEEL1, ratio18_1, true);
motor Flywheel2(FLYWHEEL2, ratio18_1, false);

motor Intake1(INTAKE1, ratio18_1, true);
motor Intake2(INTAKE2, ratio18_1, false);

void Bot::AdjustHeading(int x, int y, double pctspeed) {
	
}

void Bot::Turn(double angle) {

}

void Bot::SetHeading(double angle) {

}

void Bot::Shoot(bool start) {
	if(start) {
		Flywheel1.setVelocity(100, percentUnits::pct);
		Flywheel2.setVelocity(100, percentUnits::pct);
	}
	else {
		Flywheel1.setVelocity(0, percentUnits::pct);
		Flywheel2.setVelocity(0, percentUnits::pct);
	}
}

void Bot::Intake(bool start) {
	if(start){
		Intake1.setVelocity(100, percentUnits::pct);
		Intake2.setVelocity(100, percentUnits::pct);
	}
	else{
		Intake1.setVelocity(0, percentUnits::pct);
		Intake2.setVelocity(0, percentUnits::pct);
	}
}