/*
PORT 1  LEFT MOTOR
PORT 2  RIGHT MOTOR
*/

#include "vex.h"
#include <bits/stdc++.h>
#include "control.h"
#include "bot.h"

using namespace vex;

brain Brain;
task controller_control;
Bot base;

int main() {  
  /*
   * void Move(double x, double y, double angle, double lengthTolerance = 25, double angleTolerance = 1, 
               double tickLength = 20, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg);
	 * void Turn(double angle);
	 * void SetHeading(double angle);
	 * void Shoot(int seconds);
	 * void Roll();
  */
  base.Move(1200, 1200, 45, 25, 1, 20, mm, deg);
  base.SetHeading(69);
  base.Move(-1200, -1200, 75, 25, 1, 20, mm, deg);

  // task controller_control = task(control);
}
