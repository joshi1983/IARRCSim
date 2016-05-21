#ifndef USE_TEXTURE_COMMAND_HEADER
#define USE_TEXTURE_COMMAND_HEADER

#include "../graphics/Texture.hpp"
#include "GCommand.hpp"

class UseTextureCommand: public GCommand
{
 private:
   Texture *texture;
   double r,g,b; // used only if the texture is NULL.

 public:
  UseTextureCommand(Texture *t);
  UseTextureCommand(double r,double g,double b);
  virtual ~UseTextureCommand();
  virtual void draw(int windowid,const Vector3D &viewpoint);
  virtual void drawForDesign(int windowid);

};


#endif
