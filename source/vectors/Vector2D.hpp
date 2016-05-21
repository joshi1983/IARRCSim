#ifndef VECTOR2D_HEADER
#define VECTOR2D_HEADER

#include <iostream>

class Vector2D
{
private:
	double x,y;
public:
	Vector2D();
	Vector2D(const Vector2D &v);
	Vector2D(double x,double y);
	void set(double x,double y);
	double getX() const;
	double getY() const;
	void add(const Vector2D &v);
	void add(double x,double y);
	Vector2D getTimes(double d);
	double getDistanceFrom(double x,double y) const;
	double getDistanceFrom(const Vector2D &v) const;
	void writeTo(std::ostream &out) const;
	void readFrom(std::istream &in);
	double getMagnitude() const;
};

#endif
