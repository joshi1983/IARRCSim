#include "Vector2D.hpp"

#include <cmath>

using namespace std;

Vector2D::Vector2D(): x(0),y(0)
{
}

Vector2D::Vector2D(double x,double y):x(x),y(y)
{
}

Vector2D::Vector2D(const Vector2D &v):x(v.x),y(v.y)
{
}

void Vector2D::set(double x,double y)
{
  this->x=x;
  this->y=y;
}

double Vector2D::getX() const
{
  return x;
}

double Vector2D::getY() const
{
  return y;
}

void Vector2D::writeTo(ostream &out) const
{
  out << x<<" "<<y;
}

void Vector2D::readFrom(istream &in)
{
  in >> x >> y;
}

double Vector2D::getMagnitude() const
{
  return sqrt(x*x+y*y);
}

double Vector2D::getDistanceFrom(const Vector2D &v) const
{
  double dx=v.x-x;
  double dy=v.y-y;

  return sqrt(dx*dx+dy*dy);
}

double Vector2D::getDistanceFrom(double x,double y) const
{
  double dx=this->x -x;
  double dy=this->y -y;

  return sqrt(dx*dx+dy*dy);
}

void Vector2D::add(const Vector2D &v)
{
  x+=v.x;
  y+=v.y;
}

Vector2D Vector2D::getTimes(double d)
{
  Vector2D result(x*d,y*d);

  return result;
}

void Vector2D::add(double x,double y)
{
  this->x+=x;
  this->y+=y;
}
