#include "PhysicalObject.hpp"
#include "../app/Environment.hpp"
#include "../app/Application.hpp"
#include "../editor/RepositionMouseMotionListener.hpp"
#include <cmath>
#include <sstream>

using namespace std;

PhysicalObject::PhysicalObject(double x,double y,double z)
{
  position.set(x,y,z);
  enforcePositionBoundaries();
}

const Vector3D & PhysicalObject::getPosition() const
{
  return position;
}

void PhysicalObject::enforcePositionBoundaries()
{
 Environment & e = Application::getMainEnvironment();

  //cout << "PhysicalObject::enforcePositionBoundaries called."<<endl;
  if (position.getX()<e.getBoundaryMinX())
     position.setX(e.getBoundaryMinX());
  if (position.getX()>e.getBoundaryMaxX())
       position.setX(e.getBoundaryMaxX());

  if (position.getZ()<e.getBoundaryMinZ())
     position.setZ(e.getBoundaryMinZ());
  if (position.getZ()>e.getBoundaryMaxZ())
       position.setZ(e.getBoundaryMaxZ());

  if (position.getY()<e.getBoundaryMinY())
     position.setY(e.getBoundaryMinY());
  if (position.getY()>e.getBoundaryMaxY())
       position.setY(e.getBoundaryMaxY());
}

Vector3D PhysicalObject::getVelocity() const
{
  return Vector3D(0,0,0);
}

void PhysicalObject::drawShadows(const Vector3D &lightDirection) const
{

}

double PhysicalObject::getXZDistanceFrom(const Vector2D &pos) const
{
 double x=pos.getX()-position.getX(),z=pos.getY()-position.getZ();

  return sqrt(x*x+z*z);
}

void PhysicalObject::getActiveXZPointsInRange(
  double x,double z,double maxDistance,list<ActiveVector2D> &results)
{
  if (getXZDistanceFrom(x,z)<=maxDistance)
	  results.push_back(ActiveVector2D(getX(),getZ(),
	   new RepositionMouseMotionListener(this)));
}

double PhysicalObject::getUIRadius() const
{
   return 1000;
}

double PhysicalObject::getX() const
{
  return position.getX();
}

double PhysicalObject::getY() const
{
  return position.getY();
}

double PhysicalObject::getZ() const
{
  return position.getZ();
}

double PhysicalObject::getXZDistanceFrom(double x,double z) const
{
  x-=position.getX();
  z-=position.getZ();

  return sqrt(x*x+z*z);
}

void PhysicalObject::setXZPosition(double x,double z)
{
 position.setX(x);
 position.setZ(z);
}

void PhysicalObject::setX(double x)
{
  position.setX(x);
}

void PhysicalObject::setY(double y)
{
  position.setY(y);
}

void PhysicalObject::setZ(double z)
{
  position.setZ(z);
}

void PhysicalObject::writeTo(std::ostream &out) const
{
  position.writeTo(out);
}

void PhysicalObject::readFrom(std::istream &in)
{
  position.readFrom(in);
}

std::string PhysicalObject::getName() const
{
  return "PhysicalObject";
}

PhysicalObject::~PhysicalObject()
{
}

double PhysicalObject::getElevationForXZPoint(const Vector2D &p) const
{
  return 0;
}

void PhysicalObject::getTopNormal(const Vector2D &xzPoint,Vector3D &normal) const
{
  normal.set(0,1,0);
}

double PhysicalObject::getIntersectionCoefficient(const Ray3D &r) const
{
  return 0;
}

bool PhysicalObject::setProperty(const std::string &propertyName,double value)
{
  if (propertyName=="position-x")
	  position.setX(value);
  else if (propertyName=="position-y")
	  position.setY(value);
  else if (propertyName=="position-z")
	  position.setZ(value);
  else
	  return false;

  return true;
}

void PhysicalObject::addShapeRepresentation(ShapeCollection &shapes) const
{
 // add nothing.
}

void PhysicalObject::join(PhysicalObject * obj)
{
 // do nothing.
}

void PhysicalObject::getAsLines(std::vector<LineSegment> &lines) const
{

}

std::string PhysicalObject::getDescription(const std::string &format) const
{
 ostringstream result;

   if (format=="xml")
   {
        result<<"<obstacle type=\""<<this->getName()
        <<"\" x=\""<<this->getX()<<"\""
        <<" y=\""<<this->getY()<<"\""
        <<" z=\""<<this->getZ()<<"\""
        <<">"<<endl;

        result<<"</obstacle>";
   }
   else
   {
      result<<getName()<<" ";
      writeTo(result);
   }

  return result.str();
}

