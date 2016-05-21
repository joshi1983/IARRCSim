#include "Tunnel.hpp"

#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <cmath>

#include "../app/Application.hpp"
#include "../shapes/VerticalRectangle.hpp"
#include "../shapes/HorizontalRectangle.hpp"

using namespace std;

static double tunnelHeight=70;
static double tunnelWidth=200;
static double tunnelLength=500;
static double tunnelMinLength=300;
static double tunnelMaxLength=600;

Tunnel::Tunnel(): PhysicalObject(0,0,0)
,Rotatable(0),length(tunnelLength),width(tunnelWidth),height(tunnelHeight)
{
  initPylons();
}
Tunnel::Tunnel(double x,double y,double z,double rotation):
 PhysicalObject(x,y,z), Rotatable(rotation),length(tunnelLength),
	 width(tunnelWidth),height(tunnelHeight)
{
  initPylons();
}

bool Tunnel::setGlobalWidth(double val)
 {
   if (val<0)
   {
	   cout << "tunnel width must be positive."<<endl;
	 return false;
   }

   tunnelWidth=val;
   return true;
 }

bool Tunnel::setGlobalHeight(double val)
 {
  if (val<1)
  {
	cout << "tunnel height of the tunnel must be bigger than 1."<<endl;
	return false;
  }

   tunnelHeight=val;
   return true;
 }

bool Tunnel::setGlobalLength(double val)
 {
	 if (val<1)
	 {
		 cout << "tunnel length must be bigger than 1."<<endl;
	   return false;
	 }

   tunnelLength=val;
   return true;
 }

bool Tunnel::setGlobalMinLength(double val)
 {
	 if (val<0)
		 return false;

   tunnelMinLength=val;
   return true;
 }

bool Tunnel::setGlobalMaxLength(double val)
 {
	if (val<0)
		 return false;

    tunnelMaxLength=val;
    return true;
 }

double Tunnel::getGlobalWidth()
{
  return tunnelWidth;
}

double Tunnel::getGlobalHeight()
{
  return tunnelHeight;
}

double Tunnel::getGlobalLength()
{
  return tunnelLength;
}

double Tunnel::getGlobalMinLength()
{
  return tunnelMinLength;
}

double Tunnel::getGlobalMaxLength()
{
  return tunnelMaxLength;
}

void Tunnel::transform(Vector3D &v) const
{
  // rotate first.
	Rotatable::rotator.rotateXZ(v);
	v.add(position);
}

void Tunnel::addObstaclePoints(std::list<Vector2D> &points) const
{
  list<Pylon*>::const_iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
	   Vector3D pos = (*it)->getPosition();
        transform(pos);
	   points.push_back(Vector2D(pos.getX(),pos.getZ()));
   }
}

void Tunnel::addShapeRepresentation(ShapeCollection &shapes) const
{
 list<Pylon*>::const_iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
	   Cone * cone = (*it)->getAsCone();
	   Vector3D pos = cone->getPosition();
	   transform(pos);
	   cone->setPosition(pos);

	   shapes.add(cone);
   }

   // add rectangles for the walls of the tunnel.

   Vector3D pos(-width/2,height/2,0);
   transform(pos);
  VerticalRectangle *wall=
	  new VerticalRectangle(pos,length,height,Rotatable::getRotation());

     shapes.add(wall);

   pos.set(width/2,height/2,0);
   transform(pos);
    wall=
	  new VerticalRectangle(pos,length,height,Rotatable::getRotation());

   shapes.add(wall);

   pos.set(0,height,0);
   pos.add(position);

   HorizontalRectangle *roof=
	   new HorizontalRectangle(pos,length,width,Rotatable::getRotation());

   shapes.add(roof);
}

 void Tunnel::initPylons()
{
 int numPylons=8;
 double minRatio=0.3;

  // add the pylons on this ramp.
  for (int i=0;i<numPylons;i++)
  {
     double ratio=minRatio+(1-minRatio)*2*(abs((double)(i-numPylons/2))/(double)numPylons);
	double z=(i-numPylons*0.5)*length/numPylons;

      pylons.push_back(new Pylon(-width*0.4,0,z,ratio));
      pylons.push_back(new Pylon(width*0.4,0,z,ratio));
  }
}

std::string Tunnel::getName() const
{
  return string("Tunnel");
}

void Tunnel::draw(int windowid,const Vector3D &viewpoint)
{
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslated(getX(),getY(),getZ());
  glRotated(rotation,0,1,0);

  bool lightingEnabled = Application::isLightingEnabled();

  // draw the outer cover
  glColor3d(0.7,0.7,0.7);
  glBegin(GL_QUADS);

  // side 1
    if (lightingEnabled)
	    glNormal3d(-1,0,0);

    glVertex3d(-width/2,0,-length/2);
    glVertex3d(-width/2,height,-length/2);
    glVertex3d(-width/2,height,length/2);
    glVertex3d(-width/2,0,length/2);

    if (lightingEnabled)
	    glNormal3d(1,0,0);

    // side 2
    glVertex3d(width/2,0,-length/2);
    glVertex3d(width/2,height,-length/2);
    glVertex3d(width/2,height,length/2);
    glVertex3d(width/2,0,length/2);

        if (lightingEnabled)
	     glNormal3d(0,1,0);

    // top
    glVertex3d(-width/2,height,-length/2);
    glVertex3d(width/2,height,-length/2);
    glVertex3d(width/2,height,length/2);
    glVertex3d(-width/2,height,length/2);

  glEnd();

  // draw the inner cover
  double gap=1;

  // draw inside of the tunnel.

  glColor3d(0,0,0);
  glBegin(GL_QUADS);

    glVertex3d(-width/2+gap,0,-length/2+gap);
    glVertex3d(-width/2+gap,height-gap,-length/2+gap);
    glVertex3d(-width/2+gap,height-gap,length/2-gap);
    glVertex3d(-width/2+gap,0,length/2-gap);

    glVertex3d(width/2-gap,0,-length/2+gap);
    glVertex3d(width/2-gap,height-gap,-length/2+gap);
    glVertex3d(width/2-gap,height-gap,length/2-gap);
    glVertex3d(width/2-gap,0,length/2-gap);

    glVertex3d(-width/2+gap,height-gap,-length/2+gap);
    glVertex3d(width/2-gap,height-gap,-length/2+gap);
    glVertex3d(width/2-gap,height-gap,length/2-gap);
    glVertex3d(-width/2+gap,height-gap,length/2-gap);

  glEnd();

  list<Pylon*>::const_iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
     (*it)->draw(windowid,viewpoint-position);
   }

  glPopMatrix();
}

void Tunnel::drawForDesign(int windowid)
{
  glPushMatrix();

  glTranslated(getX(),getZ(),0);
  glRotated(-rotation,0,0,1);

  glColor3d(0.7,0.7,0.7);
  glBegin(GL_QUADS);

    glVertex2d(-width/2,-length/2);
    glVertex2d(width/2,-length/2);
    glVertex2d(width/2,length/2);
    glVertex2d(-width/2,length/2);

  glEnd();


  glPopMatrix();

  Rotatable::drawForDesign(windowid,getX(),getZ(),getUIRadius());
}

void Tunnel::getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
	Rotatable::getActiveXZPointsInRange(getX(),getZ(),getUIRadius(),x,z,maxDistance,results);
	PhysicalObject::getActiveXZPointsInRange(x,z,maxDistance,results);
}



double Tunnel::getUIRadius() const
{
  return length*2;
}

void Tunnel::writeTo(std::ostream & out) const
{
	PhysicalObject::writeTo(out);
	out << " "<<rotation;
}

void Tunnel::readFrom(std::istream &in)
{
   PhysicalObject::readFrom(in);
   double rot;
   in >> rot;
   Rotatable::setRotation(rot);
}

 Tunnel::~Tunnel()
 {
  // loop through pylons and destroy them.
  list<Pylon*>::iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
     delete (*it);
   }
 }

 bool Tunnel::setProperty(const std::string &propertyName,double value)
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
