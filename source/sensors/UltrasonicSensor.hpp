#ifndef ULTRASONIC_SENSOR
#define ULTRASONIC_SENSOR

#include "Sensor.hpp"

class UltrasonicSensor: public Sensor
{
private:
	static double RANGE_MAX;
	static double RANGE_MIN;

	Vector3D direction;

	double range_min;
	double range_max;

 public:
  UltrasonicSensor(const std::string &name);

  const Vector3D& getDirection() const; 
  double getCurrentDepth() const;

  virtual const std::string getDeviceInfoPropertyList(const std::string &format) const;
  virtual const std::string getType() const;
  virtual const std::string getProperty(const std::string &name) const;
  virtual bool setProperty(const std::string &propertyName,double value);


  static bool setGlobalRangeMax(double val);
  static bool setGlobalRangeMin(double val);

  static double getGlobalRangeMax();
  static double getGlobalRangeMin();
};

#endif