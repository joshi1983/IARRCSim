#include "Vector2DInt.hpp"

Vector2DInt::Vector2DInt(): x(0),y(0)
{

}

Vector2DInt::Vector2DInt(int x,int y): x(x),y(y)
{

}

void Vector2DInt::set(int x,int y)
{
  this->x=x;
  this->y=y;
}


 #ifndef _UBUNTU_

void Vector2DInt::copyTo(POINT & p) const
{
  p.x=x;
  p.y=y;
}

 #endif

