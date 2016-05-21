#include "VerticalIsoscelesTriangle.hpp"

#include <iostream>
#include <cmath>

using namespace std;

VerticalIsoscelesTriangle::VerticalIsoscelesTriangle(
 const Vector3D &pos,double length,
 double height,double rotationDegrees):
  VerticalRectangle(pos,length,height,rotationDegrees)
{

}

double VerticalIsoscelesTriangle::getNearestPositiveIntersection(
 const Ray3D &ray,double minDepth,double maxDepth) const
{
//	double depth = VerticalRectangle::getNearestPositiveIntersection(ray,minDepth,maxDepth);
 Ray3D r(ray);
 
  r.translateBy(-position);
  r.rotateUsing(VerticalRectangle::rotator);

  double dx=r.getPosition().getX();
  double dir_x=r.getDirection().getX();

 //  return depth;
  if (abs(dir_x)<0.0001)
	  return maxDepth;

  double depth = -dx/dir_x;
  double z= r.getPosition().getZ()+depth*r.getDirection().getZ();
  if (abs(z)>length/2)
	  return maxDepth;

  double y=r.getPosition().getY()+depth*r.getDirection().getY();
  if (abs(y)>height/2)
	  return maxDepth;

  if (abs(z)>0.01)
  {
  // is the point within the length and height constraints?
 double slope = (height/2-y)/abs(z);
  if (slope<2*height/length) // out of triangle boundary
     return maxDepth;

  }

  return depth;
}