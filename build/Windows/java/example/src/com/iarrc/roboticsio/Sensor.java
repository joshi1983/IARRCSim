package com.iarrc.roboticsio;

import com.iarrc.math.Vector3D;

public class Sensor
{
 private String name;
 private Vector3D position = new Vector3D(); 
  // position of the sensor relative to a chassis origin.
 protected IARRCGeneralInterfaceAdapter generalInterface;
 
 
 public Sensor(String name,IARRCGeneralInterfaceAdapter generalInterface)
 {
   setName(name);
   this.generalInterface=generalInterface;
 }
 
 public void setName(String name)
 {
   this.name = name;
 }
 
 public String getName()
 {
   return name;
 }
 
 public void setProperty(String propertyName,String value)
 {
   if (propertyName.equals("name"))
      setName(value);
   else	if (propertyName.startsWith("position-"))
      position.setProperty(propertyName,value);
   else
       System.err.println("unrecognized property: '"+propertyName+"'");
 }

 
}