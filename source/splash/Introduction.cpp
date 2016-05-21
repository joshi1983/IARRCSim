#include "Introduction.hpp"

#include "SplashWindow.hpp"
#include "../threads/Thread.hpp"
#include <iostream>

using namespace std;

//If you want to load something while the splash screen is being 
//displayed, put it in loadResources.
class MySplashWindow : public SplashWindow
{
public:
	void loadResources () const
	{
		//put code here
	}
};

//Just display a SplashWindow, no transparency, regular rectangular window
SplashWindow *sw=NULL;

void showTheWindowInOtherThread(void*p)
{
  sw=new SplashWindow("images/intro.bmp");

	if (!sw->isValidWindow ())
	{
		return;
	}

	sw->showModalWindow (10000);
}

void ShowIntro()
{
	Thread::startThread(showTheWindowInOtherThread,NULL);
}

void HideIntro()
{
	SplashWindow::staySleeping=false;
}