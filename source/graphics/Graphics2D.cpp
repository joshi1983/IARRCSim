#include "Graphics2D.hpp"

#include "../app/Application.hpp"
#include "../vectors/Vector2D.hpp"
#include "Rotator2D.hpp"

#include <cmath>

using namespace std;

void Graphics2D::setupFor2DGraphics()
{
  setupFor2DGraphics(getWindowWidth(),getWindowHeight());
}

void Graphics2D::setupFor2DGraphics(int width,int height)
{
// cout << "Graphics2D::setupFor2DGraphics(): view port set to 0,0,"
//	 <<width<<","<<height<<endl;

// GLfloat aspect=(GLfloat)width/(GLfloat)height;

    glDisable(GL_LIGHTING);
    glMatrixMode (GL_PROJECTION);
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho (0, width,height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);

    glClearColor( 0.0f, 0.0f, 0.0f,1);
}

void Graphics2D::drawRectangle(int left,int top,int width,int height)
{
  drawLine(left,top,left+width,top);
  drawLine(left,top,left,top+height);
  drawLine(left+width,top+height,left,top+height);
  drawLine(left+width,top+height,left+width,top);
}

void Graphics2D::fillRectangle(int left,int top,int width,int height)
{
  glBegin(GL_QUADS);

   glVertex2d(left,top);
   glVertex2d(left+width,top);
   glVertex2d(left+width,top+height);
   glVertex2d(left,top+height);

  glEnd();
}

void Graphics2D::fillPolygon(const Vector2DInt * points,int numPoints)
{
  glBegin(GL_POLYGON);

    // loop through the points.
    for (int i=0;i<numPoints;i++)
    {
       glVertex2d(points[i].x,points[i].y);
    }

  glEnd();
}

void Graphics2D::fillPolygon(const Vector2DInt *points,int numPoints,int left,int top,int width,int height)
{
  Vector2DInt *p=new Vector2DInt[numPoints];
  for (int i=0;i<numPoints;i++)
	  p[i]=points[i];

 // fitInRectangle(p,numPoints,left,top,width,height);

  glEnable(GL_SCISSOR_TEST);
  glScissor(left,top+height,width,height);

  fillPolygon(p,numPoints);

  glDisable(GL_SCISSOR_TEST);

  delete p;
}

void Graphics2D::drawLine(double x1,double y1,double x2,double y2,double width)
{
  Vector2D end(x2-x1,y2-y1);
  double len = end.getMagnitude();
  Rotator2D r(
	   atan2(end.getY(),end.getX())
	  );

  Vector2D points[]={
	  Vector2D(0,width),
	  Vector2D(0,-width),
       Vector2D(len,-width),
       Vector2D(len,width)
  };

	glBegin(GL_QUADS);

	 for (int i=0;i<4;i++)
	 {
	   r.rotate(points[i]);

	   glVertex2d(points[i].getX()+x1,points[i].getY()+y1);
	 }

	glEnd();
}
