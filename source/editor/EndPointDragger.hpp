#ifndef END_POINT_DRAGGER_HEADER
#define END_POINT_DRAGGER_HEADER

#include "MouseMotionListener.hpp"
#include "../physicalobjects/VerticalWall.hpp"

class EndPointDragger: public MouseMotionListener
{
 private:
  VerticalWall * wall;
  int pointNum; // 0 or 1.
  Vector2D otherP;

 public:
   EndPointDragger(VerticalWall * wall,int pointNum);
   virtual void mouseMoved(double x,double y);
};

#endif
