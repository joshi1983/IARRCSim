#include "Ray3D.hpp"
#include "Rotator3D.hpp"
#include "../stdafx.h"

Ray3D::Ray3D(const Ray3D &r):
 position(r.position),direction(r.direction)
{

}

Ray3D::Ray3D(const Vector3D & position,const Vector3D &dir):
 position(position),direction(dir)
{

}

const Vector3D &Ray3D::getPosition() const
{
  return position;
}

const Vector3D &Ray3D::getDirection() const
{
  return direction;
}

void Ray3D::setPosition(const Vector3D &position)
{
  this->position=position;
}

void Ray3D::setDirection(const Vector3D &direction)
{
  this->direction=direction;
  this->direction.makeUnitVector();
}

/**
 Rotates a specified number of degrees around a specified 
 axis(axis_x,axis_y,axis_z)
*/
void Ray3D::rotateBy(double degrees,double axis_x,double axis_y,double axis_z)
{
 Rotator3D r(degrees*PI/180,axis_x,axis_y,axis_z);

 r.rotate(position);
 r.rotate(direction);
}

void Ray3D::rotateUsing(const Rotator2D & rotator)
{
	rotator.rotateXZ(position);
	rotator.rotateXZ(direction);
}

void Ray3D::rotateYZUsing(const Rotator2D & rotator)
{
 rotator.rotateYZ(position);
 rotator.rotateYZ(direction);
}

void Ray3D::rotateAroundYBy(double radians)
{
 position.rotateAroundYBy(radians);
 direction.rotateAroundYBy(radians);
}

void Ray3D::translateBy(double x,double y,double z)
{
   position.add(x,y,z);
}

void Ray3D::translateBy(const Vector3D &v)
{
  position.add(v.getX(),v.getY(),v.getZ());
}