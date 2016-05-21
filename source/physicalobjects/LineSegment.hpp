#ifndef LINESEGMENT_HEADER
#define LINESEGMENT_HEADER

#include "../vectors/Vector3D.hpp"
#include "../vectors/Rotator3D.hpp"
#include <string>

class LineSegment
{
 private:
   Vector3D start,end;

 public:
   LineSegment(double x1,double y1,double z1,double x2,double y2,double z2);
   LineSegment(const Vector3D & start,const Vector3D & end);
   LineSegment(const LineSegment & line);
   LineSegment & operator=(const LineSegment & line);

   void rotate(const Rotator3D & rotator);
   void translateBy(const Vector3D & displacement);
   std::string getInFormat(const std::string & format) const;
};


#endif