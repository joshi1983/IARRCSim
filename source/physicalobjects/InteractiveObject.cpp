#include "InteractiveObject.hpp"


InteractiveObject::InteractiveObject(double x,double y,double z,double mass): 
 PhysicalObject(x,y,z),axisRotationDegrees(0),axis(1,0,0)
{
	this->mass=mass;
//	assert(mass>0);
}

double InteractiveObject::getMass() const
{
   return this->mass;
}

void InteractiveObject::writeTo(std::ostream &out) const
{
  PhysicalObject::writeTo(out);
  velocity.writeTo(out);
  out <<" "<< mass;
}

void InteractiveObject::readFrom(std::istream &in)
{
	PhysicalObject::readFrom(in);
	velocity.readFrom(in);
	in >> mass;
}

double InteractiveObject::getSpeed() const
{
  return velocity.getMagnitude();
}

InteractiveObject::~InteractiveObject()
{
}

void InteractiveObject::ticksPerformed(Environment * e,int milis)
{
    Vector2D p(position.getX(),position.getZ());

    Vector3D normal;
    e->getTopNormal(p,normal);
    updateAxisRotation(normal);
}

void InteractiveObject::updateAxisRotation(const Vector3D & normal)
{
  normal.convertToRotation(axis,axisRotationDegrees);
}

double InteractiveObject::getAxisRotationDegrees() const
{
   return axisRotationDegrees;
}
   
Vector3D InteractiveObject::getRotationAxis() const
{
   return axis;
}

