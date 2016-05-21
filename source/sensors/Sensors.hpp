#ifndef SENSORS_HEADER
#define SENSORS_HEADER

#include <list>
#include <string>
#include "Sensor.hpp"
#include "VideoDevice.hpp"
#include "../threads/Mutex.hpp"
#include "../physicalobjects/PhysicalObject.hpp"

/**
 A container for Sensor objects.
A Mutex is used to ensure mutual exclusion when reading or writing with the list in multithreaded applications.
*/
class Sensors
{
 private:
  std::list<Sensor*> sensors;
  Mutex m;

 public:
   virtual ~Sensors();

   /**
     Returns a copy of a list of sensors.
     Removing or adding with the returned copy will not affect this collection of sensors.
     Avoid deleting or destroying any instances of Sensor.
   */
   std::list<Sensor*> getListCopy();

   /**
    Tries to add the specified sensor to this container.
    Returns true if and only if the specified sensor was added to this container.
    Nothing is added if the sensor name is already used by another sensor.
   */
   bool add(Sensor * sensor);

   /**
    Removes the sensor with the specified name.
   */
   void remove(const std::string &name);

   Sensor * getByName(const char * name);

   /**
    Returns a unique name of a sensor that uses the specified prefix.
   */
   std::string getNewSensorName(const std::string &prefix);

  #ifndef _USE_VISUAL_STUDIO_
  VideoDevice * getVideoDeviceByWindowID(int windowid);
  #endif

  /**
   Returns a document describing all sensors.
  */
  std::string getDocument(const std::string &format);

  /**
   Removes all sensors except ones occupying windows.
  */
  void clearNonEssentialSensors();

  /**
   Writes all the sensor data to a file using an ini-like format
  */
  void saveToINI(const char *filename);

  std::vector<int> getWindowIDs();


  void switchMountingObject(PhysicalObject *fromObj,PhysicalObject *toObj);

  void repaintAllWindows();

  void deactivateSensorsMountedOn(PhysicalObject * po);
  void activateSensorsMountedOn(PhysicalObject * po);
};


#endif
