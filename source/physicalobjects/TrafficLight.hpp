#ifndef TRAFFIC_LIGHT_HEADER
#define TRAFFIC_LIGHT_HEADER

#include "InteractiveObject.hpp"
#include "Rotatable.hpp"
#include <iostream>



class TrafficLight: public InteractiveObject, public Rotatable
{
private:
	int state;

public:
	TrafficLight();
	TrafficLight(double x,double y,double z,double rotationDegrees);
	void setRotation(double degreesAngle);
	virtual void draw(int windowid,const Vector3D &viewpoint);
	virtual void drawForDesign(int windowid);
	virtual void ticksPerformed(Environment * e,int milis);
	virtual void writeTo(std::ostream & out) const;
	virtual void readFrom(std::istream &in);
	virtual std::string getName() const;
	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);
	     virtual double getUIRadius() const;

		 static bool setGlobalBottom(double val);
		 static bool setGlobalHeight(double val);
		 static bool setGlobalWidth(double val);
		 static bool setGlobalLightRadius(double val);
		 static bool setGlobalLineDistance(double val);
		 static bool setGlobalLineWidth(double val);
		 static bool setGlobalRedLightTiming(int val);
		 static bool setGlobalGreenLightTiming(int val);

		 static double getGlobalBottom();
		 static double getGlobalHeight();
		 static double getGlobalWidth();
		 static double getGlobalLightRadius();
		 static double getGlobalLineDistance();
		 static double getGlobalLineWidth();
		 static double getGlobalRedLightTiming();
		 static double getGlobalGreenLightTiming();
};

#endif
