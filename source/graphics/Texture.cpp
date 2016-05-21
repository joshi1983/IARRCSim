#include "Texture.hpp"
#include "BitmapImage.hpp"
#include "../app/Application.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

Texture::Texture(const char*bitmapfilename)
{
   init(bitmapfilename);
}

Texture::Texture(const char*bitmapfilename,bool redTransparent,bool blendPixels)
{
   init(bitmapfilename,redTransparent,blendPixels);
}

Texture::Texture(const char*bitmapfilename,int transparent_r,int transparent_g,int transparent_b)
{
   init(bitmapfilename,true,true,transparent_r,transparent_g,transparent_b);
}

void Texture::stretchDraw(int windowid,int left,int top,int width,int height) const
{
		glColor3d(1,1,1);
         // cout << "SensorEditorUI::draw() called. adding menu"<<endl;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,getOpenGLTextureName(windowid));
		glBegin(GL_QUADS);

		glTexCoord2d(0,0);
		glVertex2d(left,top);
		glTexCoord2d(1,0);
		glVertex2d(left+width,top);
		glTexCoord2d(1,1);
		glVertex2d(left+width,top+height);
		glTexCoord2d(0,1);
		glVertex2d(left,top+height);

		glEnd();
          glDisable(GL_TEXTURE_2D);
}

void Texture::draw2D(int windowid,int x,int y) const
{
		glColor3d(1,1,1);
         // cout << "SensorEditorUI::draw() called. adding menu"<<endl;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,getOpenGLTextureName(windowid));
		glBegin(GL_QUADS);

		glTexCoord2d(0,0);
		glVertex2d(x,y);
		glTexCoord2d(1,0);
		glVertex2d(x+getWidth(),y);
		glTexCoord2d(1,1);
		glVertex2d(x+getWidth(),y+getHeight());
		glTexCoord2d(0,1);
		glVertex2d(x,y+getHeight());

		glEnd();
          glDisable(GL_TEXTURE_2D);
}

bool Texture::isAlphaBlended() const
{
  return alphaBlended;
}

GLuint Texture::generateTextureForWindow(int windowid,const char*bitmapfilename,bool redTransparent,
			    bool blendPixels,const int r,const int g,const int b)
{
  GLuint  openGLName=0;

  glutSetWindow(windowid);

  alphaBlended=redTransparent;
  BitmapImage bitmap(bitmapfilename);

  // allocate a texture name
  glGenTextures( 1, &openGLName);
   //  cout << "glGenTextures used the name: "<<openGLName<<" on second call."<<endl;
  // select our current texture
  glBindTexture( GL_TEXTURE_2D, openGLName);

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  if (blendPixels)
 {
    // when texture area is small, bilinear filter the closest MIP map

     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );

      // when texture area is large, bilinear filter the first MIP map

	 glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 }
  else
  {
     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_NEAREST );
	 glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


  int byteCount=3;
  if (redTransparent)
	  byteCount=4;

 int widthStep = bitmap.getWidth()*byteCount;

 if (widthStep%4!=0)
    widthStep=(1+widthStep/4)*4;

BYTE*data=(BYTE*)malloc(widthStep*bitmap.getHeight());
bool useParameterRGB=false;

  if (data==NULL)
  {
    cout << "Texture::init unable to allocate required memory."<<endl;
    return -1;
  }
  if (r!=255 || g!=0 || b!=0)
  {
     useParameterRGB=true;
  }

  for (int y=0;y<bitmap.getHeight();y++)
  {
    for (int x=0;x<bitmap.getWidth();x++)
    {
      int red,green,blue;
	 int c= bitmap.getPixel(x,y);

	   red=bitmap.getRed(c);
        green=bitmap.getGreen(c);
        blue=bitmap.getBlue(c);

     int index=(x*byteCount+y*widthStep);

        data[index]=(BYTE)red;
	   data[index+1]=(BYTE)green;
	   data[index+2]=(BYTE)blue;

	   if (redTransparent)
	   {
		   if (useParameterRGB)
		   {
		      if (abs(r-red)<1 && abs(g-green)<1 && abs(b-blue)<1)
			 {
			   data[index+3]=0;
			   // transparent
			 }
			 else
                    data[index+3]=0xff;
		   }
		   else
		   {
		      if (red>225 && green<40 && blue<40)
			     data[index+3]=0;
		      else
		          data[index+3]=0xff;
		   }
	   }
    }
  }

  if (redTransparent)
  {
    // build our texture MIP maps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 4, bitmap.getWidth(),
      bitmap.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
  else
  {
    // build our texture MIP maps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, bitmap.getWidth(),
      bitmap.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, data);
  }

  free(data);

  this->width = bitmap.getWidth();
  this->height = bitmap.getHeight();

  //glGenTextures( 1, &openGLName);
  //cout << "glGenTextures used the name on second time: "<<openGLName<<endl;

  return openGLName;
}

void Texture::init(const char*bitmapfilename,bool redTransparent,
			    bool blendPixels,const int r,const int g,const int b)
{
 vector<int> windowids = Application::getAllSensors().getWindowIDs();

   // loop through all VideoDevice windows.
   for (unsigned int i=0;i<windowids.size();i++)
   {
      int windowid = windowids[i];
      GLuint openGLName = generateTextureForWindow(windowid,bitmapfilename,redTransparent,blendPixels,r,g,b);
      contexts.push_back(new TextureContext(openGLName,windowid));
   }
}

Texture::~Texture()
{
  //glDeleteTextures( 1, &openGLName );
  for (list<TextureContext*>::iterator it=contexts.begin();it!=contexts.end();it++)
  {
     delete (*it);
  }
}

int Texture::getWidth() const
{
  return width;
}

int Texture::getHeight() const
{
  return height;
}

GLuint Texture::getOpenGLTextureName(int windowid) const
{
    for (list<TextureContext*>::const_iterator it=contexts.begin();it!=contexts.end();it++)
    {
        if ((*it)->getWindowID()==windowid)
        {
            GLuint result=(*it)->getOpenGLName();
            return result;
        }
    }

  cout << "Can't find TextureContext match for windowid "<<windowid<<endl;
  return 0;
}
