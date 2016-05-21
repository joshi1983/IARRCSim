#include "SimulationUI.hpp"

#include "../stdafx.h"

#ifdef _USE_VISUAL_STUDIO_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <cmath>
#include "Utilities.hpp"
#include "../app/Application.hpp"
#include "../physicalobjects/RoboticCar.hpp"
#include "../app/Environment.hpp"
#include "../sensors/VideoDevice.hpp"
#include "../vectors/Vector2DInt.hpp"
#include <cstring>
#include <cstdio>

using namespace std;

static bool dragging=false;
static Vector2DInt lastPosition;
static GLfloat light_position[] = { 0, 0.6, 1, 0.0 };

void drawSkybox()
{

}

void updateLightPosition()
{
 const Vector3D pos=Application::getLightDirection();

  light_position[0]=(GLfloat)pos.getX();
  light_position[1]=(GLfloat)pos.getY();
  light_position[2]=(GLfloat)pos.getZ();
}

void SimulationUI::update() const
{
   update(Application::getMainCamera());
}

void SimulationUI::update(VideoDevice * cam1) const
{
   int windowid = cam1->getWindowID();
    glutSetWindow(windowid);

	updateLightPosition();
//	cout << "SimulationUI update() called."<<endl;

 GLint width=cam1->getResolutionWidth(),height=cam1->getResolutionHeight();
 GLfloat aspect=(GLfloat)width/(GLfloat)height;

#ifndef _USE_VISUAL_STUDIO_

   glutInitDisplayMode( GL_DOUBLE | GL_RGB | GL_DEPTH);

#endif

    glViewport(0, 0, width,height);
    glMatrixMode(GL_PROJECTION);


    char temp[200];
    sprintf(temp,"IARRC Simulator - %s",cam1->getName().c_str());
    glutSetWindowTitle(temp);

    double fov_degrees = VideoDevice::getGlobalFieldOfViewDegrees();
    if (cam1!=NULL)
    {
	    fov_degrees = cam1->getFieldOfViewDegrees();
    }
    glLoadIdentity();

    gluPerspective(fov_degrees, aspect, 5.0, 35000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    glClearColor( 0.5f, 0.5f, 0.8f,0);
    glColor3d(1,1,1);

    if (Application::isLightingEnabled())
    {
       glEnable(GL_LIGHTING);
	  glEnable(GL_NORMALIZE);
	  glColorMaterial ( GL_FRONT_AND_BACK, GL_DIFFUSE ) ;
       glEnable ( GL_COLOR_MATERIAL ) ;
	  glEnable(GL_LIGHT0);
    }
    else
    {
      glDisable(GL_LIGHTING);
    }

}

void SimulationUI::mouseWheel(short amount)
{
 VideoDevice * cam = Application::getMainCamera();
  if (cam==NULL)
	  return;

 double fov_degrees = cam->getFieldOfViewDegrees()*pow(2,0.05*amount);

   cam->setFieldOfViewDegrees(fov_degrees);

}

//Called when a key is pressed
void SimulationUI::handleKeypress(unsigned char key,bool special)
{    //The current mouse coordinates
 RoboticCar *mainCar=Application::getMainCar();

 int keyInt = key;
   if (special)
      keyInt=-keyInt;

	switch (keyInt)
	{
	     case 9:
			//cout << "Showing the display menu."<<endl;
		      break;

  	     case '2':
	  //   case 'e':
		case 'E': // for edit
			SwitchToTrackEditor();
			break;

		case '3':
			SwitchToSensorEditor();
			break;

		case 27: //Escape key

			Application::ExitApplication(); //Exit the program
			break;

               case 'a':
	       case 'A':

        case -100: // left arrow key for special key in GLUT
		case 37: // left arrow key
			mainCar->setDesiredSteeringAngle(mainCar->getDesiredSteeringAngle()+3);
			break;

               case 'w':
	       case 'W':

        case -101: // up arrow key for special key in GLUT
		case 38: // up arrow key
			mainCar->setDrivingForce(mainCar->getDrivingForce()+1.2);
			break;

        case -103: // down arrow key for special key in GLUT
        case 's':
		case 'S':
		case 'x':
		case 'X':
		case 40: // down arrow key
			mainCar->setDrivingForce(mainCar->getDrivingForce()-2);
			break;

        case -102: // right arrow in special key from GLUT
        case 'd':
		case 'D':
		case 39: // right arrow key
			mainCar->setDesiredSteeringAngle(mainCar->getDesiredSteeringAngle()-3);
			break;


        case -1: // for F1 special key in GLUT
		case 'p': // may indicate an F1 key was hit.
		case 'h': // h for help.
		case 'H':
              Utilities::launchRelativeDocument("simulationui.thtml");
		    break;

		case 13: // enter key
			{
				VideoDevice *cam = Application::getMainCamera();
				if (cam!=NULL)
				{
				   cam->goToDefaultOrientation();
				   if (cam->getFieldOfViewDegrees()!=
					   VideoDevice::getGlobalFieldOfViewDegrees())
				   {
				      cam->setFieldOfViewDegrees(VideoDevice::getGlobalFieldOfViewDegrees());
				   }
				}
				RoboticCar *car = Application::getMainCar();
				if (car!=NULL)
				{
				  car->stop();
				  if (car->getPosition().getMagnitude()>15000)
					  car->resetPositionAndSpeed();
				}
			}
			break;

		default:
			cout << "Unable to process key: '"<<key<<"', number: "<<((int)key)<<endl;
			break;
	}

}

void SimulationUI::mouseDown(int x,int y)
{
  dragging=true;
  lastPosition.set(x,y);
}

void SimulationUI::mouseMove(int x,int y)
{
 int dx=x-lastPosition.x,dy=y-lastPosition.y;

  if (dragging)
  {
	  VideoDevice *cam = Application::getMainCamera();
       if (cam!=NULL)
	  {
	   const Vector3D &dir = cam->getRotationVector();
	   double scaleFactor = cam->getFieldOfViewDegrees()*0.03;

	    cam->setProperty("rotation-y",dir.getY()-dx*0.5*scaleFactor);
	    cam->setProperty("rotation-x",dir.getX()+dy*0.5*scaleFactor);
	  }
  }

  lastPosition.set(x,y);

     #ifndef _USE_VISUAL_STUDIO_
       glutPostRedisplay();
  #endif
}

void SimulationUI::mouseUp(int x,int y)
{
  dragging=false;

     #ifndef _USE_VISUAL_STUDIO_
       glutPostRedisplay();
  #endif
}

void SimulationUI::draw()
{
  draw(Application::getMainCamera());
}

void SimulationUI::draw(VideoDevice *cam1)
{
 //   update();
 RoboticCar *mainCar=Application::getMainCar();
 Environment &e=Application::getMainEnvironment();

	//cout << "SimulationUI draw() called."<<endl;
 glutSetWindow(cam1->getWindowID());
 Vector3D camPos;

   if (cam1!=NULL)
   {
	   camPos=cam1->getPosition();
        mainCar->transformDirection(camPos);
   }

  drawSkybox();
  Vector3D v(mainCar->getX(),mainCar->getY(),mainCar->getZ());
  const Vector3D & cameraRotations = cam1->getRotationVector();

  {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// Set location in front of camera

    glLoadIdentity();

    glRotated(cameraRotations.getZ(),0,0,1);
    glRotated(cameraRotations.getX(),1,0,0);
    glRotated(cameraRotations.getY(),0,1,0);

    double angle=mainCar->getCurrentDirection()+180;

    glRotated(mainCar->getBodyTilt(),0,0,1);
    glRotated(-angle,0,1,0);
    Vector3D axis=mainCar->getRotationAxis();
    glRotated(mainCar->getAxisRotationDegrees(),axis.getX(),axis.getY(),axis.getZ());

    glTranslated(-mainCar->getX()-camPos.getX()
	    ,-mainCar->getY()-camPos.getY(), -mainCar->getZ()
	    -camPos.getZ());

       // if lighting is enabled, do this.
       glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    e.draw(cam1->getWindowID(),v);
  }

  #ifndef _USE_VISUAL_STUDIO_
  //  glutSwapBuffers();
  #endif
}


void SimulationUI::close()
{

}
