#include "UltrasonicSensor.hpp"
#include "../io/HTTPParser.hpp"
#include "../app/Application.hpp"
#include "../vectors/Ray3D.hpp"

#include <iostream>

using namespace std;

double UltrasonicSensor::RANGE_MAX=600;
double UltrasonicSensor::RANGE_MIN=10;

UltrasonicSensor::UltrasonicSensor(const std::string &name): Sensor(name),
 range_min(RANGE_MIN),range_max(RANGE_MAX),
 direction(0,0,-1)
{

}

const std::string UltrasonicSensor::getType() const
{
  return "ultrasonic";
}


bool UltrasonicSensor::setGlobalRangeMax(double val)
{
  if (val<0)
  {
    cout << "Ultrasonic range finder can't detect negative depths."<<endl;
    return false;
  }

  RANGE_MAX=val;
  return true;
}

bool UltrasonicSensor::setGlobalRangeMin(double val)
{
  if (val<0)
  {
    cout << "Ultrasonic range finder can't detect negative depths."<<endl;
    return false;
  }
  RANGE_MIN=val;
  return true;
}

double UltrasonicSensor::getGlobalRangeMax()
{
	return RANGE_MAX;
}

double UltrasonicSensor::getGlobalRangeMin()
{
  return RANGE_MIN;
}

bool UltrasonicSensor::setProperty(const std::string &propertyName,double value)
{
  if (propertyName=="direction-x")
	  direction.setX(value);
  else if (propertyName=="direction-y")
	 direction.setY(value);
  else if (propertyName=="direction-z")
	 direction.setZ(value);
  else if (propertyName=="range-max")
     this->range_max=value;
  else if (propertyName=="range-min")
	  this->range_min=value;
  else if (propertyName=="depth")
  {
     // do nothing. just ignore it.
  }
  else
  {
	  return Sensor::setProperty(propertyName,value);
  }

  return true;
}

const std::string  UltrasonicSensor::getDeviceInfoPropertyList(const std::string &format) const
{
	 string result;

	  result+=formatPropertyAssignment("range-max",range_max,format);
	  result+=formatPropertyAssignment("range-min",range_min,format);
       result+=formatPropertyAssignment("direction-x",direction.getX(),format);
	  result+=formatPropertyAssignment("direction-y",direction.getY(),format);
	  result+=formatPropertyAssignment("direction-z",direction.getZ(),format);

	  if (format!="inifile")
   	     result+=formatPropertyAssignment("depth",getCurrentDepth(),format);

	  return result;
}

const std::string UltrasonicSensor::getProperty(const std::string &name) const
{
  if (name=="range-max")
	  return asString(range_max);
  else if (name=="range-min")
	  return asString(range_min);
  else if (name=="depth")
	  return asString(getCurrentDepth());
  else if (name=="direction-x")
	  return asString(direction.getX());
  else if (name=="direction-y")
	  return asString(direction.getY());
  else if (name=="direction-z")
	  return asString(direction.getZ());
  else
	  return Sensor::getProperty(name);
}

double UltrasonicSensor::getCurrentDepth() const
{
 Ray3D r(position,direction);
 double actualDepth= Application::getDepthAlongRay(r,range_min,range_max);

 if (actualDepth>range_max)
     return range_max;
 else if (actualDepth<range_min)
	 return range_min;
 else
     return actualDepth;
}

const Vector3D& UltrasonicSensor::getDirection() const
{
  return direction;
}
