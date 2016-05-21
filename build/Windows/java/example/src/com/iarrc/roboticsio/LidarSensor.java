package com.iarrc.roboticsio;

import com.iarrc.math.*;

public class LidarSensor extends DepthSensor
{
 /**
  depths data must be refreshed once every this many milliseconds.
  If depths are queried after the time expired, 
  the query will be blocked until depths are refreshed
 */
 private static final int DEPTHS_CACHE_TIME_LIMIT = 1000;
 private long lastDepthsUpdate;
 
 private double depths[];
 private double resolutionAngle; // in degrees
 
 private double rangeMax=Double.MAX_VALUE;
 
 // in degrees usually 90 - 360 degrees
 private double rangeScan; 

 // scan frequency measured in scans per second.
 private double scanFrequency;

 private Vector3D rotation=new Vector3D();

  public LidarSensor(String name,IARRCGeneralInterfaceAdapter generalInterface) throws Exception
  {
    super(name,generalInterface);
	depths=new double[0];
  }
  
  /**
   Returns scan range in degrees.
  */
  public double getRangeScan()
  {
    return rangeScan;
  }
  
  public double getMaxDepth()
  {
    return rangeMax;
  }

  public void updateDepths() throws Exception
  {
     depths = generalInterface.getLidarDepthData(getName(),getNumberOfDepths());
	 lastDepthsUpdate = System.currentTimeMillis();
  }
  
  /**
   Returns the number of depths in a single scan.
  */
  public int getNumberOfDepths()
  {
    return (int)(rangeScan/resolutionAngle);
  }
  
  /**
  Gets depths using cache, if the cache was recently updated.
  */
  public double[] getDepthsUsingCache()
  {
    if (depths==null || depths.length==0 || 
	  System.currentTimeMillis()-lastDepthsUpdate>DEPTHS_CACHE_TIME_LIMIT)
	{
	  try
	  {
	     updateDepths();
	  }
	  catch (Exception e)
	  {
	     e.printStackTrace();
	  }
	}
	 
    return depths;
  }
  
  public double[] getDepths()
  {
     return getDepthsUsingCache();
  }
  
  public double[] getDepthsNoCache()
  {
    try
	{
       updateDepths();
	}
	catch (Exception e)
	{
	   e.printStackTrace();
	}
	return depths;
  }
  
  public void setProperty(String propertyName,String value)
  {
    if (propertyName.equals("range-max"))
	   rangeMax = Double.parseDouble(value);
    else if (propertyName.equals("range-scan"))
	    rangeScan = Double.parseDouble(value);
	else if (propertyName.equals("resolution-angle"))
	    resolutionAngle = Double.parseDouble(value);
	else if (propertyName.equals("scan-frequency"))
	    scanFrequency = Double.parseDouble(value);
	else if (propertyName.startsWith("rotation-"))
	    rotation.setProperty(propertyName,value);
	else
        super.setProperty(propertyName,value);
  }
  
  
}