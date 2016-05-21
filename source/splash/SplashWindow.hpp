#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include "../stdafx.h"

#ifndef _UBUNTU_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif

//class SplashWindow
//Desc: Easily create a Splash Screen for your application.
//			The class has functionality to make windows that
//			are not rectangular using a bitmap.
//			An overridable function is available for the doing
//			tasks before ayour application loads.

class SplashWindow
{
public:
	//Constructors
	SplashWindow ();
	//These constructors call the appropriate createSplashWindow methods
	//r,g,b is the color of the bitmap that will be clear
	//specify r = -1,g = -1,b = -1 for no clear pixels
	SplashWindow (
               #ifdef _USE_VISUAL_STUDIO_
                                       LPCSTR
                                       #else
                                       const char *
                                       #endif
                                        bitmapFileName,
		const int r = -1, const int g = -1, const int b = -1);
	SplashWindow (DWORD bitmapResourceID,
		const int r = -1, const int g = -1, const int b = -1);
	//Destructor, calls destroySplashWindow
	~SplashWindow ();
	//Shows the splash screen modally. The window is destroyed when
	//this function is finished.
	//minDisplayTime is the minumum amount of time to display
	//	the splash window
	void showModalWindow (DWORD minDisplayTime);

	//These will setup the SplashWindow
	bool createSplashWindow (
	#ifndef _UBUNTU_
	 LPCSTR
	#else
	 const char *
	#endif
	 bitmapFileName, const int r, const int g, const int b);
	bool createSplashWindow (DWORD bitmapResourceID, const int r, const int g, const int b);

	//Do we have a valid window
	bool isValidWindow () const;

	#ifndef _UBUNTU_
	//Get the associated HWND
	HWND getHWND () const;

	#endif

	//This method can be overridden and is called in a thread,
	// so you can do tasks while the splash screen is displayed.
	virtual void loadResources () const;
	//Release the window
	void destroySplashWindow ();

	void hide();
	static bool staySleeping;
private:
	bool createWindowHelper (const int r, const int g, const int b);

	//sets all data members to 0.
	void clearMembers ();

	#ifndef _UBUNTU_
	//window handle
	HWND mHWND;
	//Splash Window image information
	HBITMAP  mBitmap;
	#endif

	int mBitmapWidth;
	int mBitmapHeight;
	//Thread data
	DWORD mMinDisplayTime;

		#ifndef _UBUNTU_
	CRITICAL_SECTION mLock;
		#endif

	bool mThreadFinished;

#ifndef _UBUNTU_

	//This is the internal thread function that calls
	//SplashWindow::loadResources ()
	friend DWORD WINAPI SplashWindow_ThreadProc (LPVOID parameter);

#endif

};


#endif
