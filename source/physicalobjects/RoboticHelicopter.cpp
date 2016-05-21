#include "../stdafx.h"
#include "RoboticHelicopter.hpp"
#include "../modelio/RRSMComplexObjectLoader.hpp"
#include "../sensors/VideoDevice.hpp"
#include "../app/Application.hpp"

#ifndef _UBUNTU_

#include <windows.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#define STATE_DEFAULT 0
#define STATE_FOLLOWING 1
#define STATE_LANDING 2
#define STATE_GO_TO_TARGET_POINT 3

using namespace std;

const char *droneModelFile="models/drone/drone.rrsm";

double RoboticHelicopter::maxSpeed=30;

bool RoboticHelicopter::setMaxSpeed(double maxSpeed)
{
   if (maxSpeed<1)
   {
     cerr << "Can't set helicopter-speed-max to something less than 1."<<endl;
	return false;
   }

   RoboticHelicopter::maxSpeed=maxSpeed;
   return true;
}

double RoboticHelicopter::getMaxSpeed()
{
  return RoboticHelicopter::maxSpeed;
}

RoboticHelicopter::RoboticHelicopter(std::string id):
  InteractiveObject(0,5,0,1000),obj(NULL)
	  ,visible(true),axisRotationDegrees(0),animated(false),target(NULL),id(id),
	  state(STATE_DEFAULT)
{
  targetPosition.set(0,0,0);
  init();
}

RoboticHelicopter::RoboticHelicopter(const RoboticHelicopter & helicopter):
  InteractiveObject(0,5,0,1000),obj(NULL)
	  ,visible(true),axisRotationDegrees(0),animated(false),target(NULL),id(id),
	  state(STATE_DEFAULT)
{
  init();

  copyAttributesFrom(&helicopter);
}

RoboticHelicopter::~RoboticHelicopter()
{
    if (obj!=NULL)
       delete obj;

}

void RoboticHelicopter::init()
{
//    cout << "RoboticHelicopter::init() called."<<endl;
  obj = createNewComplexObject();
  //    cout << "RoboticHelicopter::init() complete."<<endl;
}



void RoboticHelicopter::ticksPerformed(
			Environment * e,int milis)
{
 if (!animated) // don't change anything unless this is animated.
	 return;

 //cout << "helicopter is animated."<<endl;
double minVelocity=0;

 if (target!=NULL && (state==STATE_LANDING || state==STATE_FOLLOWING ))
 {
   double elevation = targetPosition.getY();

    // move toward target.
    targetPosition=target->getPosition();
    if (state==STATE_LANDING)
       targetPosition.setY(targetPosition.getY()+30);
    else // following
       targetPosition.setY(elevation);

    // if the target is very close, latch onto it.
    if (position.distanceFrom(targetPosition)<50 && state==STATE_LANDING)
    {
      target->join(this);
    }
    minVelocity = target->getVelocity().getMagnitude()*1.05;
 }

 // update velocity to approach targetPosition.
 {
    Vector3D displacement = targetPosition-position;
    displacement=displacement;
    if (displacement.getMagnitude()<12)
       velocity=displacement*15;
    if (displacement.getMagnitude()<25)
       velocity=displacement*5;
     else if (displacement.getMagnitude()<100)
     {
       velocity=displacement*2;
     }
	else
         velocity=displacement;

    if (displacement.getMagnitude()>5 && velocity.getMagnitude()<minVelocity)
    {
       velocity.makeUnitVector();
	  velocity=velocity*minVelocity;
    }

    if (velocity.getMagnitude()>RoboticHelicopter::maxSpeed)
    {
	     velocity.makeUnitVector();
	     velocity=velocity*RoboticHelicopter::maxSpeed;
    }

 }

 position.add(velocity*milis*0.001);

 if (position.getY()<10  && (targetPosition.getY()<10) &&
     state==STATE_LANDING)
 {
   position.setY(10);
   animated=false; // stop animation.  The helicopter landed.
 }

// cout << "Helicopter ticksPerformed."<<endl;

  // rotate the "lift" wrapper.
  TranslateObjectWrapper * wrapper = obj->getWrapperByName("lift");
  list<TranslateObjectWrapper *> wrappers;
  obj->getWrappersByName("lift",wrappers);

  for (list<TranslateObjectWrapper *>::iterator it=wrappers.begin();it!=wrappers.end();it++)
  {
      TranslateObjectWrapper * w = *it;

        w->setYRotation(w->getYRotation()+milis*5.0);
  }

  wrapper = obj->getWrapperByName("tailspinner");
  if (wrapper!=NULL)
  {
     wrapper->setXRotation(wrapper->getXRotation()+milis*1.0);
  }

  enforcePositionBoundaries();
}

std::string RoboticHelicopter::getName() const
{
	return std::string("RoboticHelicopter");
}

void RoboticHelicopter::setVisible(bool visible)
{
  this->visible = visible;
}

void RoboticHelicopter::draw(int windowid,const Vector3D &viewpoint)
{
  if (obj==NULL || (!visible))
	return;

 VideoDevice *cam = Application::getAllSensors().getVideoDeviceByWindowID(windowid);
  if (cam!=NULL)
  {
     if (cam->getMountingObject()==this)
        return;
     // do not show this helicopter if the current window is looking from it.
  }
  	glPushMatrix();
     glTranslated(getX(),getY(),getZ());

	glRotated(-axisRotationDegrees,axis.getX(),axis.getY(),axis.getZ());

     glRotated(rotation+180,0,1,0);

	obj->draw(windowid,viewpoint);

	glPopMatrix();
}

void RoboticHelicopter::drawForDesign(int windowid)
{
 // no representation for design view.
}

void RoboticHelicopter::readFrom(std::istream &in)
{
   InteractiveObject::readFrom(in);
   in >> id;
}

void RoboticHelicopter::writeTo(std::ostream &out) const
{
   InteractiveObject::writeTo(out);
   out<<" "<<id;
}



void RoboticHelicopter::copyAttributesFrom(const RoboticHelicopter *helicopter)
{
    if (helicopter==NULL)
    {
       cout << "Can't copy attributes from a null object."<<endl;
       return;
    }

  rotation = helicopter->rotation;
  position = helicopter->position;
  target=helicopter->target;
  targetPosition = helicopter->targetPosition;
  axisRotationDegrees = helicopter->axisRotationDegrees;
  id = helicopter->id;
}

bool RoboticHelicopter::isAnimated() const
{
  return animated;
}



Vector3D RoboticHelicopter::getVelocity() const
{
  return velocity;
}

void RoboticHelicopter::getAsLines(std::vector<LineSegment> &lines) const
{
  if (obj!=NULL)
     obj->getAsLines(lines);
}

ComplexObject * RoboticHelicopter::createNewComplexObject()
{
 RRSMComplexObjectLoader loader;

   return loader.loadFrom(droneModelFile);
}

void RoboticHelicopter::follow(PhysicalObject * target,double elevation)
{
  this->target=target;
  targetPosition.setY(elevation);
  state=STATE_FOLLOWING;
  animated=true; // have to move blades to hover.
}

void RoboticHelicopter::setTargetPosition(
	double x,double y, double z)
{
   targetPosition.set(x,y,z);
   animated=true; // have to move blades to hover.
   state=STATE_GO_TO_TARGET_POINT;
   target=NULL;
}

void RoboticHelicopter::landASAP()
{
   targetPosition.setY(0);
   targetPosition.setX(getX());
   targetPosition.setZ(getZ());
   target=NULL;
   animated=true;
   state=STATE_LANDING;
}

void RoboticHelicopter::landOn(PhysicalObject * obj)
{
  target=obj;
  state=STATE_LANDING;
}

void RoboticHelicopter::hoverAt(double altitude)
{
   targetPosition.setY(altitude);
   targetPosition.setX(getX());
   targetPosition.setZ(getZ());
  // setY(altitude);
   target=NULL;
   animated=true; // make sure the blades are turning.
   state=STATE_GO_TO_TARGET_POINT;
}
