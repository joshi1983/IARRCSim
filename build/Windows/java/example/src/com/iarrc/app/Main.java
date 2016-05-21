package com.iarrc.app;

import com.iarrc.config.ConfigLoader;
import com.iarrc.config.Config;
import com.iarrc.roboticsio.*;
import com.iarrc.gui.*;
import java.io.*;
import javax.swing.JFrame;

public class Main
{
 private static final String CONFIG_FILENAME="config"+File.separatorChar+"config.ini";

  
 
/**
Builds a user interface for manual control of the car in the simulator
*/ 
  public static void main(String a[]) throws IOException
  {
     Config config = null;
	 
	 try
	 {
	  config = ConfigLoader.loadFrom(new File(CONFIG_FILENAME));
	 }
	 catch (IOException ioe)
	 {
	     ioe.printStackTrace();
		 System.err.println("Ensure that the configuration file is at "+CONFIG_FILENAME);
	 }
	  IARRCSimulator sim = new IARRCSimulator(config.getDocServerAddress());
	  if (!sim.isSimulatorRunning())
	  {
	     sim.runSimulator(config.getSimulatorInstallDirectory());
	  }
	  
	  IARRCGeneralInterfaceAdapter generalInterface = sim;
	  
	  ManualControlsFrame manualControls = new ManualControlsFrame(generalInterface);
	  manualControls.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
  }

}