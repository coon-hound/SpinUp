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
  base.Move(5, 5, 45);
  base.SetHeading(69);
  base.Move(-10, -5, 75);
}
