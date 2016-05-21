#include "TrackEditorUI.hpp"
#include "Utilities.hpp"

#include "../physicalobjects/Pylon.hpp"
#include "../physicalobjects/StopSign.hpp"
#include "../physicalobjects/TrafficLight.hpp"
#include "../physicalobjects/Tunnel.hpp"
#include "../physicalobjects/Ramp.hpp"
#include "../editor/MouseMotionListener.hpp"
#include "../robotcontrollers/PylonAvoider.hpp"
#include "../app/Application.hpp"
#include "../vectors/Vector2DInt.hpp"
#include "../graphics/Graphics2D.hpp"

#ifdef _USE_VISUAL_STUDIO_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <cmath>
#include <list>
#include <cstdio>

#define MENU_STATE_DEFAULT -1
#define MENU_STATE_PYLON 0
#define MENU_STATE_STOP_LIGHT 1
#define MENU_STATE_STOP_SIGN 2
#define MENU_STATE_RAMP 3
#define MENU_STATE_TUNNEL 4
#define MENU_STATE_ROBOT_CAR 5
#define MENU_STATE_MOVE 6
#define MENU_STATE_DELETE 7
#define MENU_STATE_HELP 8
#define MENU_STATE_CLOSE_EDITOR 9

using namespace std;

Vector2DInt lastMousePosition;

bool dragging=false;
double zoomFactor=0.1;
double offset_x=0,offset_z=0;
std::list<MouseMotionListener*> *curMouseListeners=NULL;

void freeMouseListeners()
{
  if (curMouseListeners==NULL)
	  return;

  /*
  list<MouseMotionListener*>::iterator it=curMouseListeners->begin();

  for (;it!=curMouseListeners->end();it++)
  {
     delete (*it);
  }
  */

  delete curMouseListeners;
  curMouseListeners=NULL;
}

void setXZ(int x,int y,double *xd,double *zd)
{
 (*xd)=(x-getWindowWidth()/2)/zoomFactor-offset_x;
 (*zd)=(y-getWindowHeight()/2)/zoomFactor-offset_z;
}

void setXZReversed(double xd,double zd,int *x,int *y)
{
  (*x)=(int)(zoomFactor*(xd+offset_x)+getWindowWidth()/2);
  (*y)=(int)(zoomFactor*(zd+offset_z)+getWindowHeight()/2);
}

/**
Counts number of zero bits left of decimal before reaching the first 1.
*/
int countZeroBits(int i,int max)
{
  int result=0;
    for (int c=0;c<max;c++)
    {
      if ((i & 1)==1)
		 break;
	 i=i>>1;
	 result++;
    }
    return result;
}

/**
 Draws lines to help the designer.

 Uses a recursive algorithm for a fractal pattern so it
 stays helpful at multiple zoom factors.
*/
void drawDesignGrid()
{
  int divisionSize=500;
  const double lineWidthC=1.1/zoomFactor;

  glColor3d(0.4,0.4,0.4);
  glBegin(GL_QUADS);

  double dx=getWindowWidth()/2/zoomFactor,dy=getWindowHeight()/2/zoomFactor;

    // loop through lines in display region.
    for (int x=-20;x<20;x++)
    {
	  double lineWidth=lineWidthC+0.7/zoomFactor*countZeroBits(x,4);
	  double x2,y2;
	  setXZ(x*100,0,&x2,&y2);
	  double y3;
       setXZ(x*100,getWindowHeight(),&x2,&y3);

	  y2+=dy;
	  y3+=dy;
       glVertex2d(dx+x*divisionSize,y2);
	  glVertex2d(dx+x*divisionSize+lineWidth,y2);
	  glVertex2d(dx+x*divisionSize+lineWidth,y3);
	  glVertex2d(dx+x*divisionSize,y3);
    }

    // loop through lines in display region.
    for (int y=-20;y<20;y++)
    {
	  double lineWidth=lineWidthC+0.7/zoomFactor*countZeroBits(y,4);
	  double x2,y2;
	  setXZ(0,0,&x2,&y2);
	  double x3;
       setXZ(getWindowWidth(),getWindowHeight(),&x3,&y2);

	  x2+=dx;
	  x3+=dx;
       glVertex2d(x2,y*divisionSize+dy);
	  glVertex2d(x2,y*divisionSize+lineWidth+dy);
	  glVertex2d(x3,y*divisionSize+lineWidth+dy);
	  glVertex2d(x3,y*divisionSize+dy);
    }

  glEnd();
}

void setXZPositionBasedOnMousePoint(PhysicalObject *o,int x,int y)
{
 double xd,zd;

  setXZ(x,y,&xd,&zd);
  o->setXZPosition(xd,zd);
}

void TrackEditorUI::close()
{
  design.setMenuState(MENU_STATE_DEFAULT);
  design.setMenuHoverState(MENU_STATE_DEFAULT);
  dragging=false;
}

void TrackEditorUI::update() const
{
    VideoDevice * cam1 = Application::getMainCamera();
	glutSetWindow(cam1->getWindowID());
	Graphics2D::setupFor2DGraphics();

    glutSetWindowTitle("IARRC Simulator - Track Editor");
}

void TrackEditorUI::launchHelp() const
{
  Utilities::launchRelativeDocument("tracks.thtml");
}

void TrackEditorUI::handleKeypress(unsigned char key,bool special)
{
  switch (key)
  {
     case 27: // escape
	  Application::ExitApplication();
	  break;

	case 'p':
	case 'P':
	   design.setMenuState(MENU_STATE_PYLON);
        break;

	case 'l':
	case 'L':
	   design.setMenuState(MENU_STATE_STOP_LIGHT);
        break;

	case 's':
	case 'S':
	   design.setMenuState(MENU_STATE_STOP_SIGN);
        break;

	case 'r':
	case 'R':
	   design.setMenuState(MENU_STATE_RAMP);
        break;

	case 't':
	case 'T':
	   design.setMenuState(MENU_STATE_TUNNEL);
        break;

	case 'm':
	case 'M':
	   design.setMenuState(MENU_STATE_MOVE);
        break;

	case 'd':
	case 'D':
	   design.setMenuState(MENU_STATE_DELETE);
        break;

	case 'b':
	case 'B':
		design.setMenuState(MENU_STATE_ROBOT_CAR);
		break;

	case '1':
     case 'c':
	case 'C': // simulation view
	   SwitchToSimulationView();
        break;

	case '3':
			SwitchToSensorEditor();
			break;

	case 'h':
	case 'H':
		launchHelp();
		break;
  }
}

void TrackEditorUI::mouseUp(int x,int y)
{
  dragging=false;
  freeMouseListeners();
}

void TrackEditorUI::mouseMove(int x,int y)
{

 if (x<140 && x>=0 && y<design.getMenuHeight() && y>=0)
 {
   design.setMenuHoverState(y/40);
 }
 else
 {
   design.setMenuHoverState(MENU_STATE_DEFAULT);
 }

 if (dragging)
 {
	 if (design.getMenuState()!=MENU_STATE_MOVE)
	 {
		offset_x+=(x-lastMousePosition.x)/zoomFactor;
		offset_z+=(y-lastMousePosition.y)/zoomFactor;
	 }
	 else
	 {
	  if (curMouseListeners!=NULL)
       {
        double dx,dz;

         setXZ(x,y,&dx,&dz);
	    std::list<MouseMotionListener*>::iterator it=curMouseListeners->begin();
	    for (;it!=curMouseListeners->end();it++)
	    {
	       (*it)->mouseMoved(dx,dz);
            //ExitApplication();
	    }
       }
	 }
 }

 lastMousePosition.x=x;
 lastMousePosition.y=y;

}

void TrackEditorUI::mouseDown(int x,int y)
{
 if (x<140 && x>=0 && y<design.getMenuHeight() && y>=0)
 {
   int tempState=y/40;

   if (tempState==MENU_STATE_CLOSE_EDITOR)
      SwitchToSimulationView();
   else if (tempState==MENU_STATE_HELP)
      launchHelp();
   else
       design.setMenuState(y/40);
 }
 else
 {
     PhysicalObject *o=NULL;
	RoboticCar * mainCar = Application::getMainCar();
	Environment & e = Application::getMainEnvironment();
	double radius = 5/zoomFactor;
	double xd,zd;

      setXZ(x,y,&xd,&zd);

	 switch (design.getMenuState())
	 {
	    case MENU_STATE_PYLON:
		    {
	       Pylon *p=new Pylon();
		  // give a random colour.
		  p->setToRandomBrightness();
		  o=p;
		    }
		  break;

	    case MENU_STATE_STOP_LIGHT:
		    o=new TrafficLight();
              break;

	    case MENU_STATE_STOP_SIGN:
		    o=new StopSign();
              break;

	    case MENU_STATE_RAMP:
		    o=new Ramp();
		    break;

	    case MENU_STATE_TUNNEL:
		    o=new Tunnel();
		    break;

	    case MENU_STATE_ROBOT_CAR:
		    {
		    RoboticCar *car2=new RoboticCar(e.getNextCarID());
		      car2->setRobotController(new PylonAvoider(car2));
		      o=car2;
		    }
		    break;

	    case MENU_STATE_DELETE:
		    {
			    list<PhysicalObject*> exceptions;
			    if (mainCar!=NULL)
				    exceptions.push_back(mainCar);

			    ComplexObject *background = e.getBackgroundObject();
			    if (background!=NULL)
				    exceptions.push_back(background);

			    e.deleteObjectsWithinXZdistance(xd,zd,radius,exceptions);
		    }
		    break;

	    case MENU_STATE_MOVE:

		    freeMouseListeners();

		    curMouseListeners=new std::list<MouseMotionListener*>();

		    list<ActiveVector2D> points;

		    e.getActiveXZPointsInRange(xd,zd,radius,points);
		    list<ActiveVector2D>::iterator it = points.begin();

     	    // draw circles around the points.
		    for (;it!=points.end();it++)
		    {
			    MouseMotionListener *listener=(*it).getMouseMotionListener();
			    if (listener!=NULL)
			    {
	 	            curMouseListeners->push_back(listener);

			    }
		    }

		    break;
	 }
	 if (o!=NULL)
	 {
	    setXZPositionBasedOnMousePoint(o,x,y);
	    e.add(o);
	 }
	 dragging=true;
 }
 lastMousePosition.x=x;
 lastMousePosition.y=y;
}

void TrackEditorUI::mouseWheel(short amount)
{
	if (amount>100)
		amount=100;
	else if (amount<-1000)
		amount=-1000;

   zoomFactor=zoomFactor*pow(2,0.05*amount);
}

void TrackEditorUI::draw()
{
 int width=getWindowWidth(),height=getWindowHeight();
 int windowid=glutGetWindow();

 glLoadIdentity();
 glDisable(GL_DEPTH_TEST);
 glScaled(zoomFactor,zoomFactor,zoomFactor);

  glColor3d(0,0,0);
  glBegin(GL_QUADS);

  // fill a black rectangle over the whole display.
    glVertex2d( 0, 0);
    glVertex2d(width/zoomFactor, 0);
    glVertex2d(width/zoomFactor,height/zoomFactor);
    glVertex2d(0,height/zoomFactor);

  glEnd();
  // draw a design helper grid

    glClear( GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslated(offset_x,offset_z,0);

    drawDesignGrid();

 glDisable(GL_DEPTH_TEST);
 double cx,cz; // cursor position in the untranslated coordinate system of the model.

    setXZ(lastMousePosition.x,lastMousePosition.y,&cx,&cz);

    design.draw(windowid,Application::getMainEnvironment(),
	    Application::getMainCar(),(int)(width/zoomFactor),(int)(height/zoomFactor)
	    ,cx,cz);

    glPopMatrix();

}
