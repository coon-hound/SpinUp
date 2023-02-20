# VEX Spin Up Code for 2022-23

## Controller Binds

```
Axis 3      Foward/Reverse Drive
Axis 4		Left/Right Drive
Axis 1      Left/Right Turn

Button Y 	Enable Precision Mode
Button X	Disable Precision Mode

Button A	Intake
Button X	Reverse Intake 

Button Up	Flywheel
```

## Move functions

```
Declared and defined respectively in include/bot.h and src/bot.cpp
(All angles mentioned are defined as follows: a clockwise turn is a negative rotation, 
and a counter-clockwise turn is a position rotation)

- void Move(double x, double y, double angle, double lengthTolerance, double angleTolerance, 
             double tickLength, distanceUnits lengthUnit, rotationUnits angleUnit);
    Moves the robot to the target coordinate (x, y) and facing a target heading (angle), 
    with a certain position tolerance (lengthTolerance = 25 mm by default) and heading tolerance 
    (angleTolerance = 1 degree by default). 
    Distance is measured in lengthUnits (default is mm) and heading is measured in angleUnits (default is deg). 
    tickLength is the approximate frequency that the robot recalculates motor headings.
    Note: angles are negative clockwise and positive counter-clockwise

- void Turn(double angle);
    Turns a certain amount specified by angle in degrees.

- void SetHeading(double angle);
    Turns the robot to a specific heading specified by angle in degrees.

- void Shoot(int seconds);
    Shooting disk for (seconds) seconds.

- void Roll();
    Spinning the Roller 180 degrees.
```

#### TODO
- [x] controller code for drive
- [x] controller code for intake
- [x] controller code for flywheel
- [x] controller code for roller (using intake)
- [x] temperature-based power throttling (unnecessary, remove later)
- [x] Decide on Autonomous round strategy
- [x] Autonomous code finished (not yet tested)
- [x] Updated code structure, putting autonomous functions in bot.h (defined in bot.cpp)
- [ ] Separate intake and shoot functions because the robot may need to move around; during intake, slowly move robot forward
- [ ] Potentially need to implement pneumatics

Motor binds in `port_config.h`

