#ifndef TUNNEL_HEADER
#define TUNNEL_HEADER

#include "PhysicalObject.hpp"
#include "Rotatable.hpp"
#include "Pylon.hpp"
#include <list>

class Tunnel: public PhysicalObject, public Rotatable
{
private:
	std::list<Pylon*> pylons;

	double length,width,height;

	void initPylons();

public:
	Tunnel();
	Tunnel(double x,double y,double z,double rotation);
	virtual ~Tunnel();
     virtual std::string getName() const;
	virtual void addShapeRepresentation(ShapeCollection &shapes) const;
	virtual void draw(int windowid,const Vector3D &viewpoint);
     virtual void drawForDesign(int windowid);
	virtual void writeTo(std::ostream & out) const;
	virtual void readFrom(std::istream &in);
	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);

	virtual void transform(Vector3D &v) const;
	virtual bool setProperty(const std::string &propertyName,double value);
	void addObstaclePoints(std::list<Vector2D> &points) const;

	virtual double getUIRadius() const;

     static bool setGlobalWidth(double val);
     static bool setGlobalHeight(double val);
     static bool setGlobalLength(double val);
     static bool setGlobalMinLength(double val);
     static bool setGlobalMaxLength(double val);

	static double getGlobalWidth();
	static double getGlobalHeight();
     static double getGlobalLength();
     static double getGlobalMinLength();
     static double getGlobalMaxLength();
};

#endif
