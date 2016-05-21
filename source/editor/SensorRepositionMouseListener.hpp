#ifndef SENSOR_REPOSITION_MOUSE_LISTENER
#define SENSOR_REPOSITION_MOUSE_LISTENER

#include "MouseMotionListener.hpp"
#include "../sensors/Sensor.hpp"

class SensorRepositionMouseListener: public MouseMotionListener
{
 private:
   Sensor *sensor;
   int view; // indicates what axis to ignore.

 public:
   SensorRepositionMouseListener(Sensor *sensor,int view); 
   virtual void mouseMoved(double x,double y);
   int getView() const;
};

#endif