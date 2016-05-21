#include "Quad.hpp"

#include "../stdafx.h"
#include "../vectors/Vector2D.hpp"
#include "../app/Application.hpp"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/glut.h>
#include <gl/glu.h>

#endif

const Vector2D textVertices[4]={
  Vector2D(0,0),Vector2D(0,1),
  Vector2D(1,1),Vector2D(1,0)
 };

Quad::Quad(const Vector3D& p1,const Vector3D& p2,
 const Vector3D& p3,const Vector3D& p4)
{
  points[0]=p1;
  points[1]=p2;
  points[2]=p3;
  points[3]=p4;

  updateNormal();
}

void Quad::updateNormal()
{
 Vector3D dir1=points[1]-points[0],dir2=points[2]-points[0];

 normal=dir1;
 normal.crossProduct(dir2);
 normal.makeUnitVector();
}

void Quad::negateNormal()
{
  normal=-normal;
}

void Quad::draw(int windowid,const Vector3D &viewpoint)
{
  glBegin(GL_QUADS);
  bool uselighting=Application::isLightingEnabled();

  	  if (uselighting)
  	     glNormal3d(normal.getX(),normal.getY(),normal.getZ());

    for (int i=0;i<4;i++)
    {
	  glTexCoord2d(textVertices[i].getX(),textVertices[i].getY());

       glVertex3d(points[i].getX(),points[i].getY(),points[i].getZ());
    }

  glEnd();
}

void Quad::drawForDesign(int windowid)
{

  glBegin(GL_QUADS);


    for (int i=0;i<4;i++)
    {
	  glTexCoord2d(textVertices[i].getX(),textVertices[i].getY());
       glVertex2d(points[i].getX(),points[i].getZ());
    }

  glEnd();
}

void Quad::getAsLines(std::vector<LineSegment> &lines) const
{
	for (int i=0;i<=4;i++)
	{
        lines.push_back(LineSegment(points[i%4],points[(i+1)%4]));
	}

}
