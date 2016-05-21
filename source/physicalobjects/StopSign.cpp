#include "../stdafx.h"
#include "StopSign.hpp"

#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/glut.h>
#include <gl/glu.h>

#endif

#include <cmath>
#include <iostream>
#include "../app/Application.hpp"
#include "../graphics/Texture.hpp"


using namespace std;

/**
50 by 50 cm but remember, this is a polygon.
radius is just a bit bigger than 25cm.
*/
static double STOP_SIGN_RADIUS=27;
static double STOP_SIGN_WIDTH=50;
static double stopSignBottom=100;
static Texture * stopSignTexture;
static double sideFactor=1.5;

double StopSign::lineDistance=100;
double StopSign::halfLineWidth=5;

const char *STOPSIGN_TEXTURE_FILE_NAME = "images/stopSign.bmp";

void StopSign::init()
{
  stopSignTexture=new Texture(STOPSIGN_TEXTURE_FILE_NAME);

  if (stopSignTexture==NULL)
  {
    cout << "Unable to load stop sign texture from "
	    <<STOPSIGN_TEXTURE_FILE_NAME
	    <<endl;
  }
}

bool StopSign::setGlobalLineDistance(double val)
{
  if (val<0)
  {
	cout << "stop sign line distance must be at least 0."<<endl;
	return false;
  }
  lineDistance=val;
  return true;
}

bool StopSign::setGlobalLineWidth(double val)
{
  if (val<0)
  {
	cout << "stop sign line width must be at least 0."<<endl;
	return false;
  }
  halfLineWidth=val*0.5;
  return true;
}

bool StopSign::setGlobalWidth(double val)
{
  // same as val/cos(PI/8)/2
  if (val<1)
  {
	cout << "stop sign width must be bigger than 1."<<endl;
     return false;
  }
  STOP_SIGN_RADIUS=val*0.58578643762690495119831127578988;
  STOP_SIGN_WIDTH=val;
  return true;
}

bool StopSign::setGlobalBottom(double val)
{
  stopSignBottom=val;
  return true;
}

double StopSign::getGlobalWidth()
{
  return STOP_SIGN_WIDTH;
}

double StopSign::getGlobalBottom()
{
  return stopSignBottom;
}

double StopSign::getGlobalLineDistance()
{
  return lineDistance;
}

double StopSign::getGlobalLineWidth()
{
  return halfLineWidth*2.0;
}

StopSign::StopSign():
  PhysicalObject(0,stopSignBottom+STOP_SIGN_WIDTH/2,0),Rotatable(0)
{

}

StopSign::StopSign(double x,double y,double z,double degreesRotation):
  PhysicalObject(x,y,z),Rotatable(degreesRotation)
{
}

void StopSign::getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
	Rotatable::getActiveXZPointsInRange(getX(),getZ(),getUIRadius(),x,z,
		maxDistance,results);

	PhysicalObject::getActiveXZPointsInRange(x,z,maxDistance,results);
}

void drawPolygon(int windowid,double x,double y,double z,double radius,int numSides)
{
  if (stopSignTexture==NULL)
     return;

  glColor3d(1,1,1);

  glEnable( GL_TEXTURE_2D );

     glBindTexture( GL_TEXTURE_2D, stopSignTexture->getOpenGLTextureName(windowid));

  glBegin(GL_POLYGON);

  if (Application::isLightingEnabled())
	glNormal3d(0,0,1);

double angleOffset=PI/numSides;
//int textureRadius=stopSignTexture->getWidth()/2;

  // loop through vertexes.
  for (int i=0;i<numSides;i++)
  {
   double angle=angleOffset+i*2*PI/numSides;
   double cx,cy,cz;

    cx=x+radius*cos(angle);
    cy=y+radius*sin(angle);
    cz=z;

    glTexCoord2d(0.5*(1+cos(angle)),0.5*(1-sin(angle)));
    glVertex3d(cx,cy,cz);
  }

  glEnd();

  // draw the backboard.
  if (Application::isLightingEnabled())
	glNormal3d(0,0,-1);

 glDisable(GL_TEXTURE_2D);
 glColor3d(0.6,0.6,0.6);
  glBegin(GL_POLYGON);

  // loop through vertexes.
  for (int i=0;i<numSides;i++)
  {
   double angle=angleOffset+i*2*PI/numSides;
   double cx,cy,cz;

    cx=x+radius*cos(angle);
    cy=y+radius*sin(angle);
    cz=z;

    glTexCoord2d(0.5*(1+cos(angle)),0.5*(1-sin(angle)));
    glVertex3d(cx,cy,cz-1);
  }

  glEnd();
}

void StopSign::draw(int windowid,const Vector3D &viewpoint)
{
  glPushMatrix();
  glTranslated(getX(),getY(),getZ());
  glRotated(rotation,0,1,0);
 // glColor3d(0.8,0,0); // red

  drawPolygon(windowid,0,0,0,STOP_SIGN_RADIUS,8);

  glDisable(GL_TEXTURE_2D);
  glColor3d(0.3,0.3,0.3);
  glBegin(GL_QUADS);

  double y=STOP_SIGN_RADIUS*cos(22.5*PI/180);

    glVertex3d(-2,-y,0);
    glVertex3d(-2,-getY(),0);
    glVertex3d(2,-getY(),0);
    glVertex3d(2,-y,0);

  glEnd();

  // draw white line.
  glColor3d(1,1,1);

  double line_spacer=0.05;

  glBegin(GL_QUADS);

      if (Application::isLightingEnabled())
	    glNormal3d(0,1,0);

    glVertex3d(-lineDistance*sideFactor,line_spacer-getY(),lineDistance-halfLineWidth);
    glVertex3d(lineDistance*sideFactor,line_spacer-getY(),lineDistance-halfLineWidth);
    glVertex3d(lineDistance*sideFactor,line_spacer-getY(),lineDistance+halfLineWidth);
    glVertex3d(-lineDistance*sideFactor,line_spacer-getY(),lineDistance+halfLineWidth);

  glEnd();


  glPopMatrix();
}

double StopSign::getUIRadius() const
{
   return 30*10;
}

void StopSign::drawForDesign(int windowid)
{
 int size=30;

  glPushMatrix();

  glTranslated(getX(),getZ(),0);
  glRotated(-rotation,0,0,1);

  glColor3d(1,0,0);
  glBegin(GL_QUADS);

    glVertex2d(-size,-size);
    glVertex2d(size,-size);
    glVertex2d(size,size);
    glVertex2d(-size,size);

  glEnd();

  glPopMatrix();

  Rotatable::drawForDesign(windowid,getX(),getZ(),getUIRadius());

}

std::string StopSign::getName() const
{
	return std::string("StopSign");
}

void StopSign::writeTo(std::ostream &out) const
{
	PhysicalObject::writeTo(out);

	out << " " << rotation;
}

void StopSign::readFrom(std::istream &in)
{
  PhysicalObject::readFrom(in);
   double rot;
   in >> rot;
   Rotatable::setRotation(rot);
}

bool StopSign::setProperty(const std::string &propertyName,double value)
{
	if (PhysicalObject::setProperty(propertyName,value))
		return true;
	if (propertyName=="rotation")
	{
		Rotatable::setRotation(value);
		return true;
	}
	return false;
}
