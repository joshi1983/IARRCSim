#include "Ramp.hpp"
#include "../io/Socket.hpp"

#include "../stdafx.h"
#include <cmath>

#ifdef _UBUNTU_

#include <GL/freeglut.h>
#include <GL/glu.h>

#else

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include "../shapes/VerticalIsoscelesTriangle.hpp"
#include "../shapes/SlantedRectangle.hpp"
#include "../app/Application.hpp"

using namespace std;

static double globalHeight=25;
static double globalWidth=150;
static double globalLength=400;
const double carSize=30;

bool Ramp::setGlobalHeight(double val)
{
	if (val<0)
		return false;

   globalHeight=val;
     return true;
}

bool Ramp::setGlobalWidth(double val)
{
  if (val<1)
		return false;

   globalWidth=val;
     return true;
}

bool Ramp::setGlobalLength(double val)
{
 if (val<1)
		return false;

  globalLength=val;
  return true;
}

double Ramp::getGlobalHeight()
{
  return globalHeight;
}

double Ramp::getGlobalWidth()
{
  return globalWidth;
}

double Ramp::getGlobalLength()
{
  return globalLength;
}

Ramp::Ramp():InteractiveObject(0,0,0,150000),
 Rotatable(0),width(globalWidth),height(globalHeight),length(globalLength)
{
  initPylons();
}

Ramp::Ramp(double x,double y,double z,double rotation):
 InteractiveObject(x,y,z,150000),Rotatable(rotation)
	 ,width(globalWidth),height(globalHeight),length(globalLength)
{
  initPylons();
}

 Ramp::~Ramp()
 {
  // loop through pylons and destroy them.
  list<Pylon*>::iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
     delete (*it);
   }
 }

void Ramp::ticksPerformed(Environment * e,int milis)
{
  // loop through the pylons and call ticksPerformed on each.
  list<Pylon*>::iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
     (*it)->ticksPerformed(e,milis);
   }
}

void Ramp::reverseTransformXZ(Vector2D &v) const
{
  v.set(v.getX()-position.getX(),v.getY()-position.getZ());

  // reverse rotate.
  Rotatable::reverseRotator.rotate(v);
}

void Ramp::transformXZ(Vector2D &v) const
{

  // reverse rotate.
  Rotatable::rotator.rotate(v);

   v.set(v.getX()-position.getX(),v.getY()-position.getZ());
}

/*
assumed to need transformation
*/
double Ramp::getElevationForXZPoint(const Vector2D &xzPoint) const
{
  Vector2D v(xzPoint);

  // reverse transform
  reverseTransformXZ(v);

	// check if within bounds of this ramp.
     if (abs(v.getX())<width/2.0 && abs(v.getY())<length/2.0+carSize)
	{
		if (abs(v.getY())<length/2.0)
	         return height-(abs(v.getY())/length)*height*2.0;
		else
			return 0.1;
	}

	return 0;
}

void Ramp::initPylons()
{
 int numPylons=8;

  if (length>300)
  {
    numPylons=(int)(length/40);
  }

  // add the pylons on this ramp.
  for (int i=0;i<numPylons;i++)
  {
	double z=(i-numPylons*0.5)*length/numPylons;
	double y=(length-2*abs(z))*height/length+(5.0*height/length);
     Pylon *p=new Pylon(-width*0.4,y,z,0.9);

      pylons.push_back(p);

	 p=new Pylon(width*0.4,y,z,0.9);
      pylons.push_back(p);
  }
}

void Ramp::writeTo(std::ostream & out) const
{
	InteractiveObject::writeTo(out);
	out << " "<<rotation;
}

void Ramp::readFrom(std::istream &in)
{
   InteractiveObject::readFrom(in);
   double rot;
   in >> rot;
   Rotatable::setRotation(rot);
}

string Ramp::getName() const
{
  return string("Ramp");
}

void Ramp::draw(int windowid,const Vector3D &viewpoint)
{
  glPushMatrix();

  glTranslated(getX(),getY(),getZ());
  glRotated(rotation,0,1,0);
  glColor3d(0.3,0.3,0.3);

   // the tops of the ramp
  glBegin(GL_QUADS);

       if (Application::isLightingEnabled())
		glNormal3d(0,length/height,-1);

	  glVertex3d(-width/2,0,-length/2);
       glVertex3d(width/2,0,-length/2);
	  glVertex3d(width/2,height,0);
       glVertex3d(-width/2,height,0);

	  if (Application::isLightingEnabled())
		glNormal3d(0,length/height,1);

	  glVertex3d(-width/2,height,0);
       glVertex3d(width/2,height,0);
	  glVertex3d(width/2,0,length/2);
       glVertex3d(-width/2,0,length/2);

  glEnd();

  // the sides of the ramp.
  glColor3d(0,0,0);
  glBegin(GL_TRIANGLES);

    glVertex3d(-width/2,0,-length/2);
    glVertex3d(-width/2,height,0);
    glVertex3d(-width/2,0,length/2);

    glVertex3d(width/2,0,-length/2);
    glVertex3d(width/2,height,0);
    glVertex3d(width/2,0,length/2);

  glEnd();

  list<Pylon*>::const_iterator it = pylons.begin();

  Vector3D v=viewpoint-position;

   for (;it!=pylons.end();it++)
   {
     (*it)->draw(windowid,v);
   }

  glPopMatrix();
}


void Ramp::drawForDesign(int windowid)
{
  glPushMatrix();

  glTranslated(getX(),getZ(),0);
  glRotated(-rotation,0,0,1);

  glColor3d(0.3,0.3,0.3);
  glBegin(GL_QUADS);

    glVertex2d(-width/2,-length/2);
    glVertex2d(width/2,-length/2);
    glVertex2d(width/2,length/2);
    glVertex2d(-width/2,length/2);

  glEnd();

  // loop through the pylons and call on each.
  list<Pylon*>::const_iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
     (*it)->drawForDesign(windowid);
   }

  glPopMatrix();

  Rotatable::drawForDesign(windowid,getX(),getZ(),getUIRadius());
}

void Ramp::getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
	Rotatable::getActiveXZPointsInRange(getX(),getZ(),getUIRadius(),
		x,z,maxDistance,results);
	PhysicalObject::getActiveXZPointsInRange(x,z,maxDistance,results);
}

double Ramp::getUIRadius() const
{
  return length*2;
}

void Ramp::transform(Vector3D &v) const
{
  // rotate first.
	Rotatable::rotator.rotateXZ(v);
	v.add(position);
}

void Ramp::addObstaclePoints(std::list<Vector2D> &points) const
{
  list<Pylon*>::const_iterator it = pylons.begin();

   for (;it!=pylons.end();it++)
   {
	   Vector3D pos = (*it)->getPosition();
        transform(pos);
	   points.push_back(Vector2D(pos.getX(),pos.getZ()));
   }
}

void Ramp::addShapeRepresentation(ShapeCollection &shapes) const
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
 Vector3D pos(-width/2,getY()+height/2,0);
   rotator.rotateXZ(pos);
   pos.add(position);

   // represent the sides.
   VerticalIsoscelesTriangle *t=new VerticalIsoscelesTriangle(
	   pos,length,height,Rotatable::getRotation());

   shapes.add(t);
   pos.set(width/2,getY()+height/2,0);
    rotator.rotateXZ(pos);
    pos.add(position);

   t=new VerticalIsoscelesTriangle(
	   pos,length,height,Rotatable::getRotation());

   shapes.add(t);

   double slantDegrees=atan2(height,length/2)*180/PI;

   pos.set(0,position.getY()+height/2,length/4);

   Rotatable::rotator.rotateXZ(pos);

   pos.add(position);

    // pythagarian theorem
   double L = sqrt(length*length/4+height*height);

   // add the 2 SlantedRectangles.
   SlantedRectangle *r=new SlantedRectangle(pos,L,width,
	   Rotatable::getRotation(),
	   -slantDegrees);

   shapes.add(r);

   pos.set(0,position.getY()+height/2,-length/4);
   Rotatable::rotator.rotateXZ(pos);
   pos.add(position);

   r=new SlantedRectangle(pos,L,width,
	   Rotatable::getRotation(),
	   slantDegrees);
   shapes.add(r);
}

void Ramp::getTopNormal(const Vector2D &xzPoint,Vector3D &normal) const
{
  Vector2D v(xzPoint);



  // reverse transform
  reverseTransformXZ(v);

	// check if within bounds of this ramp.
     if ((abs(v.getX())<width/2.0) && (abs(v.getY())<length/2.0+carSize))
	{
	  double slope=height*2.0/length;

	   if (v.getY()<-length/2)
	   {
	      normal.set(0,1,-slope*(1+(v.getY()+length/2)/carSize));
	   }
	   else if (v.getY()<-carSize)
	   {
	      normal.set(0,1,-slope);
	   }
	   else if (v.getY()<carSize)
	   {
	      normal.set(0,1,slope*(v.getY()/carSize));
	   }
	   else if (v.getY()<length/2)
	   {
	      normal.set(0,1,slope);
	   }
	   else
	   {
	      normal.set(0,1,slope*(1-(v.getY()-length/2)/carSize));
	   }
	   normal.rotateAroundYBy(-Rotatable::getRotationInRadians());
	   return;
	}

	normal.set(0,1,0);
}

 bool Ramp::setProperty(const std::string &propertyName,double value)
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
