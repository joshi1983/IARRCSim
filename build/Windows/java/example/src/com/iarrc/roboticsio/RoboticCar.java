package com.iarrc.roboticsio;

import java.util.LinkedList;

public class RoboticCar
{
 private IARRCGeneralInterfaceAdapter generalInterface;

   public RoboticCar(IARRCGeneralInterfaceAdapter generalInterface)
   {
      if (generalInterface==null)
	     throw new NullPointerException("general interface can't be null.");
		 
      this.generalInterface = generalInterface;
   }
 
  public LinkedList<Sensor> getSensors() throws Exception
  {
    return generalInterface.getSensors();
  }
 
  /**
   For steering control of the car.
  */
  public void setSteeringValue(double value) throws Exception
  {
    generalInterface.setSteeringValue(value);
  }
  
  /** for forward and reverse. */
  public void setSpeedValue(double value) throws Exception
  {
    generalInterface.setSpeedValue(value);
  }
}