#include "vex.h"
#include <bits/stdc++.h>
#include "control.h"
#include "bot.h"

using namespace vex;

brain Brain;
task controller_control;
Bot base;
controller selectionController = controller(primary);
competition Competition;

void autonomous();
void control_sequence();
void blueAutonomous();

int main() 
{
  // base.test()
  Competition.autonomous(autonomous);
  Competition.drivercontrol(control_sequence);
  while (1)
  {
    vex::task::sleep(100);
  }
}

void control_sequence() 
{
  task controller_control = task(control);
}

void blueAutonomous() 
{

  base.Move(1500, -1500, 0, 50, 3, 50);
  base.Intake();
  base.Move(1200, -1200, 90, 50, 3, 50);
  base.Intake();
  base.Move(-20, 130, -47.5, 50, 3, 50);
  base.Shoot(6);
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
  while (true)
  {
    vexDelay(1000);
  }
  /*
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
  */
}
