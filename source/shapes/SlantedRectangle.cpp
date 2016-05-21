#include "SlantedRectangle.hpp"


#include <cmath>

using namespace std;

SlantedRectangle::SlantedRectangle(const Vector3D& pos,double length,double width,
     double xzRotationDegrees,double slantDegrees):
 position(pos),length(length),width(width),xzRotator(xzRotationDegrees*PI/180),
 slantRotator(slantDegrees*PI/180)
{
 
}

double SlantedRectangle::getNearestPositiveIntersection(
   const Ray3D &ray,double minDepth,double maxDepth) const
{
 Ray3D r(ray);

  r.translateBy(-position);

  // rotate the ray on x-z plane.
  r.rotateUsing(xzRotator);

  // rotate the ray for the slant.
  r.rotateYZUsing(slantRotator);

  // now the math is similar to ZeroPlane.

 Vector3D dir(r.getDirection());
 Vector3D pos(r.getPosition());

 if (abs(dir.getY())<0.0001) // avoid division by 0.
	 return maxDepth;

 double d=-pos.getY()/dir.getY();

  if (d<0) // no positive intersection.
     return maxDepth;

 double x=pos.getX()+d*dir.getX();

  if (abs(x)>width/2)
     return maxDepth;

 double z=pos.getZ()+d*dir.getZ();

  if (abs(z)>length/2)
     return maxDepth;
 
  return d;  
}