#include "vex.h"
#include <bits/stdc++.h>
#include "control.h"
#include "bot.h"

using namespace vex;

brain Brain;
task controller_control;
Bot base;
controller selectionController = controller(primary);

void autonomous();
void control_sequence();
void blueAutonomous();

int main() 
{
  // control_sequence();
  blueAutonomous();
  // autonomous();
  // control_sequence();

}

void control_sequence() 
{
  task controller_control = task(control);
}

void blueAutonomous() 
{
  base.Move(100, 100, 0, 50, 5, 0, mm, deg);
  std::cout << "Done 1\n\n\n\n\n\n\n\n\n";
  vexDelay(1000);
  base.Move(-100, -500, 15, 50, 5, 0, mm, deg);
  std::cout << "Done 2\n";
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
  while (!selectionController.ButtonUp.pressing() and !selectionController.ButtonDown.pressing())
  {
    vexDelay(10);
  }

  // autonomous code selection and execution
  if (selectionController.ButtonUp.pressing())
  {
    blueAutonomous();
  }
  else
  {
    redAutonomous();
  }
}