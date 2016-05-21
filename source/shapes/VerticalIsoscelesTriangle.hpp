#ifndef VERTICAL_TRIANGLE_HEADER
#define VERTICAL_TRIANGLE_HEADER

#include "VerticalRectangle.hpp"

/**

Represents a triangle parallel to the y-axis.

The shape is is
*/
class VerticalIsoscelesTriangle: public VerticalRectangle
{
 public:
   VerticalIsoscelesTriangle(const Vector3D &pos,
     double length,double height,double rotationDegrees);

   virtual double getNearestPositiveIntersection(
 const Ray3D &ray,double minDepth,double maxDepth) const;
};

#endif