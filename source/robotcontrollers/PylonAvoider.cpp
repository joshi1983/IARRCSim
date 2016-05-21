#include "PylonAvoider.hpp"
#include "PathFinder.hpp"

using namespace std;

PylonAvoider::PylonAvoider(RoboticCar *car): RobotController(car)
{

}

void PylonAvoider::update(Environment * e)
{
  // use the PathFinder class to find a 
  // good new steering direction and speed.
  // current position can be found from the car variable.
  // car->getX(), car->getZ()...

  list<Vector2D> points;

  Vector2D position(car->getX(),car->getZ());

  e->addObstaclePointsWithinDistance(points,position,900,car);

  double speed=car->getSpeed();
  double direction=car->getCurrentDirection();
  double dirOld=direction;

  PathFinder::findDirectionAndSpeed(points,
	  position,direction,speed);

  car->setDrivingForce(speed);
  double steeringAngle = direction-dirOld;

  car->setDesiredSteeringAngle(steeringAngle);

}

