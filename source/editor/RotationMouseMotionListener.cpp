#include "RotationMouseMotionListener.hpp"

RotationMouseMotionListener::RotationMouseMotionListener(Rotatable *r,double x,double y)
 : r(r),x(x),y(y)
{

}

void RotationMouseMotionListener::mouseMoved(double x,double y)
{
  r->setRotation(this->x,this->y,x,y);
}