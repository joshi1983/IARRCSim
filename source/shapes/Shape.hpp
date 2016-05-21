#ifndef SHAPE_HEADER
#define SHAPE_HEADER

#include "../vectors/Ray3D.hpp"


class Shape
{
 public:

  virtual bool consideredWithin(const Vector3D &pos,double distance) const;
  virtual double getNearestPositiveIntersection(
   const Ray3D &ray,double minDepth,double maxDepth) const=0;

  /**
  @param directions is dependent on other parameters but is available 
  for an efficiency boost in case the implementation would have to calculate them.
  */
  virtual void getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,const Vector3D *directions,int numRays,double *results);
};

#endif