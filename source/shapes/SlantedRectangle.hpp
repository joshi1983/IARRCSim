#ifndef SLANTED_RECTANGLE_HEADER
#define SLANTED_RECTANGLE_HEADER

#include "../stdafx.h"
#include "Shape.hpp"
#include "../vectors/Vector3D.hpp"
#include "../vectors/Ray3D.hpp"
#include "../graphics/Rotator2D.hpp"

class SlantedRectangle: public Shape
{
 private:
   double length,width;
   Vector3D position;

   Rotator2D xzRotator;
   Rotator2D slantRotator;

 public:
   SlantedRectangle(const Vector3D& pos,double length,double width,
     double xzRotationDegrees,double slantDegrees);

  virtual double getNearestPositiveIntersection(
   const Ray3D &ray,double minDepth,double maxDepth) const;

};

#endif