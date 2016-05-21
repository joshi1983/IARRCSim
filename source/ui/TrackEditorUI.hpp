#ifndef TRACK_EDITOR_UI_HEADER
#define TRACK_EDITOR_UI_HEADER

#include "UserInterface.hpp"
#include "../editor/CircuitDesign.hpp"

class TrackEditorUI: public UserInterface
{
private:
	CircuitDesign design;

 public:
   void launchHelp() const;

   virtual void close();
   virtual void handleKeypress(unsigned char key,bool special);
   virtual void mouseDown(int x,int y);
   virtual void mouseMove(int x,int y);
   virtual void mouseUp(int x,int y);
   virtual void mouseWheel(short amount);
   virtual void draw();
   virtual void update() const;
};


#endif
