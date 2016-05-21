#ifndef TEXTURE_CONTEXT_HEADER
#define TEXTURE_CONTEXT_HEADER

#include "../stdafx.h"


class TextureContext
{
 private:
  GLuint openGLName;
  int windowid;

 public:
   TextureContext(GLuint openGLName,int windowid);
   virtual ~TextureContext();
   GLuint getOpenGLName() const;
   int getWindowID() const;
};

#endif
