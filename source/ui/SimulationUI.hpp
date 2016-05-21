#ifndef SIMULATION_UI_HEADER
#define SIMULATION_UI_HEADER

#include "UserInterface.hpp"
#include "../sensors/VideoDevice.hpp"

class SimulationUI: public UserInterface
{
 public:
   virtual void close();
   virtual void handleKeypress(unsigned char key,bool special);
   virtual void mouseMove(int x,int y);
   virtual void mouseUp(int x,int y);
   virtual void mouseDown(int x,int y);
   virtual void mouseWheel(short amount);

   virtual void draw();
   void draw(VideoDevice *cam1);
   virtual void update() const;
   void update(VideoDevice * cam1) const;
};

#endif
