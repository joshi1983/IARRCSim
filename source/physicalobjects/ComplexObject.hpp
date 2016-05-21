#ifndef COMPLEX_OBJECT_HEADER
#define COMPLEX_OBJECT_HEADER

#include "PhysicalObject.hpp"
#include "GCommand.hpp"
#include <list>

class ComplexObject;

#include "TranslateObjectWrapper.hpp"

class ComplexObject: public PhysicalObject
{
private:
	std::list<GCommand*> commands;
     double latitudeDegrees,longitudeDegrees,geoRotationDegrees;
     double boundary_XRange,boundary_ZRange;

public:
	ComplexObject();

    void setBoundaries(double xRange,double zRange);
    double getBoundaryXRange() const;
    double getBoundaryZRange() const;

	virtual void addShapeRepresentation(
		ShapeCollection &shapes) const;

	void add(GCommand* gc);
	TranslateObjectWrapper * getWrapperByName(const char *name);
	void getWrappersByName(const char *name,std::list<TranslateObjectWrapper *> & wrappers);

	virtual void draw(int windowid,const Vector3D &viewpoint);
	virtual void drawForDesign(int windowid);
	virtual void writeTo(std::ostream &out) const;

	virtual void getAsLines(std::vector<LineSegment> &lines) const;
	virtual std::string getAsLines(const std::string &format) const;
	void setGeoOriginAndOrientation(
		double latitude,double longitude,double rotationAngle);

	double getLatitudeDegrees() const;
	double getLongitudeDegrees() const;
	double getGeoRotationDegrees() const;
};

#endif
