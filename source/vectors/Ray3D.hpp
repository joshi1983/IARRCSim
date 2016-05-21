#ifndef RAY3D_HEADER
#define RAY3D_HEADER

#include "Vector3D.hpp"
#include "../graphics/Rotator2D.hpp"

class Ray3D
{
 private:
   Vector3D position;
   Vector3D direction;

 public:
   Ray3D(const Ray3D &r);
   Ray3D(const Vector3D & position,const Vector3D &dir);

   /**
   Rotates a specified number of degrees around a specified axis(axis_x,axis_y,axis_z)
   */
   void rotateBy(double degrees,double axis_x,double axis_y,double axis_z);

   /**
   A bit more efficient than the more general rotateBy method
   */ 
   void rotateAroundYBy(double radians);

   void rotateUsing(const Rotator2D & rotator);
   void rotateYZUsing(const Rotator2D & rotator);

/**
Adds the specified values to position.
*/
   void translateBy(double x,double y,double z);
   void translateBy(const Vector3D &v);

   const Vector3D &getPosition() const;
   const Vector3D &getDirection() const;

   void setPosition(const Vector3D &position);
   void setDirection(const Vector3D &direction);
};

#endif