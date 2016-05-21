#ifndef ROBOTICCAR_HEADER
#define ROBOTICCAR_HEADER

class RoboticCar;
class RoboticHelicopter;

#include "InteractiveObject.hpp"
#include "Rotatable.hpp"
#include "ComplexObject.hpp"
#include "RoboticHelicopter.hpp"

#include "../robotcontrollers/RobotController.hpp"
#include "../shapes/ShapeCollection.hpp"
#include "../threads/Mutex.hpp"

class RoboticCar: public InteractiveObject,public Rotatable
{
private:
     static double suspensionSoftness;
     Mutex droneAttachMutex;
     RoboticHelicopter drone;

     	std::string id;

	double desiredSteeringAngle;
	 // angle desired based on current duty cycle received by servo motors.

     double steeringAngle;
	// angle of the steering wheels.
     // different from rotation of the car.
     // steeringAngle determines how sharply and in what direction
	// the car is currently steering.

	double driveForce;
	// a force applied in a forward direction, if the value is positive.
	// assumed to be measured in Newtons

     double forwardSpeed;
	// measured in centimetres/second

     double bodyTilt;
	// a tilt usually caused by a high speed turn and weak suspension

     RobotController *controller;

     ComplexObject *obj;

	/**
	false for cars controlled by the user or by custom robot controllers.

	true when built-in automatic driving controllers are used.  For example, PylonAvoider.
	*/
	bool controlledExternally;

	/**
	NULL when the drone is sitting locked on top of the car.
	Not NULL when this car released the helicopter to fly elsewhere.
	*/
     RoboticHelicopter *releasedDrone;



	void initDrone();

public:
	RoboticCar(const RoboticCar& car);
	RoboticCar(const std::string & id);
	RoboticCar(const std::string & id,double x,double y,double z,double rotation,bool controlledExternally=true);

/**
 Returns the address of the currently used drone.
   If the drone is separated from the car, releasedDrone is returned.  Otherwise, &drone is returned.
 */
     RoboticHelicopter * getCurrentDrone();

/**
 Returns the released drone.  If this car has its drone attached, NULL is returned.
*/
     RoboticHelicopter * getReleasedDrone();
	bool isControlledExternally() const;

	/*
	Used in replacing a loaded car with position
	and other properties from the file.
	*/
	void copyAttributesFrom(const RoboticCar *car);

	void resetPositionAndSpeed();

	/** brings speed and drive force to 0 */
	void stop();

	virtual void draw(int windowid,const Vector3D &viewpoint);
	virtual void drawForDesign(int windowid);
	virtual void readFrom(std::istream &in);
     virtual void writeTo(std::ostream &out) const;

	virtual void join(PhysicalObject * obj);

     virtual Vector3D getVelocity() const;

	/**
	 Sends drone to the specified elevation.

	*/
	void hoverDroneAt(double elevation);

	/**
	Instantly removes the free flying drone from the
	environment and reveals the one riding on the car.

	This is to be called only after the drone has had time
	to fly back to landing position on the car.
	Otherwise, the animation will look fake.
	*/
	void reattachDrone();

	/**
	Releases the drone.
	This is to be done before hovering or any other commands issued to the drone.
	*/
	void releaseDrone();


     void landDroneOnCar();

     void landDroneASAP();

	const std::string &getID() const;

	/**
	Transforms the specified ray from the same coordinate
	system as the tilting, rotating main body of this car
	into the one used by the larger environment around it.

	This transformation typically includes rotations and a translation.
	*/
     void transform(Ray3D &ray) const;
     void transform(Vector3D &v) const;
	void transformDirection(Vector3D &v) const;

	/**
	Removes the complex object used to represent this robot
	*/
	void removeComplexObject();
	void removeAutomaticRobotController();

	void setControlledExternally(bool controlledExternally);

	double getDesiredSteeringAngle() const;
	double getSteeringAngle() const; // relative to the car's orientation
	double getDrivingForce() const;
	double getCurrentDirection() const;
	double getSignedSpeed() const;
	double getBodyTilt() const; // returns in degrees
	void setDrivingForce(double force);
	virtual void ticksPerformed(Environment * e,int milis);

	virtual bool setProperty(const std::string &propertyName,double value);

	/**
	Checks if this is the main car
	*/
     bool isMainCar() const;

	/**
	 Angle in degrees
	*/
	void setDesiredSteeringAngle(double angle);
	void setRobotController(RobotController *controller);
	virtual std::string getName() const;
	virtual double getUIRadius() const;
	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);
     virtual void addShapeRepresentation(ShapeCollection &shapes) const;
	virtual void getAsLines(std::vector<LineSegment> &lines) const;

	static ComplexObject * createNewComplexObject();
	static void setGlobalSuspensionSoftness(double val);
	static double getGlobalSuspensionSoftness();
};

#endif
