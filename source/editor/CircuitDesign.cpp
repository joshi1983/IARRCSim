#include "CircuitDesign.hpp"

#include "../stdafx.h"

#include "../app/Application.hpp"
#include "../graphics/Texture.hpp"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <cstring>
#include <cmath>

#define MENU_ITEM_HEIGHT 40

using namespace std;

int menuState=-1;
int menuHoverState=-1;
Texture *menuTexture;


void CircuitDesign::init()
{
  menuTexture = new Texture("images/editormenu.bmp");
}

CircuitDesign::CircuitDesign()
{

}

int CircuitDesign::getMenuHeight() const
{
  return menuTexture->getHeight();
}

void CircuitDesign::setMenuState(int stateIndex)
{
  menuState=stateIndex;
}

int CircuitDesign::getMenuState() const
{
  return menuState;
}

void CircuitDesign::setMenuHoverState(int stateIndex)
{
  menuHoverState=stateIndex;
}

void CircuitDesign::drawMenu(int windowid) const
{
  glPushMatrix();
  glLoadIdentity();

  menuTexture->draw2D(windowid,0,0);

  glEnable (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (menuHoverState!=menuState)
  {
  glColor4d(0.99,0.99,0.99,0.14);
  glBegin(GL_QUADS);

	glVertex2d(0,menuHoverState*MENU_ITEM_HEIGHT);
	glVertex2d(menuTexture->getWidth(),menuHoverState*MENU_ITEM_HEIGHT);
	glVertex2d(menuTexture->getWidth(),(1+menuHoverState)*MENU_ITEM_HEIGHT);
	glVertex2d(0,(1+menuHoverState)*MENU_ITEM_HEIGHT);

  glEnd();
  }

  glColor4d(0.99,0.99,0.99,0.25);
  glBegin(GL_QUADS);

	glVertex2d(0,menuState*MENU_ITEM_HEIGHT);
	glVertex2d(menuTexture->getWidth(),menuState*MENU_ITEM_HEIGHT);
	glVertex2d(menuTexture->getWidth(),(1+menuState)*MENU_ITEM_HEIGHT);
	glVertex2d(0,(1+menuState)*MENU_ITEM_HEIGHT);

  glEnd();

  glDisable (GL_BLEND);
  glPopMatrix();
}

void drawCircle(double x,double y,double radius)
{
 int numPoints=8;

 glColor3d(0.9,0.9,0.9);
 glBegin(GL_POLYGON);

  // loop through points.
  for (int i=0;i<numPoints;i++)
  {
	  double angle=i*2*PI/numPoints;

	  double vx=x+radius*cos(angle),
		  vy=y+radius*sin(angle);

         glVertex3d(vx,vy,0);
  }

  glEnd();
}

void CircuitDesign::draw(int windowid,Environment&e,
                         RoboticCar*mainCar,int scaledwidth,int scaledheight,double x,double z)
{
	glPushMatrix();

	glTranslated(scaledwidth/2,scaledheight/2,0);

	double radius = scaledwidth*0.005;

	list<ActiveVector2D> points;

	e.getActiveXZPointsInRange(x,z,radius,points);
	list<ActiveVector2D>::iterator it = points.begin();

     // draw circles around the points.
	for (;it!=points.end();it++)
	{
	   // draw a circle centred on ((*it).getX(),(*it).getY())
		drawCircle((*it).getX(),(*it).getY(),radius);
	}

     e.drawForDesign(windowid);

	glPopMatrix();

	drawMenu(windowid);
}

void CircuitDesign::writeTo(std::ostream & out) const
{

}
