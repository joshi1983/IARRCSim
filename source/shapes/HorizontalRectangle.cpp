#include "HorizontalRectangle.hpp"

#include <cmath>

using namespace std;

HorizontalRectangle::HorizontalRectangle(const Vector3D&pos,double length,
						   double width,double rotationDegrees):
 position(pos),length(length),width(width),rotator(rotationDegrees*PI/180)
{

}

double HorizontalRectangle::getNearestPositiveIntersection(
   const Ray3D &ray,double minDepth,double maxDepth) const
{
 Ray3D r(ray);
  r.translateBy(-position);
  r.rotateUsing(rotator);

  Vector3D dir(r.getDirection());
  Vector3D pos(r.getPosition());

   if (abs(dir.getY())<0.0001) // avoid division by 0.
	 return maxDepth;

  // now similar to ZeroPlane.
 double d=-pos.getY()/dir.getY();

  if (d<0)
	  return maxDepth;

  double x=pos.getX()+d*dir.getX();

  if (abs(x)>width/2)
     return maxDepth;

 double z=pos.getZ()+d*dir.getZ();

  if (abs(z)>length/2)
     return maxDepth;

  return d;
}