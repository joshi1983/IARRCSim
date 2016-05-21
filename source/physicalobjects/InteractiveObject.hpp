#ifndef INTERACTIVE_OBJECT_HEADER
#define INTERACTIVE_OBJECT_HEADER


#include "PhysicalObject.hpp"
#include "../vectors/Vector3D.hpp"

class InteractiveObject;

#include "../app/Environment.hpp"


/**
Represents objects that interact with the environment and 
mutate in reaction to certain events.  

The events can include things like collisions.
Examples of interactive objects include pylons, and robotic cars.
Pylons react to certain events such as collisions.  They react by sliding or tipping over.
Robotic cars react by moving also.
*/
class InteractiveObject: public PhysicalObject
{
private:
    double mass;


protected:
    Vector3D velocity;
  
    Vector3D axis;
    double axisRotationDegrees;

    void updateAxisRotation(const Vector3D & normal);

public:
	/**
	mass is assumed to be in grams.
	*/
  InteractiveObject(double x,double y,double z,double mass);
  virtual ~InteractiveObject();
   double getSpeed() const;
   double getMass() const;

   double getAxisRotationDegrees() const;
   Vector3D getRotationAxis() const;

  virtual void ticksPerformed(Environment * e,int milis);
   // for simulating the passage of time.
  virtual void writeTo(std::ostream & out) const;
  virtual void readFrom(std::istream &in);
  virtual std::string getName() const=0;
};

#endif