#include "ComplexObject.hpp"
#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <sstream>
#include "../shapes/ZeroPlane.hpp"

using namespace std;

ComplexObject::ComplexObject(): PhysicalObject(0,0,0)
 ,latitudeDegrees(0),longitudeDegrees(0),geoRotationDegrees(0),
 boundary_XRange(999999),boundary_ZRange(999999)
{

}

void ComplexObject::setBoundaries(double xRange,double zRange)
{
  this->boundary_XRange = xRange;
  this->boundary_ZRange = zRange;
}

TranslateObjectWrapper * ComplexObject::getWrapperByName(const char *name)
{
	if (name[0]=='\0')
		return NULL;

  	list<GCommand*>::iterator it=commands.begin();

	for (;it!=commands.end();it++)
	{
	   if ((*it)->matchesName(name))
	   {
	        return dynamic_cast<TranslateObjectWrapper *>(*it);
	   }
	}

   return NULL;
}

void ComplexObject::add(GCommand*gc)
{
  commands.push_back(gc);
}

void ComplexObject::draw(int windowid,const Vector3D &viewpoint)
{
	glColor3d(1,1,1);

	list<GCommand*>::const_iterator it=commands.begin();
	for (;it!=commands.end();it++)
	{
	   (*it)->draw(windowid,viewpoint);
	}
	glDisable(GL_TEXTURE);
}

void ComplexObject::drawForDesign(int windowid)
{
	glColor3d(1,1,1);

	list<GCommand*>::const_iterator it=commands.begin();
	for (;it!=commands.end();it++)
	{
	   (*it)->drawForDesign(windowid);
	}
	glDisable(GL_TEXTURE);
}

void ComplexObject::writeTo(std::ostream &out) const
{
 // do nothing.
}

void ComplexObject::addShapeRepresentation(ShapeCollection &shapes) const
{
  shapes.add(new ZeroPlane());
}

void ComplexObject::setGeoOriginAndOrientation(
		double latitude,double longitude,double rotationAngle)
{
  latitudeDegrees=latitude;
  longitudeDegrees=longitude;
  geoRotationDegrees=rotationAngle;
}

double ComplexObject::getLatitudeDegrees() const
{
  return latitudeDegrees;
}

double ComplexObject::getLongitudeDegrees() const
{
  return longitudeDegrees;
}

double ComplexObject::getGeoRotationDegrees() const
{
  return geoRotationDegrees;
}

void ComplexObject::getAsLines(std::vector<LineSegment> &lines) const
{
		// built up a collection of LineSegments.
	list<GCommand*>::const_iterator it = commands.begin();
    for (;it!=commands.end();it++)
	{
		(*it)->getAsLines(lines);
	}
}

std::string ComplexObject::getAsLines(const std::string &format) const
{
 ostringstream result;

     vector<LineSegment> lines;

     getAsLines(lines);


  if (format=="JSON")
     result<<"["<<endl;
  else if (format=="xml")
	  result<<"<lines>"<<endl;


	// loop through the collection of LineSegment and
	// add the formatted version of it to the result.
     for (unsigned int i=0;i<lines.size();i++)
	{
		if (i!=0)
		{
             if (format=="JSON")
			  result<<",";

		   result<<endl;
		}

		result<<lines[i].getInFormat(format);
	}

  if (format=="JSON")
	result<<"]"<<endl;
  else if (format=="xml")
	  result<<"</lines>";

  return result.str();
}

double ComplexObject::getBoundaryXRange() const
{
   return boundary_XRange;
}

double ComplexObject::getBoundaryZRange() const
{
   return boundary_ZRange;
}

void ComplexObject::getWrappersByName(const char *name,std::list<TranslateObjectWrapper *> & wrappers)
{
	if (name[0]=='\0')
		return;

  	list<GCommand*>::iterator it=commands.begin();

	for (;it!=commands.end();it++)
	{
	   if ((*it)->matchesName(name))
	   {
	        wrappers.push_back(dynamic_cast<TranslateObjectWrapper *>(*it));
	   }
	}
}

