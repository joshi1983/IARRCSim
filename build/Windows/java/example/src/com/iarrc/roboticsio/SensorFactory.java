package com.iarrc.roboticsio;

import org.w3c.dom.*;

public class SensorFactory
{

 public static Sensor createFrom(Element e,IARRCGeneralInterfaceAdapter generalInterface) throws Exception
 {
  Sensor result=null;
  String type = e.getAttribute("type");
  String sensorName = e.getAttribute("name");
  
  System.out.println("sensorName: "+sensorName);
   if (type.equals("video"))
      result = new VideoSensor(sensorName,generalInterface);
   else if (type.equals("ultrasonic"))
        result = new UltrasonicSensor(sensorName,generalInterface); 
   else if (type.equals("lidar"))
        result = new LidarSensor(sensorName,generalInterface); 
	else
        throw new IllegalArgumentException("invalid device type: '"+type
		+"' must be video, ultrasonic or lidar.");	
		
	// loop through setting properties of the sensor.
   NodeList props = e.getElementsByTagName("property");
   
    for (int i=0;i<props.getLength();i++)
	{
	    Element prop = (Element)props.item(i);
		String name = prop.getAttribute("name");
		String value = prop.getAttribute("value");
		
		result.setProperty(name,value);
	}
    
	return result;
 }
 
}