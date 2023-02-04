#include "intake.h"
#include "port_config.h"
#include <vex.h>
using namespace vex;

Intake::Intake()
{
	*Intake1 = motor(INTAKE1, ratio36_1, true);
	*Intake2 = motor(INTAKE2, ratio36_1, false);
}

void Intake::Spin(double pctspeed, bool reverse)
{
	if(reverse){
		pctspeed *= -1;
	}
	Intake1->spin(fwd, pctspeed, percentUnits::pct);
	Intake2->spin(fwd, pctspeed, percentUnits::pct);
}