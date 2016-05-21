package com.iarrc.roboticsio;

import java.util.*;
import org.w3c.dom.*;
import java.net.URL;
import java.io.*;
import java.awt.image.*;
import javax.imageio.ImageIO;

/**
For interacting with the simulation software.
*/
public class IARRCSimulator extends IARRCGeneralInterfaceAdapter
{
 private static long MAX_CONTROL_UPDATE_TIME = 100;
 private String docRootAddress; 
 private long lastSteeringUpdate=Long.MIN_VALUE;
 private long lastSpeedUpdate=Long.MIN_VALUE;
 private double prevSteeringValue;
 private double nextSteeringValue;
 private double prevSpeedValue;
 private double nextSpeedValue;

  /**
  @param docRootAddress must be the address of the document server.
    For example "http://localhost:2600/"
  */
  public IARRCSimulator(String docRootAddress)
  {
     assert(docRootAddress!=null);
	 
	 if (!docRootAddress.endsWith("/"))
	   docRootAddress = docRootAddress+"/";
	 
	 this.docRootAddress = docRootAddress;
	 
	 new UpdaterThread().start();
  }
 
  public LinkedList<String> getLoadableTracks()
  {
    return null;
  }

  public LinkedList<Sensor> getSensors() throws Exception
  {
   LinkedList<Sensor> result = new LinkedList<Sensor>();
   Document dom = XMLHelper.getDocFromURL(docRootAddress+"get?item=sensors");
   NodeList nl = dom.getElementsByTagName("device");
   
    // loop through elements.
	for (int i=0;i<nl.getLength();i++)
    {
	   Sensor s = SensorFactory.createFrom((Element)nl.item(i),this);
	   result.add(s);
	}
    
	return result;
  }
  
  private void sendGETRequest(String urlS) throws Exception
  {
     URL url = new URL(urlS);
	 url.openStream(); // send get request.
  }
  
  public void setSteeringValue(double value) throws Exception
  {
    nextSteeringValue=value;
	if (prevSteeringValue==0)
	    updateSteeringBlocking();
  }
  
  public void setSteeringValueBlocking(double value) throws Exception
  {
         nextSteeringValue=value;
		 updateSteeringBlocking();
  }
  
  private void updateSteeringBlocking()
  {
    try
	{
      // send request to "";
	  String url = docRootAddress+"set?item=steering&value="+nextSteeringValue;
	
	  sendGETRequest(url);
	  prevSteeringValue = nextSteeringValue;
	  lastSteeringUpdate = System.currentTimeMillis();
  	}
	catch (Exception exc)
	{
	   exc.printStackTrace();
	}
  }
  
  public void updateSpeedBlocking()
  {
    try
	{
         // send request to "";
	  String url = docRootAddress+"set?item=drive&value="+nextSpeedValue;

	    sendGETRequest(url);
        prevSpeedValue = nextSpeedValue;
	    lastSpeedUpdate = System.currentTimeMillis();
	}
	catch (Exception exc)
	{
	   exc.printStackTrace();
	}
  }
  
  public void setSpeedValue(double value) throws Exception
  {
    nextSpeedValue=value;
    if (prevSpeedValue==0)
	     updateSpeedBlocking();
		 
	// another thread will handle sending the message to the server.
  }
  
  public void setSpeedValueBlocking(double value) throws Exception
  {
         nextSpeedValue=value;
		 updateSpeedBlocking();
  }

  /**
   Makes a car that uses this interface to update its information.
   
   The returned object can be treated like a proxy for the real car.
  */
  public RoboticCar createRoboticCar()
  {
	return new RoboticCar(this);
  }
  
  public double[] getLidarDepthData(String sensorName,int numDepths) throws Exception
  {
    String urlS = docRootAddress+"get?item=sensordata&name="+sensorName+"&numpoints="+numDepths;
	URL url = new URL(urlS);
	double result[]=new double[numDepths];
	String line = null;
	BufferedReader br = new BufferedReader(new InputStreamReader(url.openStream()));
	int index = 0;
	
	 // loop through lines of the resulting document.
	 while ((line=br.readLine())!=null)
	 {
	    try
		{
		  double value = Double.parseDouble(line);
		  
		   result[index]=value;
		   index++; // go to next index
		}
		catch (NumberFormatException nfe)
		{
		   nfe.printStackTrace();
		   System.err.println("Problem getting double from Lidar data.");
		}
	 }
	 
	 return result;
  }
  
   public double getDepth(String sensorName) throws Exception
   {
     String urlS = docRootAddress+"get?item=property&name=device-"+sensorName+"-depth";
	 URL url = new URL(urlS);
	 Scanner sc = new Scanner(url.openStream());
	 return sc.nextDouble();
   }
   
   public String getConnectionString()
   {
     return docRootAddress;
   }
   
   public java.awt.Image getCurrentFrame(String videoSensorName) throws Exception
   {
    String urlS = docRootAddress+"get?item=frame&name="+videoSensorName;
	URL url = new URL(urlS);
	 
	BufferedImage result = ImageIO.read(url); 
	 
	 return result;
   }
   
   /**
   Stops the simulator from progressing time forward on its own with computer's clock.
   After calling this, use 
   */
   public void stopRealTime() throws Exception
   {
     String urlS = docRootAddress+"set?item=iniproperty&name=application-paused&value=1";
	  sendGETRequest(urlS);
   }
   
   /**
    Makes the simulator progress through time on its own using the computer's clock.
   */
   public void startRealTime() throws Exception
   {
     String urlS = docRootAddress+"set?item=iniproperty&name=application-paused&value=0";
	 
   	  sendGETRequest(urlS);
   }
   
   /**
   Gets the simulator to pass the specified amount of time.
   */
   public void passTime(int milis) throws Exception
   {
     String urlS = docRootAddress+"set?item=time&value="+milis;
	 
   	  sendGETRequest(urlS);
   }
   
   public void updateInfoWithCar()
   {
    long time = System.currentTimeMillis();
	long dt;

     if (nextSpeedValue!=prevSpeedValue)
	 {
	   dt = time-lastSpeedUpdate;

	    if (dt>MAX_CONTROL_UPDATE_TIME || dt<0)
		{
	
		   updateSpeedBlocking();
		}
	 }
	 if (nextSteeringValue!=prevSteeringValue)
	 {
	   dt = time-lastSteeringUpdate;
	    if (dt>MAX_CONTROL_UPDATE_TIME || dt<0)
		{
	        updateSteeringBlocking();
	    }
	 }
   }

   /**
    Tries to run the simulator that was installed at the specified directory.
   */   
   public void runSimulator(String installDirectory)
   {
      if (!installDirectory.endsWith("\\")&&!installDirectory.endsWith("/"))
	  {
	     installDirectory = installDirectory+File.separatorChar;
	  }

       Process p;
        ProcessBuilder builder = new ProcessBuilder();
        String filename = installDirectory+"IARRC_Sim.exe";

        if (!(new File(filename).exists()))
            filename = installDirectory+"IARRC_Sim";
        

        builder = builder.command(filename);
        try
        {
		  builder.directory(new File(installDirectory));
          p = builder.start();
		  System.out.println("Running: "+filename);
		  try
		  {
		    for (int i=0;i<5;i++)
			{
		      Thread.sleep(5000); // sleep for 5 seconds before continuing.
			  if (isSimulatorRunning())
			      break;
				  
			  System.out.println("Attempting "+(i+1)
			   +" times to connect with the simulator.");
			}
		  }
		  catch (Exception e)
		  {
		     e.printStackTrace();
		  }
		  if (!isSimulatorRunning())
		  {
		    System.out.println("Please run the simulator.");
		  }
        }
        catch (IOException ioe)
        {
          ioe.printStackTrace();
		  System.out.println("Is this running on the same computer?  "
		  +"The simulator is not running and attempts to make it run automatically have failed.");
        }
   }
   
   /**
   Checks if the simulator is running.
   */
   public boolean isSimulatorRunning()
   {
     try
	 {
	    sendGETRequest( this.docRootAddress);	
        return true;		
	 }
	 catch (Exception exc)
	 {
	    return false;
	 }
   }

   class UpdaterThread extends Thread
   {
     
     public void run()
	 {
	 
	  try{
	   while (true)
	   {
	    
	      updateInfoWithCar();
	      Thread.sleep(50); // sleep for a tenth of a second.
	   }
	   }
	   catch (Throwable t)
	   {
	     t.printStackTrace();
	   }
	 }
   }
   
}

