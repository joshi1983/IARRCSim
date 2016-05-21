#include "VideoDevice.hpp"
#include "../io/ServerSocket.hpp"
#include "../threads/Thread.hpp"
#include "../io/HTTPRequest.hpp"
#include "../io/HTTPResponse.hpp"
#include "../graphics/BitmapImage.hpp"
#include "../app/Application.hpp"
#include "../graphics/Texture.hpp"
#include "../graphics/Graphics2D.hpp"

#include "../stdafx.h"

#include <cstdlib>

#ifdef _USE_VISUAL_STUDIO_

#include <gl/gl.h>
#include <gl/glu.h>

#endif


#if !defined(_UBUNTU_) && !defined(_USE_VISUAL_STUDIO_)


#endif

#include <string>
#include <sstream>
#include <cstdio>

int VideoDevice::global_width=1000;
int VideoDevice::global_height=800;
double VideoDevice::global_fov_degrees=45.0;
Vector3D VideoDevice::globalPosition(0,25,0);

using namespace std;

static Texture *novideoTexture=NULL;

#ifdef _UBUNTU_

static const char *appname = "IARRC Simulator";

#else

#ifdef _USE_VISUAL_STUDIO_
static const LPCWSTR appname = TEXT("IARRC Simulator");
#else
static const char* appname = "IARRC Simulator";
#endif


static WNDCLASS wndclass;

#endif

static string BITMAP_MIMES[4]=
{
  "image/bmp","image/bitmap","image/x-bmp",
  "image/x-windows-bmp"
};


FrameCapturingJob::FrameCapturingJob(VideoDevice *device):device(device)
{
}

void FrameCapturingJob::run()
{
  //  cout <<"FrameCapturingJob::run() called." <<endl;
 if (!device->frameUpdateNeeded)
    return;

 int numBytes=3;
 int width=device->getResolutionWidth(),height=device->getResolutionHeight();

 //cout << "FrameCapturingJob width="<<width<<", "<<height<<endl;

 int widthStep=width*numBytes;
 if (widthStep%4!=0)
 {
   widthStep+=(4-widthStep%4);
 }
 int sizeNeeded=width*height*numBytes;

 //cout << "FrameCapturingJob sizeNeeded="<<sizeNeeded<<endl;

  if (sizeNeeded>device->pixelBufferLength)
  {
     if (device->pixelBuffer!=NULL)
		free(device->pixelBuffer);

	device->pixelBuffer = (unsigned char*)malloc(1*sizeNeeded);
	if (device->pixelBuffer!=NULL)
        device->pixelBufferLength=sizeNeeded;
	else
         device->pixelBufferLength=0;
  }
  unsigned char *pixelBuffer=device->pixelBuffer;

  if (pixelBuffer==NULL)
  {
    cout << "pixelBuffer is unexpectedly NULL."<<endl;
  }
  else
  {
     // cout << "Going to set current window."<<endl;
      glutSetWindow(device->getWindowID());

    int w2=glutGet(GLUT_WINDOW_WIDTH);
    int h2=glutGet(GLUT_WINDOW_HEIGHT);

     if (w2!=width || h2!=height)
    {
        cout << "Dimensions do not match."<<endl;
        if (w2!=width)
           cout << "w2="<<w2<<", width="<<width<<endl;
        if (h2!=height)
            cout << "h2="<<h2<<", height="<<height<<endl;

        device->frameUpdateNeeded=false;
        return;
    }
    // cout << "Going to read pixels using glReadPixels."<<endl;

     // copy all pixel values from OpenGL colour buffer.
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,pixelBuffer);

    // cout << "Going to set latestFrame size."<<endl;
    device->latestFrame.setSize(width,height);
    if (device->latestFrame.getWidth()==0)
    {
        cout << "latestFrame size could not be updated."<<endl;
        device->frameUpdateNeeded=false;
        return;
    }
    // cout << "Going to copy pixels to the latestFrame."<<endl;
    for (int y=0;y<height;y++)
    {
       for (int x=0;x<width;x++)
	  {
	    int index=y*widthStep+x*numBytes;
	    int encodedColour=((pixelBuffer[index]&0xff)<<16)
	      |((pixelBuffer[index+1]&0xff)<<8)|(pixelBuffer[index+2]&0xff);
	      device->latestFrame.setPixel(x,height-y-1,encodedColour);
	  }
    }
  }

  //   cout << "All done."<<endl;
  device->frameUpdateNeeded=false;
}

#ifndef _USE_VISUAL_STUDIO_

void resize(int width, int height)
{
    VideoDevice*cam = Application::getAllSensors().getVideoDeviceByWindowID(glutGetWindow());


    if (cam==Application::getMainCamera())
    {
         Application::setMainVideoDeviceDimensions(width,height);
             myGlutINIT();
    }
    if (cam!=NULL)
    {
        cam->resizeGraphics();
    }
}

void display()
{
  int windowid = glutGetWindow();
  VideoDevice * cam = Application::getAllSensors().getVideoDeviceByWindowID(windowid);
  if (cam!=NULL)
     cam->repaint();
}

void mouseWheel(int button,int dir,int x,int y)
{
  UserInterface *ui = getCurrentUI();

  if (ui!=NULL)
  {
    ui->mouseWheel((short)dir);
  }
}

void mouseFunc(int button,int state,int x,int y)
{
  VideoDevice * cam = Application::getAllSensors().getVideoDeviceByWindowID(glutGetWindow());
  UserInterface *ui = getCurrentUI();

  if (ui!=NULL && cam==Application::getMainCamera())
  {
    if (state==GLUT_UP)
       ui->mouseUp(x,y);
    else if (state==GLUT_DOWN)
       ui->mouseDown(x,y);
    else
       ui->mouseMove(x,y);
  }
  else
  { // drone window.

    if (state!=GLUT_DOWN)
    {
       return;
    }
    RoboticCar * car = Application::getMainCar();
    RoboticHelicopter * drone = car->getReleasedDrone();
    if (drone==NULL)
    {
       car->releaseDrone();
       drone = car->getReleasedDrone();
       if (drone!=NULL)
       {
          drone->follow(car,2000);
       }
    }
    else
    {
       car->landDroneOnCar();
    }
  }
}

void mouseMotionFunc(int x,int y)
{
  UserInterface *ui = getCurrentUI();

  if (ui!=NULL)
  {
     ui->mouseMove(x,y);
  }

}

void keypress(unsigned char key,int x,int y)
{
   UserInterface *ui=getCurrentUI();
   if (ui!=NULL)
   {
     ui->handleKeypress(key,false);
   }
}

void special_callback(int key, int x, int y)
{
   UserInterface *ui=getCurrentUI();
   if (ui!=NULL)
   {
     ui->handleKeypress(key,true);
   }
}

void idle()
{
   glutPostRedisplay();
}

#else

 HDC VideoDevice::getHDC()
 {
   return hDC;
 }

 HWND VideoDevice::getWindowHandle()
 {
   return hWnd;
 }

#endif


void VideoDevice::bringToFront()
{
#ifdef _USE_VISUAL_STUDIO_

	SetForegroundWindow(getWindowHandle());

#endif
}

void VideoDevice::setGlobalHeight(int height)
{
  global_height=height;
}

void VideoDevice::setGlobalWidth(int width)
{
  global_width=width;
}

int VideoDevice::getGlobalHeight()
{
  return global_height;
}

int VideoDevice::getGlobalWidth()
{
  return global_width;
}

bool VideoDevice::setGlobalPositionX(double x)
{
   globalPosition.setX(x);
   return true;
}

bool VideoDevice::setGlobalPositionY(double y)
{
   if (y<0.001)
   {
	cout << "position-y must be positive instead of "<<y<<"."<<endl;
	return false;
   }

   globalPosition.setY(y);
   return true;
}

bool VideoDevice::setGlobalPositionZ(double z)
{
   globalPosition.setZ(z);
   return true;
}

void VideoDevice::setGlobalFieldOfViewDegrees(double degrees)
{
  global_fov_degrees=degrees;
}


double VideoDevice::getGlobalFieldOfViewDegrees()
{
  return global_fov_degrees;
}

void VideoDevice::updateBufferedFrame()
{
  cout << "VideoDevice::updateBufferedFrame() called."<<endl;
  frameUpdateNeeded=true;
  cout << "going to submit FrameCapturingJob."<<endl;
  Application::SubmitJobForMainThread(new FrameCapturingJob(this),true);
}

BitmapImage& VideoDevice::getLatestFrame()
{
  return latestFrame;
}

void VideoDevice::showCurrentBuffer()
{
 #ifndef _USE_VISUAL_STUDIO_

  glutSwapBuffers();

 #else
    SwapBuffers(hDC);

 #endif

}

void VideoDevice::goToDefaultOrientation()
{
  rotationDegrees.set(0,0,0);
}


VideoDevice::VideoDevice(const string &name):
  pixelBuffer(NULL),pixelBufferLength(0),Sensor(name),
  rotationDegrees(0,0,0),fov_degrees(global_fov_degrees),inactiveLastRepaint(false)
{
 char temp[200];
    sprintf(temp,"IARRC Simulator - %s",name.c_str());

    glutCreateWindow(temp);
    windowid=glutGetWindow();

     glutInitWindowPosition(600,100);

     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

     glutReshapeFunc(resize);
     // call back for resizing the window.

     glutDisplayFunc(display);
     // call back for updating the display.

     glutMouseFunc(mouseFunc);
}

VideoDevice::VideoDevice(const string &name,UserInterface *ui
#ifndef _UBUNTU_
#ifdef _USE_VISUAL_STUDIO_
					,HINSTANCE &hInstance
#endif
#endif
	):
 pixelBuffer(NULL),pixelBufferLength(0),Sensor(name),
 rotationDegrees(0,0,0),fov_degrees(global_fov_degrees),
 inactiveLastRepaint(false)
{
	position=globalPosition;
   init(ui
#ifndef _UBUNTU_
#ifdef _USE_VISUAL_STUDIO_
	   ,hInstance
#endif
#endif
	   );

}

 const Vector3D & VideoDevice::getRotationVector() const
 {
   return rotationDegrees;
 }



bool VideoDevice::init(UserInterface *ui
#ifndef _UBUNTU_
#ifdef _USE_VISUAL_STUDIO_
				   ,HINSTANCE &hInstance
#endif
#endif
				   )
{
#ifdef _USE_VISUAL_STUDIO_

    // Define the window class
    wndclass.style         = 0;
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, appname);
    wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wndclass.lpszMenuName  = appname;
    wndclass.lpszClassName = appname;

    // Register the window class
    if (!RegisterClass(&wndclass))
    {
	    cout << "Unable to register window class"<< endl;
	    return false;
    }
        // Create the window
    this->hWnd = CreateWindow(
            appname,
            appname,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
		  VideoDevice::getGlobalWidth(),
            VideoDevice::getGlobalHeight(),
            NULL,
            NULL,
            hInstance,
            NULL);

    if (!hWnd)
    {
	  cout << "Unable to create window."<<endl;
       return false;
    }
    this->hDC = GetDC(hWnd);


    SetupPixelFormat();

    this->hRC = wglCreateContext(this->hDC);
    wglMakeCurrent(this->hDC, this->hRC);

    if (ui!=NULL)
       ui->update();

    // Display the window
    ShowWindow(this->hWnd,SW_SHOWNORMAL);

    return true;

#else // Ubuntu or mingw is used

     glutInitWindowSize(VideoDevice::getGlobalWidth(),
            VideoDevice::getGlobalHeight());


     glutInitWindowPosition(10,10);

     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

myGlutINIT();

glutCreateWindow(appname);
windowid = glutGetWindow();

glutMouseWheelFunc(mouseWheel);
glutMouseFunc(mouseFunc);
glutMotionFunc(mouseMotionFunc);

glutKeyboardFunc(keypress);
glutSpecialFunc(special_callback);

//glutIdleFunc(idle);

glutReshapeFunc(resize);
// call back for resizing the window.

glutDisplayFunc(display);
// call back for updating the display.

return true; // success

#endif
}

void VideoDevice::SetupPixelFormat()
{
 #ifdef _USE_VISUAL_STUDIO_

    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 16;
    ppfd->cDepthBits = 16;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    pixelformat = ChoosePixelFormat(this->hDC, ppfd);
    SetPixelFormat(this->hDC, pixelformat, ppfd);

 #endif

}

const std::string VideoDevice::getDeviceInfoPropertyList(const std::string &format) const
{
	 string result;

	  result+=formatPropertyAssignment("width",getResolutionWidth(),format);
	  result+=formatPropertyAssignment("height",getResolutionHeight(),format);

	  result+=formatPropertyAssignment("rotation-x",rotationDegrees.getX(),format);
	  result+=formatPropertyAssignment("rotation-y",rotationDegrees.getY(),format);
	  result+=formatPropertyAssignment("rotation-z",rotationDegrees.getZ(),format);
      result+=formatPropertyAssignment("fov-degrees",fov_degrees,format);

	  return result;
}

bool VideoDevice::setProperty(const std::string &propertyName,double value)
{
  if (propertyName=="width")
	  this->width=(int)value;
  else if (propertyName=="height")
	  this->height=(int)value;
  else if (propertyName=="rotation-x")
	  this->rotationDegrees.setX(value);
  else if (propertyName=="rotation-x")
	  this->rotationDegrees.setX(value);
  else if (propertyName=="rotation-y")
	  this->rotationDegrees.setY(value);
  else if (propertyName=="rotation-z")
	  this->rotationDegrees.setZ(value);
  else if (propertyName=="fov-degrees")
	  this->setFieldOfViewDegrees(value);
  else
	  return Sensor::setProperty(propertyName,value);

  return true;
}

const std::string VideoDevice::getProperty(const std::string &name) const
{
  if (name=="width")
	  return asString(getResolutionWidth());
  else  if (name=="height")
       return asString(getResolutionHeight());
  else if (name=="rotation-x")
	  return asString(rotationDegrees.getX());
  else if (name=="rotation-y")
	  return asString(rotationDegrees.getY());
  else if (name=="rotation-z")
	  return asString(rotationDegrees.getZ());
  else if (name=="fov-degrees")
	  return asString(fov_degrees);
  else
	  return Sensor::getProperty(name);
}

const std::string VideoDevice::getType() const
{
  return "video";
}

int VideoDevice::getResolutionWidth() const
{
#ifdef _USE_VISUAL_STUDIO_

  RECT rect;
    GetClientRect(this->hWnd, &rect);
    return rect.right;

#else

  return width;

#endif

}

int VideoDevice::getResolutionHeight() const
{
#ifdef _USE_VISUAL_STUDIO_

    RECT rect;
    GetClientRect(this->hWnd, &rect);
    return rect.bottom;

#else

  return height;

#endif

}

int VideoDevice::getWindowTop() const
{
#ifdef _USE_VISUAL_STUDIO_

    WINDOWINFO info;
    GetWindowInfo(hWnd, &info);
    return info.rcClient.top+30;

#else
 return 0;

#endif
}

int VideoDevice::getWindowLeft() const
{
#ifdef _USE_VISUAL_STUDIO_

    WINDOWINFO info;
    GetWindowInfo(hWnd, &info);
    return info.rcClient.left;
#else

 return 0;

#endif

}

#ifdef _USE_VISUAL_STUDIO_

void VideoDevice::GetPointInClient(const POINT *p,int *x,int *y)
{
 POINT p1=(*p);

  ScreenToClient(hWnd,&p1);

  (*x)=p1.x;
  (*y)=p1.y;
}

#else

void VideoDevice::setDimensions(int width,int height)
{
   this->width=width;
   this->height=height;
   resizeGraphics();
}

#endif

void VideoDevice::setName(const string &name)
{
	Sensor::name = name;
}

VideoDevice::~VideoDevice()
{
  #ifdef _USE_VISUAL_STUDIO_

    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);

 #endif

    if (pixelBuffer!=NULL)
       free(pixelBuffer);
}

void VideoDevice::setFieldOfViewDegrees(double val)
{
  if (val<1)
	val=1;
  else if (val>160)
	val=160;

  if (fov_degrees==val)
	  return;

  fov_degrees=val;
  resizeGraphics();
}

double VideoDevice::getFieldOfViewDegrees() const
{
  return fov_degrees;
}

#ifndef _USE_VISUAL_STUDIO_

int VideoDevice::getWindowID() const
{
  return windowid;
}

#endif

void VideoDevice::setActive(bool active)
{
  if (active==Sensor::active)
     return;

  Sensor::active=active;
  resizeGraphics();
}

bool VideoDevice::isActive() const
{
  bool result=false;

    if (!Sensor::active)
       return false;

    if (mountingObject==NULL)
       result=true;
    else
    {
       RoboticHelicopter *helicopter = dynamic_cast<RoboticHelicopter*>(mountingObject);
       if (helicopter!=NULL)
       {
        //   cout << "VideoDevice::isActive() mountingObject is a helicopter."<<endl;
           result= helicopter->isAnimated();
       }
    }
   // cout << "VideoDevice::isActive() returning "<<result<<endl;

    return result;
}

void VideoDevice::repaint()
{
    VideoDevice *cam1 = Application::getMainCamera();



  if (this==cam1) // main window.
  {
    UserInterface *ui = Application::getUserInterface();

	 if (ui!=NULL)
	 {
        ui->draw();
	 }
  }
  else // not main window
  {


      if (isActive())
      { // show 3D graphics.
          // update the graphics for another
         // cout << "Drawing helicopter view."<<endl;
         SimulationUI * sim = Application::getSimulationUI();
         glutSetWindow(this->windowid);

         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         Environment &e=Application::getMainEnvironment();
         Vector3D viewpoint(position);
          viewpoint.add(mountingObject->getPosition());

         // cout << "y="<<viewpoint.getY()<<endl;
          sim->update(this);

          glLoadIdentity();
          glRotated(90,1,0,0);
          glTranslated(-viewpoint.getX(),-viewpoint.getY(),-viewpoint.getZ());
          //glDisable(GL_TEXTURE_2D);

          e.draw(this->windowid,viewpoint);
      }
      else
      {
        if (inactiveLastRepaint)
           return;

         glutSetWindow(this->windowid);
        glPushMatrix();
        glLoadIdentity();
         // show black screen with the no video text.
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // novideoTexture
         if (novideoTexture!=NULL)
         {
            novideoTexture->draw2D(this->windowid,0,0);
         }

         glPopMatrix();
      }
      if (!isActive())
         inactiveLastRepaint=true;
      else
         inactiveLastRepaint=false;
  }

  // Show the new scene
  showCurrentBuffer();

}

void VideoDevice::resizeGraphics()
{
  glutSetWindow(this->windowid);

  width = glutGet(GLUT_WINDOW_WIDTH);
  height = glutGet(GLUT_WINDOW_HEIGHT);

  if (Application::getMainCamera()==this)
     ResizeMainWindowGraphics();
  else
  {
        glViewport(0,0,width,height);

        if (!isActive())
           Graphics2D::setupFor2DGraphics(width,height);
  }

  this->width=width;
  this->height=height;
}

void VideoDevice::initTextures()
{
   if (novideoTexture==NULL)
      novideoTexture=new Texture("images/no_video.bmp");
}

void VideoDevice::setWindowPosition(int top,int left)
{
   glutSetWindow(windowid);
   glutPositionWindow(top,left);
}

