package gui;

import mapping.ProcessingCentre;
import mapping.ObstacleMap;
import mapping.*;
import com.iarrc.roboticsio.*;
import com.iarrc.config.*;
import javax.swing.*;
import java.io.File;
import java.io.IOException;
import java.awt.BorderLayout; 

public class MapDisplayFrame extends JFrame
{
 private static final String CONFIG_FILENAME="config"+File.separatorChar+"config.ini";
 
 private MapDisplayPanel display;
 private ObstacleMap map;
 private IARRCGeneralInterfaceAdapter carinterface;
 private boolean autonomousRunning=false;
 private TestControlPanel testControlPanel;
 private VideoControlPanel videoControls;
 private boolean useVideo=false;
 
  public MapDisplayFrame()
  {
     super("Map Display");
	 setJMenuBar(new MapDisplayFrameMenuBar(this));
	 
	 initCarInterface();
	 
     map = new ObstacleMap();
     display = new MapDisplayPanel(map);
	 testControlPanel = new TestControlPanel(this);
	 videoControls = new VideoControlPanel(this,carinterface,"video1");
	 
	 setLayout(new BorderLayout());
     add(display,BorderLayout.CENTER);
	 add(testControlPanel,BorderLayout.EAST);
	 
	 if (useVideo)
	    add(videoControls,BorderLayout.WEST);
	 
     setSize(500,500);
     setVisible(true);
	 
	 new ProcessingThread().start();
  }
  
  /**
  Returns true if and only if the car is under autonomous control
  */
  public boolean isRunning()
  {
    return autonomousRunning;
  }
  
  public void useAutonomousControl()
  {
     autonomousRunning=true;
  }
  
  /**
   Stops the car and its autonomous control system
  */
  public void stopCar()
  {
    try
	{
        carinterface.setSpeedValue(0); // stop the car.
		map.allStop();
	 }
	 catch (Exception e)
	 {
	    e.printStackTrace();
	 }
	 autonomousRunning=false;
  }
  
  /**
  Clears the map of all obstacles and resets the current lidar position and orientation
  */
  public void clearMapAndStopAll()
  {
       try
	   {
         carinterface.setSpeedValue(0); // stop the car.
	   }
	   catch (Exception e)
	   {
	      e.printStackTrace();
	   }
     map.clear();
	 stopCar();
  }
  
  private void initCarInterface()
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

	   carinterface = sim;
  }

  private class ProcessingThread extends Thread
  {

    public void run()
    {
	  try
	  {
	  LidarSensor lidar = (LidarSensor)carinterface.getSensor("lidar1");
	  long time_ms = System.currentTimeMillis();
	  
	    ProcessingCentre.processInit(carinterface,lidar,map);
		CarDriver driver = new PylonMappingCarDriver();
		     // create a path planner/dr specialized for exploring a track to map it out.
		
        while (true)
	    {
	      try
		  {
	        ProcessingCentre.process(carinterface,lidar,map,time_ms);
			
			if (isRunning()) // if using autonomous control
			{
			  DriveSettings drivingsettings = driver.getNewDriveSettingsFrom(map);
			
			    carinterface.setSpeedValue(drivingsettings.getSpeedValue());
			    carinterface.setSteeringValue(drivingsettings.getSteeringValue());

			}
			testControlPanel.addSpeedValue(map.getSpeedEstimate());
			testControlPanel.addMergeScore(map.getLastMapMergeScore());
			testControlPanel.addRotationSpeedValue(map.getRotationSpeedEstimate());
			testControlPanel.addMatchRatio(map.getMatchRatio());
			display.setEstimatedRotationAngle(map.getRotationEstimate());
			if (useVideo)
			    videoControls.updateVideoFrame();
		  }
		  catch (Exception e)
		  {
		     e.printStackTrace();
			 break;
		  }
		  display.repaint(); // show any changes made to the display.
	      Thread.sleep(100);
	      time_ms = System.currentTimeMillis();
	     }
	     carinterface.setSpeedValue(0); // stop the car.
	     System.out.println("Processing stopped so car stopped.");
	   }
	   catch (Exception e)
	   {
	      e.printStackTrace();
	   }
    }
  }
}