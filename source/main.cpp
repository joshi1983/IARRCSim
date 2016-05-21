/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */


#include "stdafx.h"

#ifndef _UBUNTU_
#   include <windows.h>
#endif

#include "io/HTTPRequest.hpp"
#include "io/HTTPResponse.hpp"

#include <iostream>

#include "app/Application.hpp"
#include "ui/Utilities.hpp"
#include "threads/Thread.hpp"

#ifdef _UBUNTU_

#include <GL/freeglut.h>
#include <GL/glut.h>

#else
#   include "io/console.hpp"

#   ifdef __APPLE__
#         include <GLUT/glut.h>
#   else
#         include <GL/glut.h>
#   endif

#endif

#include <stdlib.h>


#include "docserver/DocServer.hpp"
#include "app/Application.hpp"
#include "threads/Mutex.hpp"
#include "threads/Thread.hpp"

using namespace std;


void testLidar()
{
  /*->getDepthsOnPlane(pos,axis1,axis2,minDepth,
		 maxDepth,degreesToScan,directions,numRays,results);
*/
}

long long timeTicker;

void timerFunc(int val)
{
   Application::doJobs();
   repaintAllWindows();

   if (isInSimulationMode() && (!Application::isPaused()))
   {
      long long difference = Thread::getCurrentTickCount()-timeTicker;

        if (difference<0 || difference>2000)
	       difference=0;

	Application::getMainEnvironment().ticksPerformed(
	  difference
	);

   }
   timeTicker=Thread::getCurrentTickCount();

   glutTimerFunc(10,timerFunc,10);
}

int main(int argc,char *argv[])
{

    RedirectIOToConsole();
 Application::setCommandLineArguments(argc,argv);

 HTTPResponse::loadHTTPResponseCodes();

  InitGraphics();

 DocServer docServe;

  Utilities::launchRelativeDocument("simulationwindow.thtml");

  timeTicker=Thread::getCurrentTickCount();
  glutTimerFunc(10,timerFunc,10);

 glutMainLoop();
 Application::ExitApplication();

 return 0;
}

