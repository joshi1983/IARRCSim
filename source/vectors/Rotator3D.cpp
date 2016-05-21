#include "Rotator3D.hpp"

#include <cmath>

/*
math reference:

http://en.wikipedia.org/wiki/Rotation_matrix

http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_given_an_axis_and_an_angle
*/

Rotator3D::Rotator3D(double rotationRadians,const Vector3D &axis):
  rotationRadians(rotationRadians),axis(axis),m(3,3)
{
 updateRotationMatrix();
}

Rotator3D::Rotator3D(double rotationRadians,double x,double y,double z):
 rotationRadians(rotationRadians),axis(x,y,z),m(3,3)
{
 updateRotationMatrix();
}

void Rotator3D::setAxis(double x,double y,double z)
{
  axis.set(x,y,z);
  updateRotationMatrix();
}

void Rotator3D::setRotation(double radians)
{
  this->rotationRadians=radians;
  updateRotationMatrix();
}

void Rotator3D::updateRotationMatrix()
{	
  axis.makeUnitVector();

 double c=cos(rotationRadians);
 double s=sin(rotationRadians);
 double Ux=axis.getX();
 double Uy=axis.getY();
 double Uz=axis.getZ();
 double Ux2=Ux*Ux;
 double Uy2=Uy*Uy;
 double Uz2=Uz*Uz;

  m.set(0,0,Ux2+(1-Ux2)*c);  m.set(0,1,Ux*Uy*(1-c)-Uz*s);  m.set(0,2,Ux*Uz*(1-c)+Uy*s);
  m.set(1,0,Ux*Uy*(1-c)+Uz*s);  m.set(1,1,Uy2+(1-Uy2)*c);  m.set(1,2,Uy*Uz*(1-c)-Ux*s);
  m.set(2,0,Ux*Uz*(1-c)-Uy*s);  m.set(2,1,Uy*Uz*(1-c)+Ux*s);  m.set(2,2,Uz2+(1-Uz2)*c);

}

void Rotator3D::rotate(Vector3D & point) const
{
  m.mul(point,point);
}
