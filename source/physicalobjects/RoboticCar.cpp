#include "../stdafx.h"
#include "RoboticCar.hpp"
#include "../robotcontrollers/PylonAvoider.hpp"


#include "../stdafx.h"

#ifndef _UBUNTU_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#endif

#include <fstream>
#include <cmath>
#include <algorithm>
#include "../modelio/RRSMComplexObjectLoader.hpp"
#include "../shapes/VerticalRectangle.hpp"
#include "../app/Application.hpp"
#include "../app/EnvironmentRemovePhysicalObjectJob.hpp"

#define MAX_RIGHT 25
#define MAX_LEFT -MAX_RIGHT
#define MAX_BODY_TILT 15
#define MIN_BODY_TILT -MAX_BODY_TILT

using namespace std;

double RoboticCar::suspensionSoftness = 1.0;

const char *carModelFile="models/traxxas/traxxas.rrsm";

double getBoundedSteeringAngle(double angle)
{
  if (angle<MAX_LEFT)
	  return MAX_LEFT;
  else if (angle>MAX_RIGHT)
	  return MAX_RIGHT;

  return angle;
}

void RoboticCar::setGlobalSuspensionSoftness(double val)
{
  suspensionSoftness=val;
}

double RoboticCar::getGlobalSuspensionSoftness()
{
  return suspensionSoftness;
}

RoboticCar::RoboticCar(const RoboticCar& car):
  InteractiveObject(car.getX(),car.getY(),car.getZ(),car.getMass()),
	  Rotatable(car.getRotation()),
	  driveForce(car.driveForce),steeringAngle(car.steeringAngle),
	  desiredSteeringAngle(car.desiredSteeringAngle),
	  controller(NULL),bodyTilt(car.getBodyTilt()),
	  controlledExternally(car.isControlledExternally()),
	  id(car.getID()),drone(car.getID()+"_drone"),obj(NULL)
{
  this->velocity=car.getVelocity();
}

RoboticCar::RoboticCar(const std::string & id):
 InteractiveObject(0,25,0,5000),Rotatable(180),driveForce(0),
 steeringAngle(0),desiredSteeringAngle(0),controller(NULL),
 bodyTilt(0), controlledExternally(true),id(id),drone(id+"_drone")
{
  obj = createNewComplexObject();

  /*
  ofstream of("car_json.js");

  of << obj->getAsLines(string("JSON"));

  of.close();
*/

  setRobotController(new PylonAvoider(this));
  initDrone();
}


RoboticCar::RoboticCar(const std::string & id,double x,double y,double z,double rotation,bool controlledExternally):
 InteractiveObject(x,y,z,5000),Rotatable(rotation),driveForce(0),
 steeringAngle(0),desiredSteeringAngle(0),obj(NULL),controller(NULL),
 bodyTilt(0), controlledExternally(controlledExternally),id(id),drone(id+"_drone")
{
  if (this->controlledExternally)
  {
     setRobotController(new PylonAvoider(this));
  }
  else
	  drone.setVisible(false);

  initDrone();
}

 void RoboticCar::initDrone()
 {
   drone.setY(29);
   drone.setZ(-5);
   drone.setRotation(180);
   releasedDrone=NULL;
 }

void RoboticCar::setControlledExternally(bool controlledExternally)
{
   if (this->controlledExternally==controlledExternally)
	 return; // do nothing.

   this->controlledExternally=controlledExternally;

   if (!controlledExternally)
   {
	  removeComplexObject();
       removeAutomaticRobotController();
	  drone.setVisible(false);
   }
   else
   {
        obj = createNewComplexObject();
	   setRobotController(new PylonAvoider(this));
	   drone.setVisible(true);
   }
}

 void RoboticCar::resetPositionAndSpeed()
 {
   position.set(0,0,0);
   driveForce=0;
   forwardSpeed=0;
   desiredSteeringAngle=0;
   steeringAngle=0;
 }

 void RoboticCar::stop()
 {
   driveForce=0;
   forwardSpeed=0;
   desiredSteeringAngle=0;
   steeringAngle=0;
 }

void RoboticCar::setRobotController(RobotController *controller)
{
   removeAutomaticRobotController();

   this->controller = controller;
}

bool RoboticCar::isMainCar() const
{
  if (obj==NULL || controlledExternally==false)
	  return true;
  else
	  return false;
}

void RoboticCar::removeComplexObject()
{
  if (obj==NULL)
	  return;

  delete obj;
  obj=NULL;
}

void RoboticCar::removeAutomaticRobotController()
{
  if (controller==NULL)
	  return;

  delete controller;
  controller=NULL;
}

void RoboticCar::transform(Ray3D &ray) const
{
	ray.rotateBy(rotation+180,0,1,0);
	ray.rotateBy(-axisRotationDegrees,axis.getX(),axis.getY(),axis.getZ());
	ray.translateBy(getX(),getY(),getZ());
}

void RoboticCar::transform(Vector3D &v) const
{
	v.rotateBy(rotation+180,0,1,0);
	v.rotateBy(-axisRotationDegrees,axis.getX(),axis.getY(),axis.getZ());
	v.add(getX(),getY(),getZ());
}

void RoboticCar::transformDirection(Vector3D &v) const
{
	v.rotateBy(-bodyTilt,0,0,1);
	v.rotateBy(Rotatable::getRotation()+180,0,1,0);
	v.rotateBy(-axisRotationDegrees,axis.getX(),axis.getY(),axis.getZ());
}

void RoboticCar::draw(int windowid,const Vector3D &viewpoint)
{
	if (obj==NULL || (windowid==Application::getMainCamera()->getWindowID() && this==Application::getMainCar()))
		return;

 // draw a cube.
	glPushMatrix();
     glTranslated(getX(),getY(),getZ());

	glRotated(-axisRotationDegrees,axis.getX(),axis.getY(),axis.getZ());

     glRotated(rotation+180,0,1,0);

	obj->draw(windowid,viewpoint-position);

    droneAttachMutex.enterCriticalSection();
	if (releasedDrone==NULL)
	{
	   glRotated(-bodyTilt,0,0,1);

	   drone.draw(windowid,viewpoint);

	}
	droneAttachMutex.leaveCriticalSection();

	glPopMatrix();
}

void RoboticCar::copyAttributesFrom(const RoboticCar *car)
{
  if (car==NULL)
  {
	cout << "RoboticCar can not copyAttributesFrom a NULL car."
		<<endl;
	return;
  }

  this->position=car->position;
  setDesiredSteeringAngle(car->desiredSteeringAngle);
  Rotatable::setRotation(car->getRotation());
}

double RoboticCar::getUIRadius() const
{
  return 30*5;
}

void RoboticCar::drawForDesign(int windowid)
{
int size=30;

  glPushMatrix();

  glTranslated(getX(),getZ(),0);
  glRotated(-rotation,0,0,1);

  if (isMainCar())
	  glColor3d(0,1,1);
  else
	  glColor3d(0,0,1);

  glBegin(GL_QUADS);

    glVertex2d(-size,-size);
    glVertex2d(size,-size);
    glVertex2d(size,size);
    glVertex2d(-size,size);

  glEnd();
  glPopMatrix();

  Rotatable::drawForDesign(windowid,getX(),getZ(),getUIRadius());
}

void RoboticCar::setDesiredSteeringAngle(double angle)
{
  desiredSteeringAngle=getBoundedSteeringAngle(angle);
}

double RoboticCar::getSteeringAngle() const
{
  return steeringAngle;
}

double RoboticCar::getDrivingForce() const
{
  return driveForce;
}

void RoboticCar::setDrivingForce(double force)
{
  this->driveForce=force;
}

double RoboticCar::getDesiredSteeringAngle() const
{
 return desiredSteeringAngle;
}

void RoboticCar::ticksPerformed(Environment * e,int milis)
{
	drone.ticksPerformed(e,milis);
  if (controller!=NULL)
  {
     controller->update(e);
  }

 // simulate the passage of time for the whole car.
 // move the car forward a certain amount based on current velocity.
 position.add(velocity*milis*0.001);

 forwardSpeed=driveForce*10;

 double r=forwardSpeed;
 double dx=r*sin(rotation*PI/180),dz=r*cos(rotation*PI/180);

 // adjust velocity to approach the one determined by driveForce.
 velocity.set(dx,0,dz);

 if (obj!=NULL)
 {
	 const char *wheelNames[] = {"front_L","front_R","rear_R","rear_L"};
	 for (int i=0;i<4;i++)
	 {
        TranslateObjectWrapper * wrapper = obj->getWrapperByName(wheelNames[i]);
	   if (wrapper!=NULL)
	   {
	     wrapper->setXRotation(wrapper->getXRotation()-r*milis*0.005);

		if (wheelNames[i][0]=='f') // a front wheel
		{
		  wrapper->setYRotation(steeringAngle);
		}
	   }
	 }
	 TranslateObjectWrapper * wrapper = obj->getWrapperByName("body");
	 wrapper->setZRotation(-bodyTilt);



 }

// simulate time in servo motors:
// adjust steering, if desired steering angle is different from actual.

  steeringAngle=getBoundedSteeringAngle(desiredSteeringAngle);
 // cout << "RoboticCar::tickPerformed, steeringangle = "<<steeringAngle<<endl;
  double bodyTilt2=suspensionSoftness*steeringAngle*abs(forwardSpeed)*0.001;

  if (bodyTilt2>MAX_BODY_TILT)
     bodyTilt2=MAX_BODY_TILT;
  else if (bodyTilt2<MIN_BODY_TILT)
     bodyTilt2=MIN_BODY_TILT;

  if (abs(bodyTilt-bodyTilt2)>0.0001)
  {
    double max_change=milis*0.05;

     if (bodyTilt>bodyTilt2)
	   bodyTilt=max(bodyTilt-max_change,bodyTilt2);
	else
         bodyTilt=min(bodyTilt+max_change,bodyTilt2);
  }

 // steeringAngle=desiredSteeringAngle;
  rotation+=(steeringAngle)*forwardSpeed*milis*0.00002;
  setRotation(rotation);

// simulate time in drive motors:
// adjust drive speed based on the net force on the car.
// This includes driveForce and angles the car is on.

  Vector2D p(position.getX(),position.getZ());

    position.setY(e->getElevationForXZPoint(p));

    InteractiveObject::ticksPerformed(e,milis);

    enforcePositionBoundaries();
}

double RoboticCar::getBodyTilt() const
{
  return bodyTilt;
}

std::string RoboticCar::getName() const
{
	return std::string("RoboticCar");
}

double RoboticCar::getCurrentDirection() const
{
  return rotation;
}

double RoboticCar::getSignedSpeed() const
{
  // sign can be determined by the motion vector and rotation.

   if (forwardSpeed<0)
	   return -getSpeed();
   else
        return getSpeed();
}

void RoboticCar::getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
	Rotatable::getActiveXZPointsInRange(getX(),getZ(),getUIRadius(),x,z,maxDistance,results);
	PhysicalObject::getActiveXZPointsInRange(x,z,maxDistance,results);
}

void RoboticCar::writeTo(std::ostream &out) const
{
  InteractiveObject::writeTo(out);

  out <<" "<< Rotatable::getRotation()<<" ";

  if (controlledExternally==true)
	  out<<"true";
  else
	  out<<"false";

  out << " "<<id;
}

void RoboticCar::readFrom(std::istream &in)
{
   InteractiveObject::readFrom(in);

   double rot;
   in >> rot;
   Rotatable::setRotation(rot);
   string temp;
   in >> temp;

   if (temp=="true")
      setControlledExternally(true);
   else
   {
	   if (temp=="false")
	     setControlledExternally(false);
	   else
	   {
	      cout <<
			 "Invalid value for the roboticcar being externally controlled.  "
			 <<"It should be true or false."<<endl;
	   }
   }
   in >> id;
}

bool RoboticCar::isControlledExternally() const
{
  return controlledExternally;
}

void RoboticCar::addShapeRepresentation(ShapeCollection &shapes) const
{
  /*
   Do not represent the main car.
   We want to assume the depth sensors on the main car
   are not obstructed by the main car.
  */
  if (isMainCar())
	return;

 double width=35;
 double length=55;

  // add 4 VerticalRectangle instances to shapes.
 Vector3D pos=Vector3D(-width/2,getY(),0);
  rotator.rotateXZ(pos);
  pos.add(position);

 double height = 25;
 VerticalRectangle *r=new VerticalRectangle(pos,length,height,
	 Rotatable::getRotation());

   shapes.add(r);

   // represent other side.
  pos.set(width/2,getY(),0);
  rotator.rotateXZ(pos);
  pos.add(position);
  r=new VerticalRectangle(pos,length,height,
	 Rotatable::getRotation());

   shapes.add(r);

  // represent front and back.
  pos.set(0,getY(),-length/2);
  rotator.rotateXZ(pos);
  pos.add(position);
  r=new VerticalRectangle(pos,width,height,
	 Rotatable::getRotation()+90);

  shapes.add(r);

  pos.set(0,getY(),length/2);
  rotator.rotateXZ(pos);
  pos.add(position);
  r=new VerticalRectangle(pos,width,height,
	 Rotatable::getRotation()+90);

  shapes.add(r);
}

void RoboticCar::releaseDrone()
{
    if (obj==NULL)
      obj = createNewComplexObject();

  if (releasedDrone!=NULL)
	return; // nothing to do.  already detached.

 droneAttachMutex.enterCriticalSection();

  releasedDrone = new RoboticHelicopter(drone);

  if (releasedDrone==NULL)
  {
     cout << "Unable to create new drone."<<endl;
     droneAttachMutex.leaveCriticalSection();
	 return;
  }
 // update position of the released drone relative to this car.

  releasedDrone->setY(getY()+30);
  releasedDrone->setX(getX());
  releasedDrone->setZ(getZ());

  Environment &e=Application::getMainEnvironment();

   e.add(releasedDrone);

   Application::getAllSensors().switchMountingObject(&drone,releasedDrone);
   Application::getAllSensors().activateSensorsMountedOn(releasedDrone);
   droneAttachMutex.leaveCriticalSection();
}

void RoboticCar::reattachDrone()
{
	if (releasedDrone==NULL)
		return; // do nothing.  The drone was already reattached.

droneAttachMutex.enterCriticalSection();
    Application::getAllSensors().switchMountingObject(releasedDrone,&drone);
    Application::getAllSensors().deactivateSensorsMountedOn(&drone);

	Environment &e=Application::getMainEnvironment();

	drone.setVisible(true);

    //cout << "About to remove releasedDrone."<<endl;
	Application::SubmitJobForMainThread(new EnvironmentRemovePhysicalObjectJob(&e,releasedDrone,true));
    //cout << "Done removing releasedDrone."<<endl;

	releasedDrone=NULL;

	//cout << "releasedDrone set to NULL."<<endl;
	droneAttachMutex.leaveCriticalSection();

}

void RoboticCar::hoverDroneAt(double elevation)
{
  if (elevation<5)
  {
     landDroneASAP();
	return;
  }

  if (releasedDrone==NULL)
  {
	releaseDrone();
	droneAttachMutex.enterCriticalSection();
	if (releasedDrone!=NULL)
	{
	 releasedDrone->setX(getX());
     releasedDrone->setZ(getZ());
     releasedDrone->setY(getY());
    }
    droneAttachMutex.leaveCriticalSection();
  }
  droneAttachMutex.enterCriticalSection();
  if (releasedDrone!=NULL)
     releasedDrone->hoverAt(elevation);
  droneAttachMutex.leaveCriticalSection();
}

void RoboticCar::landDroneOnCar()
{
  droneAttachMutex.enterCriticalSection();

  if (releasedDrone!=NULL)
  {
	releasedDrone->landOn(this);
  }
  droneAttachMutex.leaveCriticalSection();
}

void RoboticCar::landDroneASAP()
{
    droneAttachMutex.enterCriticalSection();
  if (releasedDrone==NULL)
  {
    droneAttachMutex.leaveCriticalSection();
	return;
  }

  releasedDrone->landASAP();
  droneAttachMutex.leaveCriticalSection();
}

const std::string& RoboticCar::getID() const
{
	  return id;
}

RoboticHelicopter * RoboticCar::getReleasedDrone()
{
  return releasedDrone;
}

void RoboticCar::join(PhysicalObject * obj)
{
 // assuming obj to be the releasedDrone.

  reattachDrone();
}

Vector3D RoboticCar::getVelocity() const
{
   return velocity;
}

bool RoboticCar::setProperty(const std::string &propertyName,double value)
{
     if (PhysicalObject::setProperty(propertyName,value))
		return true;
	if (propertyName=="rotation")
	{
		Rotatable::setRotation(value);
		return true;
	}
	return false;
}

void RoboticCar::getAsLines(std::vector<LineSegment> &lines) const
{
  if (obj!=NULL)
     obj->getAsLines(lines);
}

ComplexObject * RoboticCar::createNewComplexObject()
{
 RRSMComplexObjectLoader loader;

   return loader.loadFrom(carModelFile);
}

RoboticHelicopter * RoboticCar::getCurrentDrone()
{
    RoboticHelicopter * result=NULL;
    droneAttachMutex.enterCriticalSection();
    if (releasedDrone!=NULL)
        result=releasedDrone;
    else
        result=&drone;
    droneAttachMutex.leaveCriticalSection();

    return result;
}

