#include "ZeroPlane.hpp"

#include <iostream>
#include <cmath>

using namespace std;

double ZeroPlane::getNearestPositiveIntersection(const Ray3D &ray,double minDepth,double maxDepth) const
{
 double y=ray.getPosition().getY();

  if (y<=0) // only has to work from top of plane
     return -1;

 double dy=ray.getDirection().getY();

  if (dy>-0.00001) // avoid division by zero
     return -1;

  return -y/dy;
}

bool ZeroPlane::consideredWithin(const Vector3D &pos,double distance) const
{
  if (distance>abs(pos.getY()))
	  return true;
  else
	  return false;

	return true;
}
