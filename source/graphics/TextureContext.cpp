#include "TextureContext.hpp"

using namespace std;

TextureContext::TextureContext(GLuint openGLName,int windowid):openGLName(openGLName),windowid(windowid)
{

}

TextureContext::~TextureContext()
{
  glDeleteTextures( 1, &openGLName );
}

GLuint TextureContext::getOpenGLName() const
{
  return openGLName;
}

int TextureContext::getWindowID() const
{
  return windowid;
}
