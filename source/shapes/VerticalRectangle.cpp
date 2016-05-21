#include "VerticalRectangle.hpp"

#include "../stdafx.h"
#include <iostream>
#include <cmath>

using namespace std;

VerticalRectangle::VerticalRectangle(const Vector3D &pos,
     double length,double height,double rotationDegrees):position(pos),
 rotator(rotationDegrees*PI/180),length(length),height(height)
{

}

double VerticalRectangle::getNearestPositiveIntersection(
 const Ray3D &ray,double minDepth,double maxDepth) const
{
 Ray3D r(ray);

  r.translateBy(-position);
  r.rotateUsing(rotator);

  double dx=r.getPosition().getX();
  double dir_x=r.getDirection().getX();

  if (abs(dir_x)<0.0001)
	  return maxDepth;

  double depth = -dx/dir_x;
  double z= r.getPosition().getZ()+depth*r.getDirection().getZ();
  if (abs(z)>length/2) // intersection is beyond the end of the rectangle.
	  return maxDepth;

  double y=r.getPosition().getY()+depth*r.getDirection().getY();
  if (abs(y)>height/2) // intersection is beyond the height of the rectangle
	  return maxDepth;

  // is the point within the length and height constraints?

  return depth;
}

bool VerticalRectangle::consideredWithin(const Vector3D &pos,double distance) const
{
  double dist = position.distanceFrom(pos)-(length+height)/2;

  if (dist>distance)
	  return false;
  else
	  return true;

	return true;

}
