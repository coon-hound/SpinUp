#include "vex.h"
#include <bits/stdc++.h>
#include "control.h"
#include "bot.h"

using namespace vex;

brain Brain;
task controller_control;
Bot base;

void autonomous();

int main() 
{
  autonomous();
  control();

}

void control() 
{
  task controller_control = task(control);
}

void blueAutonomous() 
{
  /*
   * void Move(double x, double y, double angle, double lengthTolerance = 25, double angleTolerance = 1, 
               double tickLength = 20, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg);
	 * void Turn(double angle);
	 * void SetHeading(double angle);
	 * void Shoot(int seconds);
	 * void Roll();
  */
}

void redAutonomous() 
{
  /*
   * void Move(double x, double y, double angle, double lengthTolerance = 25, double angleTolerance = 1, 
               double tickLength = 20, distanceUnits lengthUnit = mm, rotationUnits angleUnit = deg);
	 * void Turn(double angle);
	 * void SetHeading(double angle);
	 * void Shoot(int seconds);
	 * void Roll();
  */
}

void autonomous() 
{
  // waiting for selection: Neither button is pressed
  while (!ButtonL1.pressing() and !ButtonR1.pressing())
  {
    vexDelay(10);
  }

  // autonomous code selection and execution
  if (ButtonL1.pressing())
  {
    blueAutonomous();
  }
  else
  {
    redAutonomous();
  }
}