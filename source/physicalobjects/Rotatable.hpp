#ifndef ROTATABLE_HEADER
#define ROTATABLE_HEADER

#include "../stdafx.h"
#include <list>
#include "../editor/ActiveVector2D.hpp"
#include "../graphics/Rotator2D.hpp"

class Rotatable
{
 protected:
   double rotation; // an angle in degrees.
   Rotator2D rotator;
   Rotator2D reverseRotator;

 public:
   Rotatable();
   Rotatable(double rotation);

   /**
     returns the angle in degrees
   */
   double getRotation() const;

   double getRotationInRadians() const;

   /**
     @param rotation is the angle in degrees.
   */
   void setRotation(double rotation);
   /**
   Sets rotation based on the position of an object(position_x,position_z) and
   a point (x,z) so the object ends up facing (x,z)
   */
   void setRotation(double position_x,double position_z,double x,double z);
   void drawForDesign(int windowid,double x,double z,double size) const;

   virtual void getActiveXZPointsInRange(double position_x,
	   double position_z,double tipDistance,
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);
};

#endif
