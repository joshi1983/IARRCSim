#include "TrafficLight.hpp"
#include "../stdafx.h"

#include "../app/Application.hpp"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/glut.h>
#include <gl/glu.h>

#endif

#include <cmath>

#define CIRCLE_NUM_SIDES 15
#define STAND_HALF_WIDTH 3

static int GREEN_TIME=9000;
static int RED_TIME=3000;

static double HALF_WIDTH=20;
static double HALF_HEIGHT=50;
static double LIGHT_RADIUS=10;

static double lineDistance=150;
static double halfLineWidth=10;
static double LIGHT_BOTTOM=100;
const double line_spacer=0.4;


using namespace std;

TrafficLight::TrafficLight():
 InteractiveObject(0,LIGHT_BOTTOM,0,10000),Rotatable(0), state(0)
{
}

TrafficLight::TrafficLight(double x,double y,double z,double degreesRotation):
 InteractiveObject(x,y,z,10000), state(0),Rotatable(degreesRotation)
{
}

 bool TrafficLight::setGlobalBottom(double val)
 {
   if (val<0)
   {
     cout << "Street Light bottom must be positive."<<endl;
     return false;
   }

   LIGHT_BOTTOM=val;
   return true;
 }

bool TrafficLight::setGlobalHeight(double val)
{
  if (val<5)
  {
	  cout << "Traffic Lights can't be shorter than 5."<<endl;
       return false;
  }

   HALF_HEIGHT=val*0.5;
   return true;
}

bool TrafficLight::setGlobalWidth(double val)
{
   HALF_WIDTH=val*0.5;
   return true;
}

bool TrafficLight::setGlobalLightRadius(double val)
{
  if (val<1)
  {
	cout << "Can't have traffic light radii less than 1."<<endl;
	return false;
  }
   LIGHT_RADIUS=val;
   return true;
}

bool TrafficLight::setGlobalLineDistance(double val)
{
   if (val<0)
   {
      cout << "Can't have negative line distance for traffic lights."<<endl;
	 return false;
   }
   lineDistance=val;
   return true;
}

bool TrafficLight::setGlobalLineWidth(double val)
{
	if (val<0)
	{
		cout << "Traffic light line width must be at least 0."<<endl;
	   return false;
	}
  halfLineWidth=val*0.5;
  return true;
}

bool TrafficLight::setGlobalRedLightTiming(int val)
 {
 	 if (val<0)
	 {
		 cout << "Can't have traffic light in red state less than 0 time."<<endl;
		 return false;
	 }

   RED_TIME=val;
   return true;
 }


bool TrafficLight::setGlobalGreenLightTiming(int val)
 {
	 if (val<0)
	 {
		 cout << "Can't have traffic light in green state less than 0 time."<<endl;
		 return false;
	 }

   GREEN_TIME=val;
   return true;
 }

double TrafficLight::getGlobalBottom()
{
  return LIGHT_BOTTOM;
}
double TrafficLight::getGlobalHeight()
{
  return HALF_HEIGHT*2.0;
}
double TrafficLight::getGlobalWidth()
{
  return HALF_WIDTH*2.0;
}
double TrafficLight::getGlobalLightRadius()
{
  return LIGHT_RADIUS;
}
double TrafficLight::getGlobalLineDistance()
{
  return lineDistance;
}
double TrafficLight::getGlobalLineWidth()
{
  return halfLineWidth*2.0;
}
double TrafficLight::getGlobalRedLightTiming()
{
  return RED_TIME;
}
double TrafficLight::getGlobalGreenLightTiming()
{
  return GREEN_TIME;
}

void drawCircle(double x,double y,double z,double radius)
{

  glBegin(GL_POLYGON);

  // loop through vertexes.
  for (int i=0;i<CIRCLE_NUM_SIDES;i++)
  {
   double angle=i*2*PI/CIRCLE_NUM_SIDES;
   double cx,cy,cz;

    cx=x+radius*cos(angle);
    cy=y+radius*sin(angle);
    cz=z;
    glVertex3d(cx,cy,cz);
  }

  glEnd();

}

void TrafficLight::draw(int windowid,const Vector3D &viewpoint)
{
  glPushMatrix();
  glTranslated(getX(),getY(),getZ());
  glRotated(rotation,0,1,0);

  glColor3d(0,0,0);


  // draw a black rectangle.
 glBegin(GL_QUADS);

	glVertex3d(-HALF_WIDTH, +HALF_HEIGHT, 0);
	glVertex3d(+HALF_WIDTH, +HALF_HEIGHT, 0);
	glVertex3d(+HALF_WIDTH, -HALF_HEIGHT, 0);
	glVertex3d(-HALF_WIDTH, -HALF_HEIGHT, 0);

     // draw the stand.
     glVertex3d(-STAND_HALF_WIDTH, -HALF_HEIGHT, 0);
	glVertex3d(+STAND_HALF_WIDTH, -HALF_HEIGHT, 0);
	glVertex3d(+STAND_HALF_WIDTH, -getY(), 0);
	glVertex3d(-STAND_HALF_WIDTH, -getY(), 0);

  glEnd();


  // draw green circle.
  if (state<GREEN_TIME) // green
     glColor3d(0.3,0.1,0.1);
  else
     glColor3d(1,0.2,0.1);

   if (Application::isLightingEnabled())
     glDisable(GL_LIGHTING);

  drawCircle(0,+HALF_HEIGHT/2,+1,LIGHT_RADIUS);

  // draw red circle.
  if (state>=GREEN_TIME) // green
     glColor3d(0.1,0.3,0.1);
  else
     glColor3d(0.1,1,0.1);


  drawCircle(0,-HALF_HEIGHT/2,+1,LIGHT_RADIUS);

   if (Application::isLightingEnabled())
      glEnable(GL_LIGHTING);

  // draw the white line as a quad.
  glColor3d(1,1,1);

  glBegin(GL_QUADS);

    if (Application::isLightingEnabled())
	    glNormal3d(0,1,0);

    glVertex3d(-lineDistance,line_spacer-getY(),lineDistance-halfLineWidth);
    glVertex3d(lineDistance,line_spacer-getY(),lineDistance-halfLineWidth);
    glVertex3d(lineDistance,line_spacer-getY(),lineDistance+halfLineWidth);
    glVertex3d(-lineDistance,line_spacer-getY(),lineDistance+halfLineWidth);

  glEnd();

  glPopMatrix();
}

void TrafficLight::drawForDesign(int windowid)
{
int size=30;

  glPushMatrix();

  glTranslated(getX(),getZ(),0);
  glRotated(-rotation,0,0,1);

  glColor3d(0,1,0);
  glBegin(GL_QUADS);

    glVertex2d(-size,-size);
    glVertex2d(size,-size);
    glVertex2d(size,size);
    glVertex2d(-size,size);

  glEnd();


  glPopMatrix();
  Rotatable::drawForDesign(windowid,getX(),getZ(),getUIRadius());

}

void TrafficLight::getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
	Rotatable::getActiveXZPointsInRange(getX(),getZ(),getUIRadius(),
		x,z,maxDistance,results);

	PhysicalObject::getActiveXZPointsInRange(x,z,maxDistance,results);
}

double TrafficLight::getUIRadius() const
{
   return 30*10;
}

void TrafficLight::ticksPerformed(Environment * e,int milis)
{
  state+=milis;
  if (state>RED_TIME+GREEN_TIME)
  {
    state-=RED_TIME+GREEN_TIME;
  }
}

void TrafficLight::writeTo(std::ostream & out) const
{
	InteractiveObject::writeTo(out);
	out << " " << rotation;
	out << " " << state;
}

void TrafficLight::readFrom(std::istream &in)
{
   InteractiveObject::readFrom(in);
   double rot;
   in >> rot;
   Rotatable::setRotation(rot);
   in >> state;
}

std::string TrafficLight::getName() const
{
	return std::string("TrafficLight");
}

