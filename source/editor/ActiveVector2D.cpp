#include "ActiveVector2D.hpp"

ActiveVector2D::ActiveVector2D(double x,double y,MouseMotionListener *ml)
 : mlistener(ml),v(Vector2D(x,y))
{

}

ActiveVector2D::~ActiveVector2D()
{
  //if (mlistener!=NULL)
  //   delete mlistener;
}

MouseMotionListener *ActiveVector2D::getMouseMotionListener() const
{
  return mlistener;
}

Vector2D ActiveVector2D::getVector2D() const
{
  return v;
}

double ActiveVector2D::getX() const
{
  return v.getX();
}

double ActiveVector2D::getY() const
{
  return v.getY();
}