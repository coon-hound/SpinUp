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
  // initialize controller control
  // task controller_control = task(control);
  base.Move(1200, 1200, 45, 5);
  base.SetHeading(69);
  base.Move(-1200, -1200, 75, 5);
}
