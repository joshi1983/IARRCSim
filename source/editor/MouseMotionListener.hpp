#ifndef MOUSE_MOTION_LISTENER_HEADER
#define MOUSE_MOTION_LISTENER_HEADER

class MouseMotionListener
{
 public:
  virtual void mouseMoved(double x,double y)=0;
  virtual ~MouseMotionListener();
};

#endif