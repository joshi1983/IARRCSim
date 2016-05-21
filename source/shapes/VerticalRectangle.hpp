#ifndef VERTICAL_RECTANGLE_HEADER
#define VERTICAL_RECTANGLE_HEADER

#include "Shape.hpp"
#include "../graphics/Rotator2D.hpp"

class VerticalRectangle: public Shape
{
 protected:
   Vector3D position;
   Rotator2D rotator;
   double length;
   double height;

 public:
   VerticalRectangle(const Vector3D &pos,
     double length,double height,double rotationDegrees);

   virtual double getNearestPositiveIntersection(
 const Ray3D &ray,double minDepth,double maxDepth) const;

     virtual bool consideredWithin(const Vector3D &pos,double distance) const;
};

#endif