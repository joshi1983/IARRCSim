#ifndef CONE_HEADER
#define CONE_HEADER

#include "Shape.hpp"

class Cone: public Shape
{
 private:
    Vector3D position;
    double slope;
    double slope_sqr; // maintained as slope*slope
    double height;
    double maxDistanceFromPosition; // maintained as sqrt(height*height+(height*slope)^2)

    void updateMaxDistanceFromPosition();

    /**
    Gets the minimum and maximum indexes into the rays needed to be checked.
    */
    void getDirectionRange(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2,double degreesToScan,int numRays,int &minIndex,int &maxIndex);

 public:
    Cone(const Vector3D& position);
    void setSlope(double slope);
    void setHeight(double height);
    void setPosition(const Vector3D & pos);
    const Vector3D& getPosition() const;

    virtual void translateBy(const Vector3D &v);
    virtual bool consideredWithin(const Vector3D &pos,double distance) const;
    virtual double getNearestPositiveIntersection(const Ray3D &ray,double minDepth,double maxDepth) const;

    virtual void getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,const Vector3D *directions,int numRays,double *results);
};

#endif