#include "PerspectiveViewTransformer.hpp"
#include <cmath>

using namespace std;

PerspectiveViewTransformer::PerspectiveViewTransformer()
 : ViewTransformer(350.0), distance(100)
{
  updateViewPoint();
}

double PerspectiveViewTransformer::getVerticalRotation() const
{
  return xAxisRotator.getAngleInRadians();
}

double PerspectiveViewTransformer::getHorizontalRotation() const
{
  return yAxisRotator.getAngleInRadians();
}

void PerspectiveViewTransformer::updateViewPoint()
{
 double x,y,z;

  y=distance*sin(xAxisRotator.getAngleInRadians());

 double r=sqrt(distance*distance-y*y);

  x=r*cos(-yAxisRotator.getAngleInRadians());
  z=r*sin(-yAxisRotator.getAngleInRadians());

  viewpoint.set(x,y,z);
}

/** 
 Updates viewpoint accordingly.
*/
void PerspectiveViewTransformer::setDistance(double distance)
{
  this->distance=distance;
  updateViewPoint();
}

/**
 Updates viewpoint and yAxisRotator accordingly
*/
void PerspectiveViewTransformer::setHorizontalRotation(double radians)
{
  this->yAxisRotator.setAngleRadians(radians);
  updateViewPoint();
}

/**
 Updates viewpoint and xAxisRotator accordingly
*/
void PerspectiveViewTransformer::setVerticalRotation(double radians)
{
  this->xAxisRotator.setAngleRadians(radians);
  updateViewPoint();
}

bool PerspectiveViewTransformer::transform(const Vector3D &p3d,
 int cx,int cy,int &x,int &y) const
{
  Vector3D p=p3d;

   xAxisRotator.rotateYZ(p);
   yAxisRotator.rotateXZ(p);

   x=y=0;

   double z=p.getZ();

   z+=distance;
   if (z<=0.1)
   {
	 return false; // point shouldn't appear in display
   }
   
   x=cx+(int)(zoom*p.getX()/z);
   y=cy-(int)(zoom*p.getY()/z);

   return true;
}


