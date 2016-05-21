#ifndef SENSOR_HEADER
#define SENSOR_HEADER

#include <string>

#include "../vectors/Vector3D.hpp"
#include "../physicalobjects/PhysicalObject.hpp"


class Sensor
{
 private:
   const std::string getPositionPropertyList(const std::string &name) const;



protected:
   std::string name;
   bool active;
   Vector3D position;
   // the physical object this sensor is mounted on
   PhysicalObject * mountingObject;

   const std::string formatPropertyAssignment(
	   const char*name,const char*val,const std::string &format) const;
   const std::string formatPropertyAssignment(
     const char*name,const double val,const std::string &format) const;


   static double POSITION_MIN_Y;

 public:
   Sensor(const std::string &name);

   bool isMovable() const;
   virtual bool isActive() const;
   virtual void setActive(bool active);
   const Vector3D & getPosition() const;

   virtual const std::string getType() const=0;
   const std::string &getName() const;

   void setMountingObject(PhysicalObject *po);
   PhysicalObject * getMountingObject();

   virtual bool setProperty(const std::string &propertyName,double value);
   virtual const std::string getProperty(const std::string &name) const;
   virtual const std::string getDeviceInfoPropertyList(const std::string &format) const=0;

   /**
   @param format can be "xml" or "txt" but
    maybe others depending on which device it is.
   */
   virtual const std::string getDeviceInfo(const std::string &format) const;


};


#endif
