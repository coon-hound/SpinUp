/*
PORT 1  LEFT MOTOR
PORT 2  RIGHT MOTOR
*/

#include "vex.h"
#include <bits/stdc++.h>
#include "control.h"

using namespace vex;

brain Brain;
task controller_control;

int main() {
  // initialize controller control
  task controller_control = task(control);
}