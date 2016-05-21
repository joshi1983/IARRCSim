#ifndef PYLON_HEADER
#define PYLON_HEADER

#include "../stdafx.h"

#ifdef _UBUNTU_

#include <GL/freeglut.h>
#include <GL/glut.h>

#else

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include "InteractiveObject.hpp"
#include "../shapes/Cone.hpp"
#include <iostream>

class Pylon: public InteractiveObject
{
private:
   static double global_height;
   static double global_radius;

	double brightness;
	/*
	Pylons are usually orange but is it pinkish? more yellowy?  more redish?
	To keep things realistic, the unique brightness of the pylon should be included.
	*/

  double height;
  double radius;
  double radius_over_height; // a cached relationship radius/height
  double radiusSqr_over_height; // another cached relationship.

  void updateRadiusOverHeight();

public:
	Pylon();
	Pylon(double x,double y,double z,double brightness);
	void setToRandomBrightness();
	double getBrightness() const;
	virtual void addShapeRepresentation(ShapeCollection &shapes) const;
	Cone *getAsCone() const;

	virtual void drawShadows(const Vector3D &lightDirection) const;
	virtual void draw(int windowid,const Vector3D &viewpoint);
	virtual void drawForDesign(int windowid);
	virtual void writeTo(std::ostream &out) const;
	virtual void readFrom(std::istream &in);
	virtual ~Pylon();
	virtual std::string getName() const;
	virtual void ticksPerformed(Environment * e,int milis);

	static double getGlobalHeight();
	static bool setGlobalHeight(double height);

	static double getGlobalRadius();
	static bool setGlobalRadius(double height);
};

#endif

