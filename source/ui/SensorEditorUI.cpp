#include "SensorEditorUI.hpp"

#include "../stdafx.h"
#include "../app/Application.hpp"
#include "../graphics/Texture.hpp"
#include "../graphics/Graphics2D.hpp"
#include "../sensors/UltrasonicSensor.hpp"
#include "../sensors/Lidar.hpp"
#include "../vectors/Vector2D.hpp"
#include "../editor/SensorRotationMouseListener.hpp"
#include "../editor/PerspectiveMouseMotionListener.hpp"
#include "Utilities.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>


#ifdef _USE_VISUAL_STUDIO_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#define MENU_STATE_DEFAULT -1
#define MENU_STATE_ULTRASONIC 0
#define MENU_STATE_LIDAR 1

#define MENU_STATE_MOVE 2
#define MENU_STATE_DELETE 3
#define MENU_STATE_HELP 4
#define MENU_STATE_CLOSE_EDITOR 5

#define MENU_ITEM_HEIGHT 40
#define GRID_LINE_SIZE 2
#define DOT_SIZE 2
#define AXIS_SIZE 1

using namespace std;

const char SENSOR_MENU_FILENAME[]="images/sensormenu.bmp";
const char SENSOR_GRADIANT_FILENAME[]="images/sensorgradiant.bmp";
const char FrontTexture_FILENAME[]="images/text_Front.bmp";
const char TopTexture_FILENAME[]="images/text_Top.bmp";
const char RightTexture_FILENAME[]="images/text_Right.bmp";
const char PerspectiveTexture_FILENAME[]="images/text_Perspective.bmp";

double SensorEditorUI::guide_height=25;
double SensorEditorUI::guide_width=35;
double SensorEditorUI::guide_length=55;

static GLint width=3000,height=3000;
static Texture *menuTexture=NULL;
static Texture *gradiantTexture=NULL;
static Texture *frontTexture=NULL;
static Texture *rightTexture=NULL;
static Texture *topTexture=NULL;
static Texture *perspectiveTexture=NULL;

static int menuState=MENU_STATE_DEFAULT;
static int menuHovered=MENU_STATE_DEFAULT;
static bool dragging=false;
static bool animatePerspectiveRotation=true;
static Vector3D guideshape_points[6][4];

double SensorEditorUI::zoom=3.0;
PerspectiveViewTransformer SensorEditorUI::perspectiveView;

list<MouseMotionListener*> *SensorEditorUI::curMouseListeners=NULL;


bool SensorEditorUI::setGuideHeight(double val)
{
	if (val<0.001)
	{
		cout << "guide-height must be positive."<<endl;
	   return false;
	}

	guide_height=val;
	return true;
}

bool SensorEditorUI::setGuideWidth(double val)
{
	if (val<0.001)
	{
		cout << "guide-width must be positive."<<endl;
	   return false;
	}

	guide_width=val;
	return true;
}

bool SensorEditorUI::setGuideLength(double val)
{
	if (val<0.001)
	{
	   cout << "guide-length must be positive."<<endl;
	   return false;
	}

	guide_length=val;
	return true;
}

void SensorEditorUI::initGuideShape()
{
  guideshape_points[0][0].set(guide_width/2,0,guide_length/2);
  guideshape_points[0][1].set(-guide_width/2,0,guide_length/2);
  guideshape_points[0][2].set(-guide_width/2,0,-guide_length/2);
  guideshape_points[0][3].set(guide_width/2,0,-guide_length/2);

  guideshape_points[1][0].set(guide_width/2,0,guide_length/2);
  guideshape_points[1][1].set(guide_width/2,guide_height,guide_length/2);
  guideshape_points[1][2].set(-guide_width/2,guide_height,guide_length/2);
  guideshape_points[1][3].set(-guide_width/2,0,guide_length/2);

  guideshape_points[2][0].set(guide_width/2,0,guide_length/2);
  guideshape_points[2][1].set(guide_width/2,guide_height,guide_length/2);
  guideshape_points[2][2].set(guide_width/2,guide_height,-guide_length/2);
  guideshape_points[2][3].set(guide_width/2,0,-guide_length/2);

  guideshape_points[3][0].set(guide_width/2,guide_height,guide_length/2);
  guideshape_points[3][1].set(-guide_width/2,guide_height,guide_length/2);
  guideshape_points[3][2].set(-guide_width/2,guide_height,-guide_length/2);
  guideshape_points[3][3].set(guide_width/2,guide_height,-guide_length/2);

  guideshape_points[4][0].set(guide_width/2,0,-guide_length/2);
  guideshape_points[4][1].set(guide_width/2,guide_height,-guide_length/2);
  guideshape_points[4][2].set(-guide_width/2,guide_height,-guide_length/2);
  guideshape_points[4][3].set(-guide_width/2,0,-guide_length/2);

  guideshape_points[5][0].set(-guide_width/2,0,guide_length/2);
  guideshape_points[5][1].set(-guide_width/2,guide_height,guide_length/2);
  guideshape_points[5][2].set(-guide_width/2,guide_height,-guide_length/2);
  guideshape_points[5][3].set(-guide_width/2,0,-guide_length/2);

}

void SensorEditorUI::freeMouseListeners()
{
  if (curMouseListeners==NULL)
	  return;

  list<MouseMotionListener*>::iterator it = curMouseListeners->begin();

  // loop through listeners and delete each one.
  for (;it!=curMouseListeners->end();)
  {
     MouseMotionListener* ml=(*it);
	it++;
     delete ml;
  }

  delete curMouseListeners;
  curMouseListeners=NULL;
}

SensorEditorUI::SensorEditorUI()
{
    perspectiveView.setVerticalRotation(-PI/4);
}

void setGuideHighlightColour()
{
  glColor3d(0.3,0.3,0.3);
}

void setGuideColour()
{
  glColor3d(0.2,0.2,0.2);
}

void SensorEditorUI::drawGuideRectangle(int left,int top,int width,int height)
{
	/* just something to avoid the left side highlighted line
	from appearing when it shouldn't.
	*/
	left--;
	width++;

    setGuideColour();
    Graphics2D::fillRectangle(left,top,width,height);
    setGuideHighlightColour();
    Graphics2D::drawRectangle(left,top,width,height);
}

void SensorEditorUI::mouseUp(int x,int y)
{
  freeMouseListeners();
  dragging=false;
  animatePerspectiveRotation=true;
}

/**
Loads textures
*/
void SensorEditorUI::init()
{
    if (menuTexture==NULL)
        menuTexture=new Texture(SENSOR_MENU_FILENAME);
    if (gradiantTexture==NULL)
       gradiantTexture=new Texture(SENSOR_GRADIANT_FILENAME);

    if (frontTexture==NULL)
	    frontTexture=new Texture(FrontTexture_FILENAME);

    if (topTexture==NULL)
	    topTexture=new Texture(TopTexture_FILENAME);

    if (rightTexture==NULL)
         rightTexture=new Texture(RightTexture_FILENAME);

    if (perspectiveTexture==NULL)
	    perspectiveTexture=new Texture(PerspectiveTexture_FILENAME);
}

/**
Sets the direction based on the specified view and coordinate offset.
(dx,dy) is an offset from another point.
For example, a location where someone is dragging the mouse minus the sensor location.

*/
void SensorEditorUI::setDirection(int dx,int dy,int view, Vector3D &dir)
{
  double a=-atan2((double)dy,(double)dx);
  double r=0;
  double x=dir.getX(),y=dir.getY(),z=dir.getZ();

  switch (view)
  {
  case 0: // no x-axis - right view
       r=sqrt(y*y+z*z);
       dir.setY(r*sin(a));
	  dir.setZ(r*cos(a));
	  break;

  case 1: // no y axis - top view
       r=sqrt(x*x+z*z);
       dir.setX(r*cos(a));
	  dir.setZ(r*sin(a));
       break; // no z axis - front view

  case 2: // no z axis
       r=sqrt(x*x+y*y);
       dir.setX(r*cos(a));
	  dir.setY(r*sin(a));
	  break;
  }

}

void SensorEditorUI::handleKeypress(unsigned char key,bool special)
{
 switch (key)
 {
   case 27: // escape
	  Application::ExitApplication();
	  break;

 case '1':
 case 'C':
 case 'c':
	 SwitchToSimulationView();
	 break;

 case 'L':
 case 'l':
      menuState=MENU_STATE_LIDAR;
	 break;

 case 'U':
 case 'u':
      menuState=MENU_STATE_ULTRASONIC;
	 break;

 case 'M':
 case 'm':
      menuState=MENU_STATE_MOVE;
	 break;

 case 'D':
 case 'd':
      menuState=MENU_STATE_DELETE;
	 break;

 case '2':
 case 'e':
	 SwitchToTrackEditor();
	 break;

 case 'p': // for whatever reason, p comes in when F1 is hit.
 case 'h':
 case 'H':
	 Utilities::launchRelativeDocument("sensorEditorUI.thtml");
	 break;
 }
}

void SensorEditorUI::viewPointTo3D(double dx,double dy,int view,Vector3D &p)
{
 int left,top,width,height;
  getViewRectangle(left,top,width,height,view);

  dx=(dx-left -width/2)/zoom;
  dy=-(dy-top - height/2)/zoom;

    if (view!=0) // not ignoring x.(top or front)
     p.setX(dx);

  if (view==0) // right side view
  {
     p.setY(dy);
	p.setZ(dx);
  }

  if (view==2) // front view
     p.setY(dy);

  if (view==1) // top view
    p.setZ(dy);
}

void SensorEditorUI::getCoordinatesFromView(double &dx,double &dy,int view) const
{
	dx=0;
	dy=0;
	  if (menuTexture==NULL || gradiantTexture==NULL)
		  return;

       int menuWidth=menuTexture->getWidth()+gradiantTexture->getWidth();
	  double halfWidth=(width-menuWidth)/2;

	  // 3D spacial coordinates.
	     if (view==1)
		{ // top view
		   dx=menuWidth+halfWidth*0.5;
		   dy=height*0.25;
		}
	     else if (view==2)
		{ // front view
			dx=menuWidth+halfWidth*0.5;
			dy=height*0.75;
		}
		else
		{ // right view
               dx=menuWidth+halfWidth*1.5;
			dy=height*0.75;
		}
}

void SensorEditorUI::mouseMove(int x,int y)
{
  if (menuTexture!=NULL)
  {
    int menuWidth = menuTexture->getWidth();
    int menuHeight = menuTexture->getHeight();

      if (x<menuWidth && y<menuHeight)
	 {
	   menuHovered=y/MENU_ITEM_HEIGHT;
	 }
	 else
		 menuHovered=MENU_STATE_DEFAULT;

     if (dragging && curMouseListeners!=NULL)
	{
	 std::list<MouseMotionListener*>::iterator it=curMouseListeners->begin();

	    for (;it!=curMouseListeners->end();it++)
	    {
	       (*it)->mouseMoved(x,y);
	    }

	}
  }
}

bool isDeletable(const Sensor* sensor)
{
  if (sensor->getType()=="lidar" || sensor->getType()=="ultrasonic")
	  return true;
  else
	  return false;
}

bool SensorEditorUI::isWithinBoundaries(double x,double y,double z,
			int view,const Sensor* sensor) const
{
 const Vector3D & pos=sensor->getPosition();
 Vector2D p1;
 Vector2D p2;
  double maxDistance=5/zoom;

     switch (view)
	{
	 case 0: // no x.
          p1.set(y,z);
		p2.set(pos.getY(),pos.getZ());
		break;

	 case 1: // no y.
          p1.set(x,z);
		p2.set(pos.getX(),pos.getZ());
		break;

	 case 2: // no z.
          p1.set(x,y);
		p2.set(pos.getX(),pos.getY());
		break;
	}

	double dist=p1.getDistanceFrom(p2);
	if (dist<maxDistance)
	{ // put on list for deleting.
	  return true;
	}

	return false;
}

/**
@param view
 0 - right (y,z)
 1 - top (x,z)
 2 - front (x,y)
*/
void SensorEditorUI::deleteSensors(double x,double y,double z,int view)
{
 // loop through the sensors.
 list<Sensor*> sensors = Application::getAllSensors().getListCopy();
 list<Sensor*>::const_iterator it = sensors.begin();
 list<string> namesToDelete;

   for (;it!=sensors.end();it++)
   {
	 // skip if the sensor is not allowed to be deleted.
	 if (!isDeletable(*it))
	     continue;

	if (isWithinBoundaries(x,y,z,view,*it))
	{ // put on list for deleting.
	   namesToDelete.push_back((*it)->getName());
	}
   }
   list<string>::iterator it2=namesToDelete.begin();
   for (;it2!=namesToDelete.end();it2++)
   {
	   Application::getAllSensors().remove(*it2);
   }
}

void SensorEditorUI::getPositionInformation(int x,int y,
	double &x3d,double &y3d,double &z3d,int &view) const
{
	  x3d=y3d=z3d=0;
	  view=0;

	  if (menuTexture==NULL || gradiantTexture==NULL)
		  return;

       int menuWidth=menuTexture->getWidth()+gradiantTexture->getWidth();
	  double halfWidth=(width-menuWidth)/2;
	  x-=menuWidth;
       bool inLeftRegion=x<halfWidth;

	  // 3D spacial coordinates.
        if (y<height/2)
	   {
	     if (inLeftRegion)
		{ // top view
		   x3d=(x-halfWidth*0.5)/zoom;
		   z3d=-(y-height*0.25)/zoom;
		   view=1;
		}
		else
			view=3;
	   }
	   else
	   {
	     if (inLeftRegion)
		{ // front view
			x3d=(x-halfWidth*0.5)/zoom;
			y3d=-(y-height*0.75)/zoom;
			view=2;
		}
		else
		{ // right view
               z3d=(x-halfWidth*1.5)/zoom;
			y3d=-(y-height*0.75)/zoom;
			view=0;
		}
	   }

}

void SensorEditorUI::mouseDown(int x,int y)
{
  if (menuTexture!=NULL)
  {
    int menuWidth = menuTexture->getWidth();
    int menuHeight = menuTexture->getHeight();

     if (x<menuWidth)
	{
      if (y<menuHeight)
	 {
        int tempState=y/MENU_ITEM_HEIGHT;
        if (tempState==MENU_STATE_CLOSE_EDITOR)
	   {
	      SwitchToSimulationView();
	   }
	   else if (tempState==MENU_STATE_HELP)
 	        Utilities::launchRelativeDocument("sensorEditorUI.thtml");
	   else
		   menuState=tempState;
	 }
	}
	else if (gradiantTexture!=NULL)
	{ // clicking in one of the views.

		menuWidth+=gradiantTexture->getWidth();

        double x3d,y3d,z3d;
	   int view;

	   getPositionInformation(x,y,x3d,y3d,z3d,view);
	   if (view==3)
	   { // perspective
		    freeMouseListeners();
		     curMouseListeners = new list<MouseMotionListener*>();
		     curMouseListeners->push_back(
				new PerspectiveMouseMotionListener(x,y,&perspectiveView));
			animatePerspectiveRotation=false;
			dragging=true;
			return;
	   }

	   if (menuState==MENU_STATE_ULTRASONIC || menuState==MENU_STATE_LIDAR)
	   {
		   Sensor *s=NULL;
          if (menuState==MENU_STATE_ULTRASONIC)
	     {
	       // add a new ULTRASONIC sensor with a unique name.
		   string name=Application::getAllSensors().getNewSensorName("ultra");
		   s=new UltrasonicSensor(name);
	     }
	     else if (menuState==MENU_STATE_LIDAR)
	     {
	       // add a new Lidar sensor with a unique name.
		   string name=Application::getAllSensors().getNewSensorName("lidar");
		   s=new Lidar(name);
	     }
		s->setProperty("position-x",x3d);
		s->setProperty("position-y",y3d);
		s->setProperty("position-z",z3d);
		Application::getAllSensors().add(s);
	   }
	   else if (menuState==MENU_STATE_DELETE)
	   {
	      // delete the ones nearby.
		   deleteSensors(x3d,y3d,z3d,view);
	   }
	   else if (menuState==MENU_STATE_MOVE)
	   {
	     // loop through all sensors.
		   freeMouseListeners();
		   curMouseListeners = new list<MouseMotionListener*>();


		   const list<Sensor*> & sensors = Application::getAllSensors().getListCopy();
		   list<Sensor*>::const_iterator it = sensors.begin();
           for (;it!=sensors.end();it++)
		   {
		       // if the sensor can't be moved in the sensor editor, don't do anything with it.
		       if (!(*it)->isMovable())
		          continue;

                // get distance from the sensor.
                if (isWithinBoundaries(x3d,y3d,z3d,view,*it))
			 {
			   // create a new
				 SensorRepositionMouseListener * listener = new
                       SensorRepositionMouseListener(*it,view);

				 curMouseListeners->push_back(listener);
			 }
			 // if it is an ultrasonic sensor, check if clicking tip of point.
			 UltrasonicSensor * us=dynamic_cast<UltrasonicSensor*>(*it);
			 if (us!=NULL)
			 {
			   // where is the tip of the line?
			   double x2,y2;

                  getUltrasonicSensorDirectionTip(
	                *us,view,x2,y2);
			   x2-=x;
			   y2-=y;
			   double distance = sqrt(x2*x2+y2*y2);
                  if (distance<3)
			   { // rotation listener must be added.
			     SensorRotationMouseListener * listener = new
                       SensorRotationMouseListener(us,view);

				 curMouseListeners->push_back(listener);
			   }
			 }
		   }
	   }

	}
  }
  dragging=true;
}

void SensorEditorUI::mouseWheel(short amount)
{
   zoom*=pow(2,amount*0.05);
}

void drawDot(double x,double y)
{
	glBegin(GL_QUADS);

	 glVertex2d(x-DOT_SIZE,y-DOT_SIZE);
	 glVertex2d(x-DOT_SIZE,y+DOT_SIZE);
	 glVertex2d(x+DOT_SIZE,y+DOT_SIZE);
	 glVertex2d(x+DOT_SIZE,y-DOT_SIZE);

	glEnd();
}

void setToThemeColour()
{
  glColor3d(84/255.0,109/255.0,142/255.0);
}

void setColourFor(const Sensor * sensor)
{
 string type=sensor->getType();
 double r,g,b;

   if (type=="lidar")
   {
      r=0.1;g=0.5;b=1;
   }
   else if (type=="video")
   {
     r=0;g=1;b=0;
   }
   else if (type=="ultrasonic")
   {
     r=1;g=0;b=0;
   }
   else
   {
     r=1;g=1;b=1;
   }

   glColor3d(r,g,b);
}

void drawHorizontalAxis(int x,int y,int width)
{
  setToThemeColour();
  glBegin(GL_QUADS);

   glVertex2d(x,y-AXIS_SIZE);
   glVertex2d(x+width,y-AXIS_SIZE);
   glVertex2d(x+width,y);
   glVertex2d(x,y);

  glEnd();
}

void drawVerticalAxis(int x,int y,int height)
{
  setToThemeColour();
  glBegin(GL_QUADS);

   glVertex2d(x,y);
   glVertex2d(x+AXIS_SIZE,y);
   glVertex2d(x+AXIS_SIZE,y+height);
   glVertex2d(x,y+height);

  glEnd();
}

bool inRange(int x,int y,int left,int top,int width,int height)
{
  if (x<left)
	  return false;
  if (x>left+width)
	  return false;
  if (y<top)
	  return false;
  if (y>top+height)
	  return false;

  return true;
}

void SensorEditorUI::getViewRectangle(
  int &left,int &top,int &w,int &h,int view)
{
  int menuWidth;
  if (menuTexture==NULL)
	  menuWidth=0;
  else
      menuWidth=menuTexture->getWidth();

  if (gradiantTexture!=NULL)
	  menuWidth+=gradiantTexture->getWidth();

  w=(width-menuWidth)/2;
  h=height/2;
  if (view==1 || view==3)
	  top=0;
  else
      top=height/2;

  int w2=width-menuWidth;

  if (view==0 || view==3)
	  left=menuWidth+w2/2;
  else
	  left=menuWidth;
}

bool SensorEditorUI::convertToViewPoint(
	const Sensor* sensor,int view,double &x,double &y)
{
  return convertToViewPoint(sensor->getPosition(),
    view,x,y);
}

bool SensorEditorUI::convertToViewPoint(const Vector3D &pos,int view,
		double &x,double &y)
{
  int left, top, width, height;
    getViewRectangle(
      left,top,width,height,view);

   if (view==3)
   {
	 int xI,yI;
      bool result=perspectiveView.transform(pos,left+width/2,top+height/2,xI,yI);

	   x=xI;
	   y=yI;
	   return result;
   }
	if (view==0)
	{
         x=pos.getZ()*zoom+left+width/2;
         y=-pos.getY()*zoom+top+height/2;
	}
	else if (view==1)
	{
		x=pos.getX()*zoom+left+width/2;
		y=-pos.getZ()*zoom+top+height/2;
	}
	else if (view==2)
	{
	     x=pos.getX()*zoom+left+width/2;
          y=-pos.getY()*zoom+top+height/2;
	}
  return true;
}

bool SensorEditorUI::getUltrasonicSensorDirectionTip(
	const UltrasonicSensor &us,int view,double &x2,double &y2)
{
	Vector3D dir = us.getDirection()*15/zoom;
	Vector3D pos=us.getPosition();

        pos.add(dir);

	   return convertToViewPoint(pos,view,x2,y2);
}

void SensorEditorUI::drawSensor(int view,const Sensor *s) const
{
	int left, top, width, height;

     // don't draw sensors unless they can be changed in the sensor editor.
    if (!s->isMovable())
       return;

	getViewRectangle(left,top,width,height,view);

     Vector3D pos = s->getPosition();

     double x,y;

     bool useResult=convertToViewPoint(pos,view,x,y);

     if (useResult)
	{
		// draw a dot.
		setColourFor(s);
		drawDot(x,y);

		//cout << "dot location: x="<<x<<", y="<<y<<endl;

		const UltrasonicSensor *us=dynamic_cast<const UltrasonicSensor*>(s);

		if (us!=NULL) // if is an ultrasonic sensor, draw the direction line.
		{
            double x2,y2;
		   useResult=getUltrasonicSensorDirectionTip(
	          *us,view,x2,y2);

		     // draw line.
		     Graphics2D::drawLine(x,y,x2,y2,1);
		}
	}

}

/**
 Makes sure the rectangle described by (L2,T2,w,h)
 is within the boundaries of (left,top,width,height)
*/
void enforceRectangleLimits(int left,int top,int width,int height,
  int &L2,int &T2,int &w,int &h)
{
  if (L2<left)
  {
     int dl=left-L2;
     w-=dl;
	L2=left;
  }
  if (T2<top)
  {
     int dt=top-T2;
     h-=dt;
	T2=top;
  }

  if (L2+w>left+width)
	  w=left+width-L2;
  if (T2+h>top+height)
	  h=top+height-T2;
  if (w<0)
	  w=0;
  if (h<0)
	  h=0;
}

void SensorEditorUI::fillPerspectivePolygon(
	const Vector3D *points,int numPoints)
{
 Vector2DInt *points2d=(Vector2DInt*)malloc(sizeof(Vector2DInt)*numPoints);

  if (points2d==NULL)
      return;


  for (int i=0;i<numPoints;i++)
  {
    double dx,dy;
    bool useResult=convertToViewPoint(points[i],3,dx,dy);
     if (!useResult)
		return;

      points2d[i].set((int)dx,(int)dy);
  }

 int left,top,width,height;
 getViewRectangle(left,top,width,height,3);

 Graphics2D::fillPolygon(points2d,numPoints);
 free(points2d);
}

void SensorEditorUI::drawPerspectivePolygon(const Vector3D *points,int numPoints)
{
  Vector2DInt prevPoint;
  double dx,dy;
  bool useResult=convertToViewPoint(points[0],3,dx,dy);
  prevPoint.set((int)dx,(int)dy);
  if (!useResult)
	  return;

  int left,top,width,height;
  getViewRectangle(left,top,width,height,3);


  for (int i=1;i<=numPoints;i++)
  {
    double dx,dy;
    bool useResult=convertToViewPoint(points[i%numPoints],3,dx,dy);
    if (!useResult)
	    return;

    Graphics2D::drawLine(prevPoint.x,prevPoint.y,dx,dy);

     prevPoint.set((int)dx,(int)dy);
  }


}

void SensorEditorUI::drawPerspectiveView(int windowid,const std::list<Sensor*> & sensors)
{
 int left,top,width,height;

  getViewRectangle(left,top,width,height,3);

    glEnable(GL_SCISSOR_TEST);
  glScissor(left,top+height,width,height);

 list<Sensor*>::const_iterator it = sensors.begin();

  if (perspectiveTexture!=NULL)
     perspectiveTexture->draw2D(windowid,left,top);

 drawVerticalAxis(left+width/2,top,height);

  setGuideColour();

 // draw the cube.
  for (int i=0;i<6;i++)
     fillPerspectivePolygon(guideshape_points[i],4);

  setGuideHighlightColour();
  for (int i=0;i<6;i++)
     drawPerspectivePolygon(guideshape_points[i],4);

  // loop through sensors.
  for (;it!=sensors.end();it++)
  {
     // draw the sensor on the display area.
     drawSensor(3,*it);
  }

   glDisable(GL_SCISSOR_TEST);
}

/**
 Draws the top view in the specified rectangle
*/
void SensorEditorUI::drawTop(int windowid,const list<Sensor*> & sensors,
	int left,int top,int width,int height) const
{
 list<Sensor*>::const_iterator it = sensors.begin();

  glEnable(GL_SCISSOR_TEST);
 glScissor(left,top+height,width,height);

  if (topTexture!=NULL)
     topTexture->draw2D(windowid,left,top);

  int L2,T2,w=(int)(guide_width*zoom),h=(int)(guide_length*zoom);
  L2=left+width/2-w/2;
  T2=top+height/2-h/2;
  enforceRectangleLimits(left,top,width,height,L2,T2,w,h);

  drawGuideRectangle(L2,T2,w,h);

  drawHorizontalAxis(left,top+height/2,width);
  drawVerticalAxis(left+width/2,top,height);

  // loop through sensors.
  for (;it!=sensors.end();it++)
  {
     // draw the sensor on the display area.
     drawSensor(1,*it);
  }
  glDisable(GL_SCISSOR_TEST);
}

void SensorEditorUI::drawFront(int windowid,const std::list<Sensor*> & sensors,
					    int left,int top,int width,int height) const
{
  glEnable(GL_SCISSOR_TEST);
 glScissor(left,0,width,height);
 list<Sensor*>::const_iterator it = sensors.begin();

   if (frontTexture!=NULL)
     frontTexture->draw2D(windowid,left,top);

  int L2,T2,w=(int)(guide_width*zoom),h=(int)(guide_height*zoom);
  L2=left+width/2-w/2;
  T2=top+height/2-h;
  enforceRectangleLimits(left,top,width,height,L2,T2,w,h);

  drawGuideRectangle(L2,T2,w,h);

   drawHorizontalAxis(left,top+height/2,width);
  drawVerticalAxis(left+width/2,top,height);

  // loop through sensors.
  for (;it!=sensors.end();it++)
  {
     drawSensor(2,*it);
  }
  glDisable(GL_SCISSOR_TEST);
}

void SensorEditorUI::drawRight(int windowid,const std::list<Sensor*> & sensors,
	int left,int top,int width,int height) const
{
   list<Sensor*>::const_iterator it = sensors.begin();
// cout << "left: "<<left<<", top: "<<top<<", width: "<<
//	 width<<", height: "<<height<<endl;

  // glViewport(left,top,width,height);
   glEnable(GL_SCISSOR_TEST);
   glScissor(left,0,width,height);
   if (rightTexture!=NULL)
     rightTexture->draw2D(windowid,left,top);

  int L2,T2,w=(int)(guide_length*zoom),h=(int)(guide_height*zoom);
  L2=left+width/2-w/2;
  T2=top+height/2-h;
  enforceRectangleLimits(left,top,width,height,L2,T2,w,h);

  drawGuideRectangle(L2,T2,w,h);

   drawHorizontalAxis(left,top+height/2,width);
  drawVerticalAxis(left+width/2,top,height);

  // loop through sensors.
  for (;it!=sensors.end();it++)
  {
     drawSensor(0,*it);
  }
  glDisable(GL_SCISSOR_TEST);
  VideoDevice *cam = Application::getMainCamera();
  if (cam!=NULL)
     glViewport(0,0,cam->getResolutionWidth(),cam->getResolutionHeight());
}

void SensorEditorUI::draw()
{
    VideoDevice * cam = Application::getMainCamera();
    int windowid = cam->getWindowID();

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (animatePerspectiveRotation)
	{
  	   perspectiveView.setHorizontalRotation(
	  	  perspectiveView.getHorizontalRotation()+0.002);
	}

	// fill everything with black.
	glColor3d(0,0,0);

  glBegin(GL_QUADS);

    glVertex2d( 0, 0);
    glVertex2d(width, 0);
    glVertex2d(width,height);
    glVertex2d(0,height);

  glEnd();

	if (menuTexture!=NULL)
	{ // draw the menu.
		menuTexture->draw2D(windowid,0,0);
		double left2=menuTexture->getWidth();

         if (gradiantTexture!=NULL)
   	    { // draw the gradiant.
		 double left1=gradiantTexture->getWidth()+menuTexture->getWidth();

		 double mid=(width-left1)/2+left1;

		 const list<Sensor*> & sensors= Application::getAllSensors().getListCopy();
		 drawPerspectiveView(windowid,sensors);
		 drawTop(windowid,sensors,(int)left1,0,(int)(mid-left1),height/2);
		 drawFront(windowid,sensors,(int)left1,height/2,mid-left1,height/2);
		 drawRight(windowid,sensors,(int)mid,height/2,(int)(mid-left1),height/2);

		 gradiantTexture->stretchDraw(windowid,menuTexture->getWidth(),0,
			 gradiantTexture->getWidth(),height);
           gradiantTexture->stretchDraw(windowid,menuTexture->getWidth(),
			 menuTexture->getHeight(),
			 -gradiantTexture->getWidth(),height-menuTexture->getHeight());

		 // draw the grid.
		 setToThemeColour();

		 glBegin(GL_QUADS);

              glVertex2d(left2,height/2-GRID_LINE_SIZE);
		    glVertex2d(width,height/2-GRID_LINE_SIZE);
		    glVertex2d(width,height/2+GRID_LINE_SIZE);
		    glVertex2d(left2,height/2+GRID_LINE_SIZE);

              glVertex2d(mid-GRID_LINE_SIZE,0);
		    glVertex2d(mid+GRID_LINE_SIZE,0);
		    glVertex2d(mid+GRID_LINE_SIZE,height);
		    glVertex2d(mid-GRID_LINE_SIZE,height);

		 glEnd();
	    }

	    if (menuHovered>=0 || menuState>=0)
	    {
		    // show the selected menu item.
		    glEnable (GL_BLEND);
		    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		    glColor4d(0.0,0.99,0.0,0.25);

		    glBegin(GL_QUADS);

		    glVertex2d(0,menuState*MENU_ITEM_HEIGHT);
		    glVertex2d(left2,menuState*MENU_ITEM_HEIGHT);
		    glVertex2d(left2,(menuState+1)*MENU_ITEM_HEIGHT);
		    glVertex2d(0,(menuState+1)*MENU_ITEM_HEIGHT);

		    glEnd();

		    if (menuHovered!=menuState)
		    {
			    glColor4d(0.99,0.99,0.99,0.14);

			    glBegin(GL_QUADS);

			    glVertex2d(0,menuHovered*MENU_ITEM_HEIGHT);
			    glVertex2d(left2,menuHovered*MENU_ITEM_HEIGHT);
			    glVertex2d(left2,(menuHovered+1)*MENU_ITEM_HEIGHT);
			    glVertex2d(0,(menuHovered+1)*MENU_ITEM_HEIGHT);

			    glEnd();
		    }
		    glDisable(GL_BLEND);
	    }
	}
}

void SensorEditorUI::update() const
{
 width=getWindowWidth();
 height=getWindowHeight();

 initGuideShape();

 Graphics2D::setupFor2DGraphics();
 VideoDevice * cam1 = Application::getMainCamera();

   glutSetWindow(cam1->getWindowID());
   glutSetWindowTitle("IARRC Simulator - Sensor Layout Editor");

   init();
}

void SensorEditorUI::close()
{
  freeMouseListeners();

   menuState=MENU_STATE_DEFAULT;
   menuHovered=MENU_STATE_DEFAULT;

}
