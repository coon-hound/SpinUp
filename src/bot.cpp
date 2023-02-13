#include "time.h"
#include "bot.h"
#include "port_config.h"
#include <vex.h>
using namespace vex;

motor LeftMotor1(PORT1, ratio18_1, true);
motor LeftMotor2(PORT2, ratio18_1, true);
motor RightMotor1(PORT3, ratio18_1, false);
motor RightMotor2(PORT4, ratio18_1, false);

namespace Bot{
	void adjustHeading(int x, int y, double pctspeed) {
		
	}

	void turn(double angle) {

	}

	void setHeading(double angle) {

	}

	void shoot() {

	}
}
