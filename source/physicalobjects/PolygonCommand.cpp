#include "PolygonCommand.hpp"

#include "../app/Application.hpp"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

using namespace std;


void PolygonCommand::add(Vector3D v)
{
  vertices.push_back(v);
  updateNormal();
}

void PolygonCommand::updateNormal()
{
  normal.set(0,1,0);
  if (vertices.size()<3)
	return;

  list<Vector3D>::iterator it = vertices.begin();
  Vector3D p1=*it;
  it++;
  Vector3D p2=*it;
  it++;
  Vector3D p3=*it;

  normal=p2-p1;
  normal.crossProduct(p3-p1);
  normal.makeUnitVector();
}

void PolygonCommand::updateRanges()
{
minX=999999;
maxX=-999999;
minY=999999;
maxY=-999999;

  list<Vector3D>::const_iterator it=vertices.begin();

    for (;it!=vertices.end();it++)
    {
       double x=it->getZ(),y=it->getY();

	  if (x<minX)
		  minX=x;
	  if (x>maxX)
		  maxX=x;
	  if (y<minY)
		  minY=y;
	  if (y>maxY)
		  maxY=y;
    }
}

double PolygonCommand::mapX(double x) const
{
  x-=minX;

  return x/(maxX-minX);
}

double PolygonCommand::mapY(double y) const
{
  y-=minY;
  return y/(maxY-minY);
}

void PolygonCommand::draw(int windowid,const Vector3D &viewpoint)
{
  glBegin(GL_POLYGON);

  list<Vector3D>::const_iterator it=vertices.begin();
  bool lightingEnabled=Application::isLightingEnabled();

    if (lightingEnabled)
       glNormal3d(normal.getX(),normal.getY(),normal.getZ());

    for (;it!=vertices.end();it++)
    {
	    // set texture coordinates.
	  glTexCoord2d(mapX(it->getZ()),mapY(it->getY()));

       glVertex3d(it->getX(),it->getY(),it->getZ());
    }

  glEnd();
}

void PolygonCommand::drawForDesign(int windowid)
{
  glBegin(GL_POLYGON);

   list<Vector3D>::const_iterator it=vertices.begin();

    for (;it!=vertices.end();it++)
    {
       glVertex2d(it->getX(),it->getZ());
    }

  glEnd();
}

void PolygonCommand::getAsLines(std::vector<LineSegment> &lines) const
{
   list<Vector3D>::const_iterator it=vertices.begin();
   const Vector3D *first=NULL;
   const Vector3D *prev=NULL;

    for (;it!=vertices.end();it++)
    {
       if (first==NULL)
		  first=&(*it);

	  if (prev!=NULL)
          lines.push_back(LineSegment(*prev,*it));

	  prev=&(*it);
    }

     if (first!=NULL && prev!=NULL && prev!=first)
	{
       // link the very first and last points together.
       lines.push_back(LineSegment(*first,*prev));
	}
}
