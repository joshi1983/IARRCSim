package com.iarrc.roboticsio;

import java.util.*;

/**
 An implementer of this can work as a proxy or a stub 
 to the actual simulation software or you may choose to implement 
 a class that interacts with real robotics hardware.
*/
public abstract class IARRCGeneralInterfaceAdapter
{
  /**
  Returns a list of loadable tracks.  
  Normally, this is a list of file names that can be loaded by the simulator.
  */
  public abstract LinkedList<String> getLoadableTracks();
  
  /**
  Returns a list of all sensors used by the robot.
  */
  public abstract LinkedList<Sensor> getSensors() throws Exception;
  
  /**
  Returns the sensor with the specified name.
  @throws Exception if no sensor has the specified name.
  */
  public Sensor getSensor(String name) throws Exception
  {
     LinkedList<Sensor> sensors = getSensors();
	 for (Sensor s: sensors)
	 {
	    if (s.getName().equals(name))
		   return s;
	 }
	 
	 throw new IllegalArgumentException("No sensor exists with the name '"+name+"'");
  }
  
  public abstract void setSteeringValue(double value) throws Exception;
  
  public abstract void setSpeedValue(double value) throws Exception;

  /**
  Makes a stub/proxy to represent the main car that is being controlled
  */
  public abstract RoboticCar createRoboticCar();
  
    /**
  Returns all depths for the specified Lidar sensor
  @throws Exception if there is any problem with the sensor 
   not existing or being something other than a Lidar type.
   Also, throws an exception if the server is not running.
  */
  public abstract double[] getLidarDepthData(String sensorName,int numDepths) throws Exception;
 
 /**
   Gets the current depth of the specified ultrasonic sensor
 */
  public abstract double getDepth(String sensorName) throws Exception;
  
  /**
  Returns a description of what this interface connects with.
  
  In the case of a connection to an IARRC simulator, 
  this returns the URL address of the simulator.
  
  In the case of a connection to the robotics hardware, 
  some other string should be returned to show that.
  */
  public abstract String getConnectionString();
  
  /**
  Gets the current frame from the specified video sensor
  */
  public abstract java.awt.Image getCurrentFrame(String videoSensorName) throws Exception;
	 
}