package com.iarrc.roboticsio;

import com.iarrc.math.*;
import java.awt.*;


public class VideoSensor extends Sensor
{
 private static long MAX_FRAME_UPDATE_TIME_LIMIT = 500;
 private int width,height;
 
 // the field of view in degrees
 private double fovDegrees;
 
 private Vector3D rotation=new Vector3D(); 
 // rotation angles for the camera.  This determines camera direction and orientation
 private Image frame=null;
 private long lastFrameUpdate;
 
  public VideoSensor(String name,IARRCGeneralInterfaceAdapter generalInterface)
  {
    super(name, generalInterface);
  }
  
  public void updateFrame()
  {
   long time = System.currentTimeMillis();
   if (time-lastFrameUpdate>MAX_FRAME_UPDATE_TIME_LIMIT)
   {
     try
     {
       frame = generalInterface.getCurrentFrame(getName());
	   lastFrameUpdate = time;
	 }
	 catch (Exception e)
	 {
	   e.printStackTrace();
	 }
    }
  }
  
  public Image getCurrentFrame()
  {
    updateFrame();
	
	return frame;
  }
  
  /**
   Paints the current display onto the graphics object
   @param left is for leaving a margin to the left of the graphics object
   @param top is for leaving a margin to the top of the graphics object
  */
  public void paintDisplay(Graphics g,int left,int top,int maxHeight)
  {
     Image img = getCurrentFrame();
	 
	 if (img!=null)
	 {
	    width = img.getWidth(null);
		height = img.getHeight(null);
		
		if (width<=0 || height<=0) // don't scale the image when dimensions are unknown.
	       g.drawImage(img,left,top,null);
		else // scale the image
		   g.drawImage(img,left,top,maxHeight*width/height,maxHeight,null);
	 }
  }
  
  public void setProperty(String propertyName,String value)
  {
     if (propertyName.equals("fov-degrees"))
	    fovDegrees = Double.parseDouble(value);
     else if (propertyName.equals("width"))
	    width=Integer.parseInt(value);
	 else if (propertyName.equals("height"))
	    height = Integer.parseInt(value);
	 else if (propertyName.startsWith("rotation"))
	    rotation.setProperty(propertyName,value);
	 else
	    super.setProperty(propertyName,value);
  }
}