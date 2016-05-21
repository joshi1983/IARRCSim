#include "Shape.hpp"

bool Shape::consideredWithin(const Vector3D &pos,double distance) const
{
  return true;
}

void Shape::getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,const Vector3D *directions,int numRays,double *results)
{
 Ray3D r(pos,axis1);

  for (int i=0;i<numRays;i++)
  {
    r.setDirection(directions[i]);
    double depth=getNearestPositiveIntersection(r,minDepth,maxDepth);

    //if (depth>=0 && depth<maxDepth)
	 //   std::cout << "index "<<i<<" has depth: "<<depth<<std::endl;

    if (depth>0 && depth<results[i])
    {
	    results[i]=depth;
    }
  }
}
