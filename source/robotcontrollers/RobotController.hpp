#ifndef ROBOT_CONTROLLER_HEADER
#define ROBOT_CONTROLLER_HEADER

class RobotController;

#include "../physicalobjects/RoboticCar.hpp"
#include "../app/Environment.hpp"

/**
 For automatically driving cars.

Mainly to simulate the operation of other cars in the track.
*/
class RobotController
{
 protected:
   RoboticCar *car;

 public:
  RobotController(RoboticCar *car);
  virtual void update(Environment * e) =0;
};

#endif