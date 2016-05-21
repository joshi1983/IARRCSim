#include "TranslateObjectWrapper.hpp"

#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include "../vectors/Rotator3D.hpp"
#include <cstring>


using namespace std;

TranslateObjectWrapper::TranslateObjectWrapper(const Vector3D &v,PhysicalObject *c):
 translation(v),c(c),forHighResOnly(false),xRotation(0),yRotation(0),zRotation(0)
{

}

double TranslateObjectWrapper::getXRotation() const
{
  return xRotation;
}

double TranslateObjectWrapper::getYRotation() const
{
  return yRotation;
}

double TranslateObjectWrapper::getZRotation() const
{
  return zRotation;
}

void TranslateObjectWrapper::setZRotation(double degrees)
{
  this->zRotation = degrees;
}

bool TranslateObjectWrapper::matchesName(const char *name) const
{
  if (strcmp(name,this->name.c_str())==0)
	  return true;
  else
	  return false;
}

void TranslateObjectWrapper::setXRotation(double degrees)
{
  this->xRotation=degrees;
}

void TranslateObjectWrapper::setYRotation(double degrees)
{
  this->yRotation=degrees;
}


void TranslateObjectWrapper::setForHighRes(bool forHighRes)
{
	this->forHighResOnly=forHighRes;
}

bool TranslateObjectWrapper::isForHighResOnly() const
{
  return this->forHighResOnly;
}

const string &TranslateObjectWrapper::getName() const
{
  return this->name;
}

void TranslateObjectWrapper::setName(const string &name)
{
  this->name=name;
}

void TranslateObjectWrapper::draw(int windowid,const Vector3D &viewpoint)
{
  if (forHighResOnly)
  {
    // check distance.
    double distance = viewpoint.distanceFrom(translation);
  	// if distance is more than 1000 than,
    if (distance>5000)
	    return;
  }

  glPushMatrix();
  glTranslated(translation.getX(),translation.getY(),translation.getZ());

  glRotated(zRotation,0,0,1);
  glRotated(yRotation,0,1,0);
  glRotated(xRotation,1,0,0);

  c->draw(windowid,viewpoint);

  glPopMatrix();
}


void TranslateObjectWrapper::drawForDesign(int windowid)
{
  glPushMatrix();
  glTranslated(translation.getX(),translation.getZ(),0);
  glRotated(yRotation,0,0,1);

  c->drawForDesign(windowid);

  glPopMatrix();
}

void TranslateObjectWrapper::getAsLines(std::vector<LineSegment> &lines) const
{
  vector<LineSegment> sublines;

  if (c!=NULL)
	  c->getAsLines(sublines);

  Rotator3D xRot(xRotation*PI/180,1,0,0);
  Rotator3D yRot(yRotation*PI/180,0,1,0);
  Rotator3D zRot(zRotation*PI/180,0,0,1);

  // add the sublines to lines.
  for (unsigned int i=0;i<sublines.size();i++)
  {
     // translate and rotate the line's vertices.
     sublines[i].rotate(xRot);
     sublines[i].rotate(yRot);
	 sublines[i].rotate(zRot);

	 sublines[i].translateBy(translation);

     lines.push_back(sublines[i]);
  }
}
