#ifndef PYLON_AVOIDER_HEADER
#define PYLON_AVOIDER_HEADER

#include "RobotController.hpp"

class PylonAvoider: public RobotController
{
 public:
    PylonAvoider(RoboticCar *car);
    virtual void update(Environment * e);
};

#endif
