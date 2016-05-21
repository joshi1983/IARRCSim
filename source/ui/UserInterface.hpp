#ifndef USER_INTERFACE_HEADER
#define USER_INTERFACE_HEADER

#include "../stdafx.h"

class UserInterface
{
 public:
	 /**
	 Closes this user interface.
	 Does what is needed before switching out of this user interface.
	 */
	virtual void close()=0;
	virtual void handleKeypress(unsigned char key,bool special);
     virtual void mouseDown(int x,int y);
	virtual void mouseMove(int x,int y);
	virtual void mouseUp(int x,int y);
	virtual void mouseWheel(short amount);
	virtual void draw()=0;

	/**
	Basically the opposite of close().  update()
	is called on when the window is resized or this user
	interface becomes visible.
	*/
	virtual void update() const=0;
};

#endif
