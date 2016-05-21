#include "Lidar.hpp"
#include "../io/HTTPParser.hpp"
#include "../stdafx.h"
#include "../app/Application.hpp"
#include "../vectors/Rotator3D.hpp"

#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>

using namespace std;

double Lidar::RANGE_MAX=400;
double Lidar::RANGE_MIN=10;
double Lidar::RESOLUTION_ANGLE=0.36;
double Lidar::RANGE_SCAN=270;
double Lidar::SCAN_FREQUENCY=10;

bool Lidar::setGlobalLidarScanFrequency(double frequencyHz)
{
  if (frequencyHz<0.001)
  {
    cout << "Lidar can't have frequency less than 0.001 Hz."<<endl;
    return false;
  }
   SCAN_FREQUENCY=frequencyHz;
   return true;
}

bool Lidar::setGlobalRangeMax(double val)
{
 if (val<1)
 {
   cout << "Can't set LIDAR range-max to less than 1 centimeter."<<endl;
   return false;
 }
 RANGE_MAX=val;
 return true;
}

bool Lidar::setGlobalRangeMin(double val)
{
 if (val>RANGE_MAX)
 {
   cout << "Can't set LIDAR range-min to less than range-max, "<<RANGE_MAX
	   <<"cm."<<endl;
   return false;
 }
 RANGE_MIN=val;
 return true;
}

bool Lidar::setGlobalResolutionAngleDegrees(double val)
{
 if (val<0.01)
 {
   cout << "Can't set LIDAR resolution-angle to less than 0.01 degrees."<<endl;
   cout << "An attempt was made to initialize to "<<val<<" degrees."<<endl;
   return false;
 }
  RESOLUTION_ANGLE=val;
  return true;
}

bool Lidar::setGlobalRangeScan(double val)
{
 if (val<0.01)
 {
   cout << "Can't set LIDAR range-scan to less than 0.01 degrees."<<endl;
   return false;
 }
  RANGE_SCAN=val;
  return true;
}

double Lidar::getGlobalRangeMax()
{
  return RANGE_MAX;
}
double Lidar::getGlobalRangeMin()
{
  return RANGE_MIN;
}
double Lidar::getGlobalResolutionAngleDegrees()
{
  return RESOLUTION_ANGLE;
}
double Lidar::getGlobalRangeScan()
{
  return RANGE_SCAN;
}
double Lidar::getGlobalLidarScanFrequency()
{
  return SCAN_FREQUENCY;
}


Lidar::Lidar(const std::string &name): Sensor(name)
,range_max(RANGE_MAX),range_min(RANGE_MIN),resolution_angle(RESOLUTION_ANGLE),
scan_frequency(SCAN_FREQUENCY),range_scan(RANGE_SCAN),
rotation(0,0,0)
{

}

const std::string Lidar::getType() const
{
  return "lidar";
}

const std::string Lidar::getDeviceInfoPropertyList(const std::string &format) const
{
	 ostringstream result;

	 result<<Sensor::formatPropertyAssignment("range-max",
		 this->range_max,format);

	  result<<Sensor::formatPropertyAssignment("range-min",
		  this->range_min,format);

	  result<<formatPropertyAssignment("resolution-angle",resolution_angle,format);
	  result<<formatPropertyAssignment("range-scan",range_scan,format);
	  result<<formatPropertyAssignment("scan-frequency",scan_frequency,format);
	  result<<formatPropertyAssignment("rotation-x",rotation.getX(),format);
	  result<<formatPropertyAssignment("rotation-y",rotation.getY(),format);
	  result<<formatPropertyAssignment("rotation-z",rotation.getZ(),format);

	  return result.str();

}

const std::string Lidar::getProperty(const std::string &name) const
{
  if (name=="range-max")
	  return asString(range_max);
  else  if (name=="range-min")
       return asString(range_min);
  else  if (name=="resolution-angle")
       return asString(resolution_angle);
  else  if (name=="range-scan")
       return asString(range_scan);
  else  if (name=="scan-frequency")
       return asString(scan_frequency);
  else if (name=="rotation-x")
	  return asString(rotation.getX());
  else if (name=="rotation-y")
	  return asString(rotation.getY());
  else if (name=="rotation-z")
	  return asString(rotation.getZ());
  else
	  return Sensor::getProperty(name);
}

void Lidar::writeDepthsTo(std::ostream &out,int numRays,std::string format) const
{
 double *results=(double*)malloc(sizeof(double)*numRays);

   Vector3D axis1(-1,0,0);
   Vector3D axis2(0,0,-1);

   // rotate the rays using x,y,z components from the rotation vector.
   Rotator3D rX(rotation.getX()*PI/180,Vector3D(1,0,0));
   Rotator3D rY(rotation.getY()*PI/180,Vector3D(0,1,0));
   Rotator3D rZ(rotation.getZ()*PI/180,Vector3D(0,0,1));

   rZ.rotate(axis1);
   rX.rotate(axis1);
   rY.rotate(axis1);

   rZ.rotate(axis2);
   rX.rotate(axis2);
   rY.rotate(axis2);

   Application::getDepthsOnPlane(position,axis1,axis2
	,range_min,range_max,range_scan
	,numRays,results);

  if (format=="xml")
     out << "<sensordata>";

  for (int i=0;i<numRays;i++)
  {
  	if (format=="xml")
	{
		out << "<depth value=\""<<results[i]<<"\" />"<<endl;
	}
	else
	{
		out << results[i]<<endl;
	}
  }

  if (format=="xml")
     out << "</sensordata>";

  free(results);
}

int Lidar::getNumberOfPoints() const
{
  int numRays=(int)(range_scan/resolution_angle);
   return numRays;
}

void Lidar::writeDepthsTo(std::ostream &out,std::string format) const
{
 writeDepthsTo(out,getNumberOfPoints(),format);
}

bool Lidar::setProperty(const std::string &propertyName,double value)
{
  if (propertyName=="range-max")
     this->range_max=value;
  else if (propertyName=="range-min")
	  this->range_min=value;
  else if (propertyName=="resolution-angle")
	  this->resolution_angle=value;
  else if (propertyName=="range-scan")
	  this->range_scan=value;
  else if (propertyName=="scan-frequency")
	  this->scan_frequency=value;
  else if (propertyName=="rotation-x")
	  this->rotation.setX(value);
  else if (propertyName=="rotation-y")
	  this->rotation.setY(value);
  else if (propertyName=="rotation-z")
	  this->rotation.setZ(value);
  else
	  return Sensor::setProperty(propertyName,value);

  return true;
}
