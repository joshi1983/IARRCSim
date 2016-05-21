package com.iarrc.roboticsio;

public class DepthSensor extends Sensor
{
  // depth maximum usually a few hundred centimeters
 private double rangeMax; 

  // depth minimum usually a few centimeters
 private double rangeMin; 
 
  public DepthSensor(String name,IARRCGeneralInterfaceAdapter generalInterface) throws Exception
  {
    super(name,generalInterface);
  }
   
  /** Returns maximum depth in cm.
  */
  public double getMaxDepth()
  {
    return rangeMax;
  }
  
  public void setProperty(String propertyName,String value)
  {
    if (propertyName.equals("range-max"))
	   rangeMax = Double.parseDouble(value);
	else if (propertyName.equals("range-min"))
	    rangeMin = Double.parseDouble(value);
    else
	    super.setProperty(propertyName,value);
  }
   
}