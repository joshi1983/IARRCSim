#ifndef VIDEO_DEVICE_HEADER
#define VIDEO_DEVICE_HEADER

#include "../io/Socket.hpp"
#include "../io/ServerSocket.hpp"

#ifdef _UBUNTU_

#include <GL/freeglut.h>
#include <GL/glu.h>

#else

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <string>

#include "../ui/UserInterface.hpp"
#include "../graphics/BitmapImage.hpp"
#include "../threads/Job.hpp"
#include "Sensor.hpp"
#include "../threads/Mutex.hpp"

class VideoDevice;

struct VideoBroadcastInfo
{
  VideoDevice *device;
  Socket *s;

  VideoBroadcastInfo(VideoDevice *device,Socket *s): device(device),s(s)
  {
  }
  ~VideoBroadcastInfo(){
    delete s;
  }
};

class FrameCapturingJob: public Job
{
 private:
	VideoDevice *device;

 protected:
		virtual void run();

 public:
	FrameCapturingJob(VideoDevice *device);
};

class VideoDevice: public Sensor
{
  friend class FrameCapturingJob;

private:
  static int global_width;
  static int global_height;
  static double global_fov_degrees;

  static Vector3D globalPosition;

  double fov_degrees;
  Vector3D rotationDegrees;

  int width,height;

  unsigned char *pixelBuffer;
  int pixelBufferLength;
  BitmapImage latestFrame;
  bool frameUpdateNeeded;

  // true if and only if the last frame was painted while not active.
  bool inactiveLastRepaint;

#ifdef _USE_VISUAL_STUDIO_

  HWND    hWnd;
  HDC     hDC;
  HGLRC   hRC;
#else

   int windowid;

#endif

  bool init(UserInterface *ui
#ifndef _UBUNTU_
#ifdef _USE_VISUAL_STUDIO_
	  ,HINSTANCE &hInstance
#endif
#endif
	  );

public:
     VideoDevice(const std::string &name);
     VideoDevice(const std::string &name,UserInterface *ui
		#ifndef _UBUNTU_
		#ifdef _USE_VISUAL_STUDIO_
		 ,HINSTANCE &hInstance
		 #endif
		#endif
		 );

	virtual ~VideoDevice();

    /**
    Updates the associated window's display
    */
    void repaint();

   /**
    Called when the associated window is resized or reshaped.
   */
    void resizeGraphics();

    const Vector3D & getRotationVector() const;
     int getResolutionWidth() const;
	int getResolutionHeight() const;
	int getWindowTop() const;
	int getWindowLeft() const;
	void SetupPixelFormat();
	void setName(const std::string &name);
	void goToDefaultOrientation();

#ifndef _USE_VISUAL_STUDIO_

     void setDimensions(int width,int height);
     int getWindowID() const;
#else
     HDC getHDC();
     HWND getWindowHandle();
     void GetPointInClient(const POINT *p,int *x,int *y);

#endif

	// brings the window for this video device to the front.
	void bringToFront();
     void showCurrentBuffer();
	void updateBufferedFrame();
	BitmapImage& getLatestFrame();

     virtual const std::string getDeviceInfoPropertyList(
		const std::string &format) const;
     virtual const std::string getType() const;
     virtual const std::string getProperty(const std::string &name) const;

    virtual bool setProperty(const std::string &propertyName,double value);

   /** overrides Sensor method */
   virtual void setActive(bool active);

   /** overrides Sensor method */
   virtual bool isActive() const;

   void setWindowPosition(int top,int left);

    void setFieldOfViewDegrees(double new_fov_degrees);
    double getFieldOfViewDegrees() const;

    static void initTextures();
	static void setGlobalFieldOfViewDegrees(double degrees);
	static void setGlobalHeight(int height);
	static void setGlobalWidth(int width);

     static bool setGlobalPositionX(double val);
     static bool setGlobalPositionY(double val);
     static bool setGlobalPositionZ(double val);

	static int getGlobalHeight();
	static int getGlobalWidth();
	static double getGlobalFieldOfViewDegrees();
};

#endif
