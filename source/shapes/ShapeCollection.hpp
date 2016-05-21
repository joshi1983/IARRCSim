#ifndef SHAPE_COLLECTION_HEADER
#define SHAPE_COLLECTION_HEADER

#include "Shape.hpp"
#include <list>

class ShapeCollection
{
 private:
   std::list<Shape*> shapes;

 public:

   virtual ~ShapeCollection();
   void clear();
   void add(Shape *s);
   int size() const;
   void removeShapesNotConsideredWithin(const Vector3D &pos,double maxDepth);
   virtual double getNearestPositiveIntersection(
	   const Ray3D &ray,double minDepth,double maxDepth) const;

    void getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,int numRays,double *results);

};

#endif