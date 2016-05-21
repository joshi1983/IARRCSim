#ifndef VECTOR_3D_HEADER
#define VECTOR_3D_HEADER

#include <iostream>
#include <string>

class Matrix;

class Vector3D
{
	friend class Matrix;

private:
	double x,y,z;
  public:

		Vector3D();
     Vector3D(double x,double y,double z);
	  Vector3D(const Vector3D &v);
	  Vector3D & operator=(const Vector3D &v);
	  std::string getFormatted(const std::string & format) const;
	  void writeTo(std::ostream &out) const;
	  void readFrom(std::istream &in);
       void set(double x,double y,double z);
       Vector3D operator+(const Vector3D & v) const;
	  void add(const Vector3D &v);
	  void add(double x,double y,double z);
	  void sub(const Vector3D &v);
       double proj(const Vector3D &dir);

	  void rotateAroundYBy(double radians);

	  Vector3D operator*(double d) const;
	  Vector3D operator/(double d) const;
	  void setX(double x);
	  void setY(double y);
	  void setZ(double z);
	  double getX() const;
	  double getY() const;
	  double getZ() const;
	  double getMagnitude() const;
	  void makeUnitVector();
	  void crossProduct(const Vector3D &v);
	  double dot(const Vector3D &v) const;


	  double getSeparationAngleRadians(const Vector3D &v) const;
	  double getSeparationAngleDegrees(const Vector3D &v) const;

	  /**
	  Converts this vector into a rotation of (0,1,0) around a derived axis and at a derived angle in degrees.
	  */
	  void convertToRotation(Vector3D &axis,double &degrees) const;
	  double distanceFrom(const Vector3D &v) const;
	  Vector3D operator-(const Vector3D &v) const;

	  // unary negative sign.
	  Vector3D operator-() const;

  /**
   Rotates a specified number of degrees around a specified axis(axis_x,axis_y,axis_z)
   */
   void rotateBy(double degrees,double axis_x,double axis_y,double axis_z);
};

Vector3D operator*(const double scalar,const Vector3D & v);

#endif
