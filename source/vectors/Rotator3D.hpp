#ifndef ROTATOR_3D_HEADER
#define ROTATOR_3D_HEADER

#include "Vector3D.hpp"
#include "Matrix.hpp"

class Rotator3D
{
 private:
   Vector3D axis; // axis of rotation   
   double rotationRadians;
   Matrix m;

   void updateRotationMatrix();

 public:
   Rotator3D(double rotationRadians,const Vector3D &axis);
   Rotator3D(double rotationRadians,double x,double y,double z);

   void setAxis(double x,double y,double z);
   void setRotation(double radians);
   void rotate(Vector3D & point) const;
};

#endif