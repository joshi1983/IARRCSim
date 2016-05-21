#include "LineSegment.hpp"
#include <sstream>

using namespace std;

LineSegment::LineSegment(double x1,double y1,double z1,double x2,double y2,double z2):
 start(x1,y1,z1),end(x2,y2,z2)
{

}

LineSegment::LineSegment(const LineSegment & line): start(line.start),end(line.end)
{

}

LineSegment::LineSegment(const Vector3D & start,const Vector3D & end):
  start(start),end(end)
{

}

std::string LineSegment::getInFormat(const std::string & format) const
{
 ostringstream result;

  if (format=="JSON")
  {
     result<<"["<<start.getFormatted(format)<<","<<end.getFormatted(format)<<"]";
  }
  else if (format=="xml")
  {
     result<<"<line>"<<start.getFormatted(format)<<endl
		<<end.getFormatted(format)<<"</line>";
  }

  return result.str();
}



LineSegment & LineSegment::operator=(const LineSegment & line)
{
  this->start=line.start;
  this->end=line.end;

  return *this;
}

void LineSegment::rotate(const Rotator3D & rotator)
{
   rotator.rotate(start);
   rotator.rotate(end);
}

void LineSegment::translateBy(const Vector3D & displacement)
{
   start.add(displacement);
   end.add(displacement);
}