#include "Application.hpp"

#include "../sensors/VideoDevice.hpp"
#include "../threads/JobCollection.hpp"
#include "../threads/Thread.hpp"

#include "../ui/TrackEditorUI.hpp"
#include "../ui/SensorEditorUI.hpp"

#include "../io/FileDirectory.hpp"
#include "../graphics/Texture.hpp"
#include "../graphics/Graphics2D.hpp"
#include "../threads/Thread.hpp"
#include "../io/HTTPParser.hpp"

#include "../splash/Introduction.hpp"
#include "../physicalobjects/Backdrop.hpp"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

const char* TRACK_FILENAME="track.txt";
const char *Application::CONFIG_INI_FILE_NAME="config/config.ini";
const char *Application::SENSORS_INI_FILE_NAME="config/sensors.ini";

bool Application::lightingEnabled=false;
Vector3D Application::lightDirection(-0.7,0.3,0.3);
bool Application::paused=false;

static Environment e;
static RoboticCar *mainCar=new RoboticCar(string("mainCar"),0,25,0,180,false);

static SimulationUI *sim=new SimulationUI();
static TrackEditorUI *trackEditor=new TrackEditorUI();
static SensorEditorUI *sensorEditor=new SensorEditorUI();
static UserInterface *ui=sim;
static vector<Backdrop> backdrops;

static UserInterface *uis[]=
  {NULL,sim,trackEditor,sensorEditor};

static VideoDevice *cam1=NULL;
static JobCollection jobs;
static Sensors sensors;

static void initBackdrops();
static int argc;
static char **argv=NULL;

void Application::setCommandLineArguments(int argc_,char **argv_)
{
   argc=argc_;
   argv=argv_;
}

void myGlutINIT()
{
 static bool initialized=false;
  	   #ifndef _USE_VISUAL_STUDIO_

       if (!initialized)
       {
        initialized=true;
  	    glutInit(&argc,argv);
  	    //cout << "glutInit called."<<endl;

       }

  	   #endif

}

static void initBackdrops()
{
	// avoid adding the same records more than once.
  if (backdrops.size()!=0)
	  return;

 Backdrop backdrop(string("1"),
	  string("models\\backdrops\\St_Denis_Field_backdrop.rrsm")
	  ,string("images/st_denis.png"));

  backdrops.push_back(backdrop);
  backdrops.push_back(Backdrop(string("2"),
	  string("models\\backdrops\\Thunderbird_backdrop.rrsm")
	  ,string("images/thunderbird.png")));

  backdrops.push_back(Backdrop(string("3"),
	  string("models\\backdrops\\Empty.rrsm")
	  ,string("images/nowhere.png")));
}

vector<Backdrop>& Application::getBackdropVector()
{
  return backdrops;
}

Sensors& Application::getAllSensors()
{
  return sensors;
}

const Vector3D& Application::getLightDirection()
{
  return lightDirection;
}

bool Application::isLightingEnabled()
{
  return lightingEnabled;
}

bool Application::setLightingEnabled(double val)
{
  if (abs((double)val)>0.5)
     lightingEnabled=true;
  else
      lightingEnabled=false;

  return true;
}

bool Application::setLightDirectionX(double val)
{
  lightDirection.setX(val);
  return true;
}

bool Application::setLightDirectionY(double val)
{
  lightDirection.setY(val);
  return true;
}

bool Application::setLightDirectionZ(double val)
{
  lightDirection.setZ(val);
  return true;
}

double Application::getLightDirectionX()
{
  return lightDirection.getX();
}

double Application::getLightDirectionY()
{
  return lightDirection.getY();
}

double Application::getLightDirectionZ()
{
  return lightDirection.getZ();
}

VideoDevice * Application::getMainCamera()
{
  return cam1;
}

void Application::clearTrack()
{
 RoboticCar * mainC = new RoboticCar(*mainCar);

  e.clear();
  // need to add the main car?
  e.add(mainC);
  mainCar=mainC;
}

double Application::getDepthAlongRay(const Ray3D & ray,double minDepth,double maxDepth)
{
 Ray3D r(ray);

  if (mainCar!=NULL)
     mainCar->transform(r);

 double depth= e.getNearestPositiveIntersection(r,minDepth,maxDepth);

 return depth;
}

void Application::getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,int numRays,double *results)
{
 Vector3D pos1(pos);
 Vector3D ax1(axis1);
 Vector3D ax2(axis2);

   if (mainCar!=NULL)
   {
	mainCar->transform(pos1);
     mainCar->transformDirection(ax1);
	mainCar->transformDirection(ax2);
   }

 return e.getDepthsOnPlane(pos1,ax1,ax2,minDepth,maxDepth,degreesToScan
	,numRays,results);
}

bool Application::loadTrackFrom(const std::string &filename)
{
  //cout <<"Application::loadTrackFrom called."<<endl;

 bool result= e.loadTrackFrom(filename);

 mainCar=e.getFirstCarNotControlledExternally();
 if (mainCar==NULL) // no car found in the environment
 {
    mainCar=new RoboticCar("maincar1",0,25,0,180,false);
    e.add(mainCar);
 }
 else
 {
   mainCar->setControlledExternally(false);
 }

   // cout <<"Application::loadTrackFrom complete."<<endl;

   return result;
}

void Application::SubmitJobForMainThread(Job *job)
{
 if (job==NULL)
 {
   cout << "Unable to submit NULL job to main thread."<<endl;
   return;
 }
  SubmitJobForMainThread(job,false);
}

void Application::SubmitJobForMainThread(Job *job,bool block)
{
  if (job==NULL)
  {
    cout << "Unable to submit NULL job for main thread."<<endl;
    return;
  }
  if (block)
      job->delayDestroy();

  jobs.add(job);

  if (block)
  {
    while (!job->isDone())
    {
      Thread::sleepMillis(5);
    }
    delete job;
  }
}

bool SwitchToUI(int mode)
{
  if (mode<1 || mode>3)
  {
	  cout << "Invalid user interface mode: "<<mode<<endl;
        return false;
  }

  if (ui!=NULL)
      ui->close();

  if (uis[mode]!=NULL)
     uis[mode]->update();
  else
  {
     cout << "weird.  UI "<<mode<<" is NULL."<<endl;
  }

  ui=uis[mode];


  if (ui!=NULL)
	  ui->draw();

  DrawGraphics();
  return true;
}

void SwitchToTrackEditor()
{
  SwitchToUI(2);
}

void SwitchToSensorEditor()
{
  SwitchToUI(3);
}

void SwitchToSimulationView()
{
  SwitchToUI(1);
}

void Application::ExitApplication()
{
   // save to track.txt.
   e.saveTo(TRACK_FILENAME);
   sensors.saveToINI(SENSORS_INI_FILE_NAME);

    delete sim;
    delete trackEditor;

    exit(0);
}

void Application::setDriveTorqueQuantity(double val)
{
  if (mainCar!=NULL)
  {
     mainCar->setDrivingForce(val);
  }
}

void Application::setSteeringTargetAngle(double angle)
{
  if (mainCar!=NULL)
  {
    mainCar->setDesiredSteeringAngle(angle);
  }
}

// Set up pixel format for graphics initialization
void SetupPixelFormat()
{
  if (cam1==NULL)
     cout << "Unable to SetupPixelFormat because cam1 is NULL."<<endl;
  else
     cam1->SetupPixelFormat();
}


int getWindowWidth()
{
	if (cam1==NULL)
		return VideoDevice::getGlobalWidth();

    return cam1->getResolutionWidth();
}

int getWindowHeight()
{
   if (cam1==NULL)
	return VideoDevice::getGlobalHeight();

    return cam1->getResolutionHeight();
}

int getWindowTop()
{
  if (cam1==NULL)
     return 10;

    return cam1->getWindowTop();
}

int getWindowLeft()
{
  if (cam1==NULL)
     return 10;

    return cam1->getWindowLeft();
}

#ifdef _USE_VISUAL_STUDIO_

void GetPointInClient(const POINT *p,int *x,int *y)
{
 cam1->GetPointInClient(p,x,y);
}

// Handle window events and messages
int WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM  lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        ResizeGraphics();
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
	    Application::ExitApplication();
        PostQuitMessage(0);
        break;

    case WM_PAINT:
	    DrawGraphics();
	    break;

    // Default event handler
    default:
        return DefWindowProc (hWnd, uMsg, wParam, lParam);
        break;
    }

    return 1;
}

#endif

#ifndef _USE_VISUAL_STUDIO_

void Application::setMainVideoDeviceDimensions(int width,int height)
{
  if (cam1!=NULL)
  {
     cam1->setDimensions(width,height);
  }
}

#ifdef _UBUNTU_
void InitGraphics()
{
  InitGraphics(ui);
}

#endif

#endif



// Initialize OpenGL graphics
void InitGraphics(UserInterface *ui
#ifndef _UBUNTU_
 #ifdef _USE_VISUAL_STUDIO_
			   ,HINSTANCE &hInstance
 #endif

#endif
   )
{
   myGlutINIT();

   #ifndef _UBUNTU_
    ShowIntro();
   #endif

	e.loadIniFile(Application::CONFIG_INI_FILE_NAME);

   cam1=new VideoDevice(string("video1"),ui
	   #ifndef _UBUNTU_
	   #ifdef _USE_VISUAL_STUDIO_
	   ,hInstance
	   #endif
	   #endif
	   );

   Application::getAllSensors().add(cam1);

    VideoDevice *droneCam1=new VideoDevice(string("drone_video1"));

   Application::getAllSensors().add(droneCam1);

   VideoDevice::initTextures();

    droneCam1->setMountingObject(Application::getMainCar()->getCurrentDrone());
    droneCam1->setWindowPosition(600,0);

   	ui->update();

	ui->draw();



	if (FileDirectory::fileExists(Application::SENSORS_INI_FILE_NAME))
       e.loadIniFile(Application::SENSORS_INI_FILE_NAME);

   initBackdrops();

   Backdrop &curBackdrop=backdrops[0];

   Environment::loadTextures();
   e.loadBackdropFrom(curBackdrop.getID());
   curBackdrop.setActive(true);

   if (FileDirectory::fileExists(TRACK_FILENAME))
   {
	   e.loadTrackFrom(TRACK_FILENAME);
	   RoboticCar * car=e.getFirstCarNotControlledExternally();

	   if (car!=NULL)
	   {
	     if (mainCar!=NULL)
	     {
	        mainCar->copyAttributesFrom(car);
	   	   e.remove(car);
	     }
	     else
		    mainCar = car;

		mainCar->setControlledExternally(false);
	   }
   }
   else
   {
        e.makeCircuitTrack();
   }
   if (mainCar!=NULL) // if there is a main car, add it to the environment.
      e.add(mainCar);

 //  ui=sim;
   ui->update(); // update the user interface display.

   if (cam1!=NULL)
      cam1->bringToFront();

   HideIntro(); // hide the introduction window
}

#ifndef _UBUNTU_
void InitGraphics(
                  #ifdef _USE_VISUAL_STUDIO_
                  HINSTANCE &hInstance
                  #endif
                  )
{
  InitGraphics(ui
             #ifdef _USE_VISUAL_STUDIO_
               ,hInstance
              #endif
               );

}

#endif

// Resize graphics to fit window
void ResizeMainWindowGraphics()
{
	if (ui!=NULL)
	{
  	   ui->update();
       //myGlutINIT();
	}
}

void repaintAllWindows()
{
  sensors.repaintAllWindows();
}

// Draw frame
void DrawGraphics()
{
 Application::getMainCamera()->repaint();
}

UserInterface *getCurrentUI()
{
  return ui;
}

RoboticCar * Application::getMainCar()
{
  return mainCar;
}

Environment & Application::getMainEnvironment()
{
  return e;
}

std::string Application::getAllBackdropsFormatted(const std::string &format)
{
 ostringstream result;

  if (format=="xml")
     result<<"<backdrops>"<<endl;
  else if (format=="JSON")
	  result<<"["<<endl;

  for (unsigned int i=0;i<backdrops.size();i++)
  {
      result << backdrops[i].getFormatted(format);
  }

  if (format=="xml")
     result<<"</backdrops>"<<endl;
  else if (format=="JSON")
	  result<<"]"<<endl;

  return result.str();
}

void Application::doJobs()
{
  jobs.doJobs();
}

bool isInSimulationMode()
{
  return ui==sim;
}

void fixFilenameSlashes(char * filename)
{
    for (int i=0;filename[i]!='\0';i++)
    {
       if (filename[i]=='\\' || filename[i]=='/')
          filename[i]=FILE_SEPERATOR;
    }
}

bool Application::isPaused()
{
  return paused;
}

void Application::setPaused(bool newValue)
{
  Application::paused=newValue;
}

UserInterface * Application::getUserInterface()
{
   return ui;
}

SimulationUI * Application::getSimulationUI()
{
  return sim;
}

