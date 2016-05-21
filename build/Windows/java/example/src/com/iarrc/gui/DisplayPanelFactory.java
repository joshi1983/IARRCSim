package com.iarrc.gui;

import com.iarrc.roboticsio.*;
import javax.swing.*;

public class DisplayPanelFactory
{
  public static JPanel createFrom(Sensor s)
  {
    if (s instanceof LidarSensor)
	   return new LidarDisplayPanel((LidarSensor)s);
	else if (s instanceof VideoSensor)
	   return new VideoDisplayPanel((VideoSensor)s);
	else if (s instanceof UltrasonicSensor)
	   return new UltrasonicDisplayPanel((UltrasonicSensor)s);
	else
	{
	   System.err.println("unsupported sensor type: "+s);
	   return null;
	}
  }

}