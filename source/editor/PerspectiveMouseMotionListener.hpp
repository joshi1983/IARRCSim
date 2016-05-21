#ifndef PERSPECTIVE_MOUSE_MOTION_LISTENER_HEADER
#define PERSPECTIVE_MOUSE_MOTION_LISTENER_HEADER

#include "MouseMotionListener.hpp"
#include "../vectors/PerspectiveViewTransformer.hpp"

class PerspectiveMouseMotionListener: public MouseMotionListener
{
 private:
   int x,y; // mouse down position.
   double horizontal_radians,vertical_radians;
   PerspectiveViewTransformer * transformer;

 public:
   PerspectiveMouseMotionListener(int x,int y,PerspectiveViewTransformer * transformer);
   virtual void mouseMoved(double x,double y);
};

#endif