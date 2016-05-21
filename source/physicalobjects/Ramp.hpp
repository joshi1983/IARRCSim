#ifndef RAMP_HEADER
#define RAMP_HEADER

#include "../io/Socket.hpp"
#include "InteractiveObject.hpp"
#include "Pylon.hpp"
#include "Rotatable.hpp"
#include <list>

class Ramp: public InteractiveObject, public Rotatable
{
  private:
	std::list<Pylon*> pylons;
     double width,height,length;

	void initPylons();
     void transformXZ(Vector2D &v) const;

  public:
	Ramp();
     Ramp(double x,double y,double z,double rotation);
	~Ramp();
	virtual void addShapeRepresentation(ShapeCollection &shapes) const;
	virtual void ticksPerformed(Environment * e,int milis);
     virtual void writeTo(std::ostream & out) const;
	virtual void readFrom(std::istream &in);
     virtual std::string getName() const;
	virtual void draw(int windowid,const Vector3D &viewpoint);
	virtual void drawForDesign(int windowid);
	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);

	 virtual double getUIRadius() const;

	 virtual void reverseTransformXZ(Vector2D &v) const;
	 virtual void transform(Vector3D &v) const;
	virtual void addObstaclePoints(std::list<Vector2D> &points) const;

	virtual bool setProperty(const std::string &propertyName,double value);
	 // assumed to need transformation
      virtual double getElevationForXZPoint(const Vector2D &p) const;
      virtual void getTopNormal(const Vector2D &xzPoint,Vector3D &normal) const;


	 static bool setGlobalHeight(double val);
      static bool setGlobalWidth(double val);
      static bool setGlobalLength(double val);

	 static double getGlobalHeight();
      static double getGlobalWidth();
      static double getGlobalLength();
};

#endif
