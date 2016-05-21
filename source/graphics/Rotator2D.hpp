#ifndef ROTATOR_2D_HEADER
#define ROTATOR_2D_HEADER

#include "../vectors/Vector2D.hpp"
#include "../vectors/Vector3D.hpp"

class Rotator2D
{
 private:
  double angle; // in radians
  double sin_angle;
  double cos_angle;

 public:
  Rotator2D();
  Rotator2D(double radianAngle);
  void rotate(double &x,double &y) const;
  void rotate(Vector2D &v) const;
  void rotateXZ(Vector3D &v) const;
  void rotateYZ(Vector3D &v) const;

  void setAngleRadians(double radianAngle);
  void setAngleDegrees(double degrees);

  double getAngleInRadians() const;

};

#endif