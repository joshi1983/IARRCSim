#include "UseTextureCommand.hpp"

#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

double unitRangeRestrict(double val)
{
  if (val<0)
	  return 0;
  else if (val>1)
       return 1;
  else
	  return val;
}

UseTextureCommand::UseTextureCommand(Texture *t): texture(t),
 r(0),g(0),b(0)
{

}

UseTextureCommand::UseTextureCommand(double r,double g,double b): texture(NULL),
 r(unitRangeRestrict(r)),g(unitRangeRestrict(g)),b(unitRangeRestrict(b))
{

}

UseTextureCommand::~UseTextureCommand()
{
  delete texture;
}

void UseTextureCommand::draw(int windowid,const Vector3D &viewpoint)
{
 if (texture==NULL)
 {
   glDisable(GL_TEXTURE_2D);
   glColor3d(r,g,b);
 }
 else
 {
	glColor3d(1,1,1);
     glEnable( GL_TEXTURE_2D );

	if (texture->isAlphaBlended())
	{
	   glEnable(GL_BLEND);
	   glAlphaFunc(GL_GREATER,(GLclampf)0.1);
	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	   glEnable(GL_ALPHA_TEST);

	}

     glBindTexture( GL_TEXTURE_2D, texture->getOpenGLTextureName(windowid) );
 }
}

void UseTextureCommand::drawForDesign(int windowid)
{
 if (texture==NULL)
 {
   glDisable(GL_TEXTURE_2D);
   glColor3d(r,g,b);
 }
 else
 {
	 glColor3d(1,1,1);
      glEnable( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, texture->getOpenGLTextureName(windowid) );
 }
}
