#include "Rotator2D.hpp"
#include "../stdafx.h"
#include <cmath>

Rotator2D::Rotator2D()
{
  setAngleRadians(0);
}

Rotator2D::Rotator2D(double radianAngle)
{
  setAngleRadians(radianAngle);
}

void Rotator2D::rotate(double &x,double &y) const
{
 double xt=x*cos_angle-y*sin_angle;
 double yt=x*sin_angle+y*cos_angle;

  x=xt;
  y=yt;
}

void Rotator2D::rotate(Vector2D &v) const
{
 double x=v.getX()*cos_angle-v.getY()*sin_angle;
 double y=v.getX()*sin_angle+v.getY()*cos_angle;

  v.set(x,y);
}

void Rotator2D::rotateXZ(Vector3D &v) const
{
  Vector2D v2d(v.getX(),v.getZ());

   rotate(v2d);

   v.set(v2d.getX(),v.getY(),v2d.getY());
}

void Rotator2D::rotateYZ(Vector3D &v) const
{
  Vector2D v2d(v.getY(),v.getZ());

   rotate(v2d);

   v.set(v.getX(),v2d.getX(),v2d.getY());
}

void Rotator2D::setAngleRadians(double radianAngle)
{
   angle=radianAngle;
   sin_angle=sin(angle);
   cos_angle=cos(angle);
}

void Rotator2D::setAngleDegrees(double degrees)
{
   setAngleRadians(degrees*PI/180);
}

double Rotator2D::getAngleInRadians() const
{
  return angle;
}

