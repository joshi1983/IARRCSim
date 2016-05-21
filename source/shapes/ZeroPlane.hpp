#ifndef ZEROPLANE_HEADER
#define ZEROPLANE_HEADER

#include "Shape.hpp"

class ZeroPlane: public Shape
{
 private:

 public:

    virtual bool consideredWithin(const Vector3D &pos,double distance) const;
    virtual double getNearestPositiveIntersection(const Ray3D &ray,
	    double minDepth,double maxDepth) const;
};

#endif