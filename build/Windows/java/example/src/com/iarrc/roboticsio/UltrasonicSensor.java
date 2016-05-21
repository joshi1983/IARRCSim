package com.iarrc.roboticsio;

import com.iarrc.math.*;

public class UltrasonicSensor extends DepthSensor
{
 private static int DEPTH_CACHE_TIME_LIMIT = 500;
 private double depth;
 private Vector3D direction=new Vector3D();
 private long lastDepthUpdate;

  public UltrasonicSensor(String name,IARRCGeneralInterfaceAdapter generalInterface) throws Exception
  {
    super(name,generalInterface);
  }
  
  public double getDepth()
  {
    if (System.currentTimeMillis()-lastDepthUpdate>DEPTH_CACHE_TIME_LIMIT)
	{
	  try
	  {
	    updateDepth();
	  }
	  catch (Exception exc)
	  {
	     exc.printStackTrace();
	  }
	}
    return depth;
  }
  
  private void updateDepth() throws Exception
  {
    setDepth(generalInterface.getDepth(getName()));
  }
  
  private void setDepth(double newDepth)
  {
     this.depth = newDepth;
     lastDepthUpdate = System.currentTimeMillis();
  }
  
  public void setProperty(String propertyName,String value)
  {
    if (propertyName.equals("depth"))
	   setDepth(Double.parseDouble(value));
	else if (propertyName.startsWith("direction-"))
	   direction.setProperty(propertyName,value);
	else
	    super.setProperty(propertyName,value);
  }
}