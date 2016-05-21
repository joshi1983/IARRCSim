package com.iarrc.gui;

import com.iarrc.config.*;
import com.iarrc.roboticsio.*;
import javax.swing.*;
import java.awt.*; // GridLayout...
import java.util.LinkedList;

public class ManualControlsFrame extends JFrame
{
 private RoboticCar car;
 
  public ManualControlsFrame(IARRCGeneralInterfaceAdapter generalInterface)
  {
    super("Manual Controls");
	
	car = generalInterface.createRoboticCar();
	try
	{
      LinkedList<Sensor> sensors = car.getSensors();
	
	setLayout(new GridLayout(sensors.size()+1,1));
	  
	  add(new ControlsPanel(generalInterface));
	  
	  for (Sensor s: sensors)
	  {
	  JPanel p = DisplayPanelFactory.createFrom(s);
		
		add(p);
	  }
	 
	}
	catch (java.net.ConnectException connectionProblem)
	{
	   connectionProblem.printStackTrace();
	   String msg = "Make sure the simulator is running at "+generalInterface.getConnectionString()
	   +".  "
	   +"The simulator must be running or this client has nothing to connect with.";
	   System.out.println(msg);
	   JOptionPane.showMessageDialog(this,msg);
	}
	catch (Exception exc)
	{
	   exc.printStackTrace();
	   System.out.println("Unable to get sensors.");
	}
	setSize(500,600);
	setVisible(true);
	
	new UpdateDisplaysThread(); // continually update the display
  }
  
  class UpdateDisplaysThread extends Thread
  {
    public UpdateDisplaysThread()
	{
	  start();
	}
	
	public void run()
	{
	  try
	  {
	    while (true)
	    {
	      repaint();
			
	  	  Thread.sleep(1000); // sleep for a second.
	    }
	  }
	  catch (Throwable t)
	  {
	     t.printStackTrace();
	  }
	}
  }
}