#ifndef STOP_SIGN_HEADER
#define STOP_SIGN_HEADER

#include "PhysicalObject.hpp"
#include "Rotatable.hpp"

class StopSign: public PhysicalObject, public Rotatable
{
private:
	static double lineDistance;
	static double halfLineWidth;

  public:
     StopSign();
	StopSign(double x,double y,double z,double degreesRotation);
	virtual void draw(int windowid,const Vector3D &viewpoint);
	virtual void drawForDesign(int windowid);
	virtual void writeTo(std::ostream & out) const;
	virtual void readFrom(std::istream &in);
	virtual std::string getName() const;
	static void init();
	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);

	virtual double getUIRadius() const;
	virtual bool setProperty(const std::string &propertyName,double value);

	static bool setGlobalWidth(double val);
	static bool setGlobalBottom(double val);

	static bool setGlobalLineDistance(double val);
	static bool setGlobalLineWidth(double val);

	static double getGlobalWidth();
	static double getGlobalBottom();

	static double getGlobalLineDistance();
	static double getGlobalLineWidth();
};

#endif
