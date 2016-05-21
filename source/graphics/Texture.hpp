#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <list>
#include "TextureContext.hpp"

class Texture
{
 private:
  std::list<TextureContext*> contexts;
  int width;
  int height;
  bool alphaBlended;
  void init(const char*bitmapfilename,bool redTransparent=false
	  ,bool blendPixels=true,int r=255,int g=0,int b=0);

  GLuint generateTextureForWindow(int windowid,const char*bitmapfilename,bool redTransparent,
			    bool blendPixels,const int r,const int g,const int b);

 public:
   Texture(const char*bitmapfilename);
   Texture(const char*bitmapfilename,bool redTransparent,bool blendPixels=true);
   Texture(const char*bitmapfilename,const int transparent_r,
	   const int transparent_g,const int transparent_b);

   virtual ~Texture();
   void draw2D(int windowid,int x,int y) const;
   void stretchDraw(int windowid,int left,int top,int width,int height) const;
   int getWidth() const;
   int getHeight() const;
   bool isAlphaBlended() const;
   GLuint getOpenGLTextureName(int windowid) const;
};

#endif
