#include "Vector3D.hpp"
#include "../stdafx.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include "Rotator3D.hpp"

#include "../graphics/Rotator2D.hpp"

using namespace std;

Vector3D::Vector3D(double x,double y,double z): x(x),y(y),z(z)
{
}

Vector3D::Vector3D(const Vector3D &v):x(v.x),y(v.y),z(v.z)
{
}

Vector3D::Vector3D(): x(0),y(0),z(0)
{
}

Vector3D & Vector3D::operator=(const Vector3D &v)
{
   x=v.x;
   y=v.y;
   z=v.z;

   return *this;
}

void Vector3D::writeTo(std::ostream &out) const
{
  out << x<<" ";
  out << y<<" ";
  out << z<<" ";
}

void Vector3D::readFrom(std::istream &in)
{
  in >> x >> y >> z;
}

void Vector3D::set(double x,double y,double z)
{
  this->x=x;
  this->y=y;
  this->z=z;
}

void Vector3D::setX(double x)
{
  this->x=x;
}

void Vector3D::setY(double y)
{
  this->y=y;
}

void Vector3D::setZ(double z)
{
  this->z=z;
}

void Vector3D::add(const Vector3D &v)
{
  x+=v.x;
  y+=v.y;
  z+=v.z;
}

void Vector3D::add(double x,double y,double z)
{
  this->x+=x;
  this->y+=y;
  this->z+=z;
}

void Vector3D::sub(const Vector3D &v)
{
  x-=v.x;
  y-=v.y;
  z-=v.z;
}

void Vector3D::rotateAroundYBy(double radians)
{
 Rotator2D rotator(radians);

  rotator.rotate(x,z);
}

double Vector3D::getX() const
{
  return x;
}

double Vector3D::getY() const
{
  return y;
}

double Vector3D::getZ() const
{
  return z;
}

Vector3D Vector3D::operator*(double d) const
{
  Vector3D result(d*this->x,d*this->y,d*this->z);

   return result;
}

double Vector3D::getMagnitude() const
{
  return sqrt(x*x+y*y+z*z);
}

double Vector3D::distanceFrom(const Vector3D &v) const
{
 double x=v.x-this->x,y=v.y-this->y,z=v.z-this->z;

  return sqrt(x*x+y*y+z*z);
}

Vector3D Vector3D::operator-(const Vector3D &v) const
{
  Vector3D result(x-v.x,y-v.y,z-v.z);

  return result;
}

void Vector3D::makeUnitVector()
{
 double m=getMagnitude();

  if (m<0.000001)
  {
    x=1;
    y=0;
    z=0;
    return;
  }

  m=1/m; /* because dividing is usually less efficient than multiplication,
	    1 division followed by 3 multiplications may be better than 3 divisions.
	    */
  x*=m;
  y*=m;
  z*=m;
}

void Vector3D::crossProduct(const Vector3D &c)
{
  double ax,ay;

  ax=y*c.z-z*c.y;
  ay=z*c.x-x*c.z;
  z=x*c.y-y*c.x;

  x=ax;
  y=ay;
}

double Vector3D::dot(const Vector3D &v) const
{
  return x*v.x+y*v.y+z*v.z;
}

double Vector3D::getSeparationAngleRadians(const Vector3D &v) const
{
  double ab=getMagnitude()*v.getMagnitude();

  if (ab<0.000001)
	  return 0; // avoid division by zero and just return 0 angle difference.

  return acos(dot(v)/ab);
}

double Vector3D::getSeparationAngleDegrees(const Vector3D &v) const
{
  return getSeparationAngleRadians(v)*180/PI;
}

void Vector3D::convertToRotation(Vector3D &axis,double &degrees) const
{

  // this dot (0,1,0) is just this->y.
  double dotResult = this->y;

  degrees=acos(dotResult/this->getMagnitude())*180/PI;

    axis.set(-this->z,0,this->x);

}

void Vector3D::rotateBy(double degrees,double axis_x,double axis_y,double axis_z)
{
 Rotator3D r(degrees*PI/180,axis_x,axis_y,axis_z);

  r.rotate(*this);
}

Vector3D Vector3D::operator-() const
{
  Vector3D result(-x,-y,-z);

  return result;
}

Vector3D Vector3D::operator/(double d) const
{
  if (abs(d)<0.00001)
	return Vector3D();

  Vector3D result(x/d,y/d,z/d);

  return result;
}

 Vector3D Vector3D::operator+(const Vector3D & v) const
 {
   Vector3D result(v.x+x,v.y+y,v.z+z);

    return result;
 }

Vector3D operator*(const double scalar,const Vector3D & v)
{
  Vector3D result(v.getX()*scalar,v.getY()*scalar,v.getZ()*scalar);

    return result;
}

std::string Vector3D::getFormatted(const std::string & format) const
{
 ostringstream result;

  if (format=="JSON")
  {
	  result<<"{\"x\": "<<x<<", \"y\": "<<y<<", \"z\": "<<z<<"}";
  }
  else if (format=="xml")
  {
     result<<"<vector x=\""<<x<<"\" y=\""<<y<<"\" z=\""<<z<<"\" />";
  }

  return result.str();
}
