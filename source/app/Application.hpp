#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "../stdafx.h"
#include "../io/Socket.hpp"
#include "../threads/Job.hpp"
#include "../sensors/Sensors.hpp"
#include "../vectors/Ray3D.hpp"
#include "../ui/UserInterface.hpp"
#include "../physicalobjects/RoboticCar.hpp"
#include "../physicalobjects/Backdrop.hpp"
#include "../ui/SimulationUI.hpp"
#include "Environment.hpp"

#include <string>
#include <vector>

#ifndef _UBUNTU_

#include <windows.h>

#endif

class Application
{
private:
	static bool lightingEnabled;
	static void freeSensors();
    static Vector3D lightDirection;
    static bool paused;


public:

#ifndef _USE_VISUAL_STUDIO_

  static void setMainVideoDeviceDimensions(int width,int height);

#endif

  static std::string getAllBackdropsFormatted(const std::string &format);
  static Sensors& getAllSensors();

  static bool isPaused();
  static void setPaused(bool newValue);
  static const Vector3D & getLightDirection();
  static bool isLightingEnabled();
  static RoboticCar * getMainCar();
  static VideoDevice * getMainCamera();
  static Environment & getMainEnvironment();
  static SimulationUI * getSimulationUI();
  static void ExitApplication();
  static void SubmitJobForMainThread(Job *job); // nonblocking
  static void SubmitJobForMainThread(Job *job,bool block);

  static void clearTrack();

  static void setDriveTorqueQuantity(double val);
  static void setSteeringTargetAngle(double angle);

  static std::vector<Backdrop>& getBackdropVector();

  /**
 Returns true if successful
*/
  static bool loadTrackFrom(const std::string &filename);

  static double getDepthAlongRay(const Ray3D & ray,double minDepth,double maxDepth);

  static void getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,int numRays,double *results);

  static void doJobs();

  static bool setLightingEnabled(double val);

  static bool setLightDirectionX(double val);
  static bool setLightDirectionY(double val);
  static bool setLightDirectionZ(double val);

  static double getLightDirectionX();
  static double getLightDirectionY();
  static double getLightDirectionZ();

  static UserInterface * getUserInterface();

  static void setCommandLineArguments(int argc,char **argv);
  static const char *CONFIG_INI_FILE_NAME;
  static const char *SENSORS_INI_FILE_NAME;
};

void fixFilenameSlashes(char * filename);

bool SwitchToUI(int mode);
void SwitchToTrackEditor();
void SwitchToSensorEditor();
void SwitchToSimulationView();
int getWindowWidth();
int getWindowHeight();


#ifdef _USE_VISUAL_STUDIO_

HDC getDeviceContext();

#endif

void DrawGraphics();
void repaintAllWindows();
void ResizeMainWindowGraphics();



#ifndef _UBUNTU_

void GetPointInClient(const POINT *p,int *x,int *y);

#endif

UserInterface *getCurrentUI();

bool isInSimulationMode();

#ifndef _UBUNTU_

int WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM  lParam);

#endif


void myGlutINIT();

void InitGraphics();


void InitGraphics(UserInterface *ui
#ifndef _UBUNTU_

  #ifdef _USE_VISUAL_STUDIO_
			   ,HINSTANCE &hInstance
  #endif

#endif
);

#endif
