#ifndef LIDAR_HEADER
#define LIDAR_HEADER

#include "Sensor.hpp"
#include <iostream>
#include <string>

class Lidar: public Sensor
{
 private:
   static double RANGE_MAX;
   static double RANGE_MIN;
   static double RESOLUTION_ANGLE;
   static double RANGE_SCAN; // degrees
   static double SCAN_FREQUENCY;

   Vector3D rotation;

     double range_max;
	double range_min;
	double resolution_angle;
	double range_scan;
	double scan_frequency;


 public:
   Lidar(const std::string &name);
   virtual const std::string getDeviceInfoPropertyList(
	   const std::string &format) const;

   virtual const std::string getType() const;
   virtual const std::string getProperty(const std::string &name) const;
   virtual bool setProperty(const std::string &propertyName,double value);

   void writeDepthsTo(std::ostream &out,std::string format) const;
   void writeDepthsTo(std::ostream &out,int numDepths,std::string format) const;

   int getNumberOfPoints() const;

   static bool setGlobalRangeMax(double val);
   static bool setGlobalRangeMin(double val);
   static bool setGlobalResolutionAngleDegrees(double val);
   static bool setGlobalRangeScan(double val);
   static bool setGlobalLidarScanFrequency(double frequencyHz);

   static double getGlobalRangeMax();
   static double getGlobalRangeMin();
   static double getGlobalResolutionAngleDegrees();
   static double getGlobalRangeScan();
   static double getGlobalLidarScanFrequency();

};

#endif
