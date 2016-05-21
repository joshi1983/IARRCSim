#ifndef REPOSITION_MOUSE_MOTION_LISTENER_HEADER
#define REPOSITION_MOUSE_MOTION_LISTENER_HEADER

#include "MouseMotionListener.hpp"
#include "../physicalobjects/PhysicalObject.hpp"

class RepositionMouseMotionListener: public MouseMotionListener
{
 private:
  PhysicalObject *po;

 public:
   RepositionMouseMotionListener(PhysicalObject *po); 
   virtual void mouseMoved(double x,double y);
   virtual ~RepositionMouseMotionListener();
};

#endif