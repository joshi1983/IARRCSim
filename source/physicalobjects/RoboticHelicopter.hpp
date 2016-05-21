#ifndef ROBOTICHELICOPTER_HEADER
#define ROBOTICHELICOPTER_HEADER

class RoboticHelicopter;

#include "InteractiveObject.hpp"
#include "Rotatable.hpp"
#include "ComplexObject.hpp"


#include "../shapes/ShapeCollection.hpp"

class RoboticHelicopter: public InteractiveObject,public Rotatable
{
private:

	Vector3D targetPosition;

	// the part that shows to the user in video windows
	ComplexObject *obj;

	/**
	landing target or target to follow.
	*/
	PhysicalObject * target;

    /**
    Are we following, landing, or something else?
    */
    int state;


     bool visible;
	double axisRotationDegrees;

     bool animated;

	std::string id;

	static double maxSpeed;

 public:
	 RoboticHelicopter(std::string id);
	 RoboticHelicopter(const RoboticHelicopter & helicopter);
    virtual ~RoboticHelicopter();

	 void init();

	 std::string getID() const;

     void follow(PhysicalObject * target,double elevation=2000);

	 /**
	 Lands as soon as possible.
	 */
	 void landASAP();

	 /**
	 Lands on the specified object.
	 The object should be a RoboticCar.
	 */
      void landOn(PhysicalObject * obj);

	 /**
	 Hovers at the specified altitude/elevation
	 */
	 void hoverAt(double altitude);

	/*
	Used in detaching a helicopter from a car.

	*/
	void copyAttributesFrom(const RoboticHelicopter *helicopter);

	virtual Vector3D getVelocity() const;

	 bool isAnimated() const;
	 void setVisible(bool visible);


      virtual std::string getName() const;
	 virtual void draw(int windowid,const Vector3D &viewpoint);
	 void setTargetPosition(double x,double y, double z);
      virtual void ticksPerformed(Environment * e,int milis);

	virtual void drawForDesign(int windowid);
	virtual void readFrom(std::istream &in);
     virtual void writeTo(std::ostream &out) const;
	virtual void getAsLines(std::vector<LineSegment> &lines) const;

	static bool setMaxSpeed(double newMaxSpeed);
	static double getMaxSpeed();
	static ComplexObject * createNewComplexObject();
};


#endif
