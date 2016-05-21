#ifndef SENSOR_ROTATION_MOUSE_LISTENER_HEADER
#define SENSOR_ROTATION_MOUSE_LISTENER_HEADER

#include "MouseMotionListener.hpp"
#include "../sensors/Sensor.hpp"

class SensorRotationMouseListener: public MouseMotionListener
{
 private:
   Sensor * sensor;
   int view;

 public:
   SensorRotationMouseListener(Sensor *s,int view);
   virtual void mouseMoved(double x,double y);
};

#endif