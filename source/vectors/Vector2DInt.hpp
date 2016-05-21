#ifndef VECTOR2D_INT_HEADER
#define VECTOR2D_INT_HEADER

#include "../stdafx.h"

#ifndef _UBUNTU_

 #include <windows.h>

#endif

class Vector2DInt
{

  public:
   int x,y;

   Vector2DInt();
   Vector2DInt(int x,int y);
   void set(int x,int y);

 #ifndef _UBUNTU_

   void copyTo(POINT & p) const; 

 #endif
};

#endif
