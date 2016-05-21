#ifndef PHYSICAL_OBJECT_HEADER
#define PHYSICAL_OBJECT_HEADER

#include "../vectors/Vector3D.hpp"
#include "../vectors/Ray3D.hpp"
#include "../editor/ActiveVector2D.hpp"
#include "../shapes/ShapeCollection.hpp"
#include "LineSegment.hpp"

#include <string>
#include <list>
#include <vector>

/**

*/
class PhysicalObject
{
protected:
	Vector3D position;

public:
	PhysicalObject(double x,double y,double z);
	virtual ~PhysicalObject();
	double getX() const;
	double getY() const;
	double getZ() const;
	void setX(double x);
	void setY(double y);
	void setZ(double z);

	virtual bool setProperty(const std::string &propertyName,double value);
	virtual Vector3D getVelocity() const;
	const Vector3D &getPosition() const;

    void enforcePositionBoundaries();

	virtual void setXZPosition(double x,double z);
	double getXZDistanceFrom(double x,double z) const;
     double getXZDistanceFrom(const Vector2D &pos) const;

	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);

	virtual void drawShadows(const Vector3D &lightDirection) const;
     virtual double getUIRadius() const;

	virtual void join(PhysicalObject * obj);
	virtual std::string getName() const;
	virtual void addShapeRepresentation(ShapeCollection &shapes) const;
	virtual void draw(int windowid,const Vector3D &viewpoint)=0;
     virtual void drawForDesign(int windowid)=0;
    virtual std::string getDescription(const std::string &format) const;
	virtual void writeTo(std::ostream & out) const;
	virtual void readFrom(std::istream &in);

	virtual double getIntersectionCoefficient(const Ray3D &r) const;

	virtual double getElevationForXZPoint(const Vector2D &p) const;
	virtual void getTopNormal(const Vector2D &xzPoint,Vector3D &normal) const;
	virtual void getAsLines(std::vector<LineSegment> &lines) const;
};

#endif
