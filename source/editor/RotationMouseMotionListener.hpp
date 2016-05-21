#ifndef ROTATION_MOUSE_MOTION_LISTENER
#define ROTATION_MOUSE_MOTION_LISTENER

#include "MouseMotionListener.hpp"
#include "../physicalobjects/Rotatable.hpp"

class RotationMouseMotionListener: public MouseMotionListener
{
 private:
    Rotatable *r;
   double x,y;


 public:
   RotationMouseMotionListener(Rotatable *r,double x,double y);
   virtual void mouseMoved(double x,double y);
};

#endif
