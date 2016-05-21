#include "Rotatable.hpp"
#include "../editor/RotationMouseMotionListener.hpp"

#ifdef _UBUNTU_

#include <GL/freeglut.h>
#include <GL/glut.h>

#else

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <cmath>

using namespace std;

Rotatable::Rotatable():rotator(0)
{
  setRotation(0);
}

Rotatable::Rotatable(double rotation):rotator(rotation*PI/180)
{
  setRotation(rotation);
}

void Rotatable::setRotation(double position_x,double position_z,double x,double z)
{
	x-=position_x;
	z-=position_z;

  double angle=atan2(z,-x);

   setRotation(angle*180/PI-90);
}

void Rotatable::getActiveXZPointsInRange(double position_x,
		double position_z,double tipDistance,
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
  double x2,z2;

  x2=position_x+tipDistance*sin(rotation*PI/180);
  z2=position_z+tipDistance*cos(rotation*PI/180);
  // calculate coordinates for the end point.

  x=x2-x;
  z=z2-z;

  double dist=sqrt(x*x+z*z);
  // calculate distance between (x2,z2) to the end point.

  if (dist<maxDistance)
  {
     results.push_back(
	 ActiveVector2D(x2,z2,
	  new RotationMouseMotionListener(this,position_x,position_z)));
  }
}

void Rotatable::drawForDesign(int windowid,double x,double z,double size) const
{
 double dx=size*0.03;

  glColor3d(0,0,0.99);

  // draw a line.
  glPushMatrix();

     glTranslated(x,z,0);
	glRotated(-rotation,0,0,1);

	glBegin(GL_QUADS);

	  glVertex2d(-dx,0);
	  glVertex2d(dx,0);
	  glVertex2d(dx,size);
	  glVertex2d(-dx,size);

	glEnd();

  glPopMatrix();
}

double Rotatable::getRotation() const
{
	return rotation;
}

double Rotatable::getRotationInRadians() const
{
  return rotation*PI/180;
}

void Rotatable::setRotation(double rotationDegrees)
{
     this->rotation=rotationDegrees;

     rotator.setAngleDegrees(-this->rotation);
	reverseRotator.setAngleDegrees(this->rotation);
}
