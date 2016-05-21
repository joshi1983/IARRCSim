#ifndef GRAPHICS_2D_HEADER
#define GRAPHICS_2D_HEADER

#include "../stdafx.h"

#include "../vectors/Vector2DInt.hpp"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

/**
 Some functions for drawing in 2 dimensions using OpenGL functions.
*/
class Graphics2D
{
 public:
   static void fillRectangle(int left,int top,int width,int height);
   static void fillPolygon(const Vector2DInt *points,int numPoints,
	   int left,int top,int width,int height);

   static void fillPolygon(const Vector2DInt * points,int numPoints);
   static void drawRectangle(int left,int top,int width,int height);
   static void drawLine(double x1,double y1,double x2,double y2,double width=1);
   static void setupFor2DGraphics();
   static void setupFor2DGraphics(int width,int height);
};

#endif
