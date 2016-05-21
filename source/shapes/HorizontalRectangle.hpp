#ifndef HORIZONTAL_PLANE_HEADER
#define HORIZONTAL_PLANE_HEADER

#include "../stdafx.h"
#include "Shape.hpp"
#include "../graphics/Rotator2D.hpp"


/**
 A plane similar to ZeroPlane except that it is a 
finite rectangle on an adjustable position.
*/
class HorizontalRectangle: public Shape
{
 private:
  Vector3D position;
  double width,length;
  Rotator2D rotator;

 public:
    HorizontalRectangle(const Vector3D&pos,double length,
	    double width,double rotationDegrees);

    virtual double getNearestPositiveIntersection(
   const Ray3D &ray,double minDepth,double maxDepth) const;

};

#endif