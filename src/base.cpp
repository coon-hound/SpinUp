#include "time.h"
#include "base.h"
#include "port_config.h"
#include <vex.h>
using namespace vex;

Base::Base(int PORT1, int PORT2, int PORT3, int PORT4) {
	*LeftMotor1 = motor(PORT1, ratio18_1, true);
	*LeftMotor2 = motor(PORT2, ratio18_1, true);
	*RightMotor1 = motor(PORT3, ratio18_1, false);
	*RightMotor2 = motor(PORT4, ratio18_1, false);
}

void Base::Goto(int x, int y, double pctspeed) {
	
}

void Base::Rotate(double angle) {

}

void Base::SetHeading(double angle) {

}

void Base::Shoot() {

}
