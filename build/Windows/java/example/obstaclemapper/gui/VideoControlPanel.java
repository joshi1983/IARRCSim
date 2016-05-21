package gui;

import com.iarrc.roboticsio.*;
import graphics.*;
import mapping.*;
import java.awt.Image;
import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Dimension;

/**
 For looking at and controlling how video data is processed
*/
public class VideoControlPanel extends JPanel
{
 private ImageSetPanel imagesPanel=new ImageSetPanel();
 private ImageDisplayPanel mainImage = new ImageDisplayPanel();
 private ImageDisplayPanel pylonImage = new ImageDisplayPanel();
 private ImageDisplayPanel splitImage1 = new ImageDisplayPanel();
 private ImageDisplayPanel splitImage2 = new ImageDisplayPanel();
 private ImageDisplayPanel splitImage3 = new ImageDisplayPanel();
 private MapDisplayFrame frame;
 private IARRCGeneralInterfaceAdapter carinterface;
 private String videoSensorName;
 
  public VideoControlPanel(MapDisplayFrame frame,IARRCGeneralInterfaceAdapter carinterface,String videoSensorName)
  {
      this.carinterface=carinterface;
      this.videoSensorName=videoSensorName;
      this.frame=frame;
      setLayout(new BorderLayout());
      add(imagesPanel,BorderLayout.CENTER);
	  setPreferredSize(new Dimension(150,1000));
	  updateDisplay();
  }

  public void updateDisplay()
  {
  	     imagesPanel.clear();
	     imagesPanel.add(mainImage);
		 imagesPanel.add(pylonImage);
	     imagesPanel.add(splitImage1);
		 imagesPanel.add(splitImage2);
		 imagesPanel.add(splitImage3);
  }
  
  /**
   Retrieves the current video frame and updates this panel's display to reflect the new information collected
  */
  public void updateVideoFrame()
  {
    try
	{
      Image img = carinterface.getCurrentFrame(videoSensorName);
	  QuickAccessImage qimg = new QuickAccessImage(img);
	
	  QuickAccessImage im = new QuickAccessImage(qimg);
	  QuickAccessImage pylonImg=new QuickAccessImage(qimg);
	  QuickAccessImage []rgbchannels=qimg.getRGBSplit();
//	  PylonDetector.thresholdForPylons(im);
	 
	     PylonDetector.thresholdForPylons(pylonImg);
	     mainImage.setQuickAccessImage(qimg);
		 
		 pylonImage.setQuickAccessImage(pylonImg);
		 splitImage1.setQuickAccessImage(rgbchannels[0]);
		 splitImage2.setQuickAccessImage(rgbchannels[1]);
		 splitImage3.setQuickAccessImage(rgbchannels[2]);
	}
	catch (Exception e)
	{
	   e.printStackTrace();
	   System.out.println("Unable to update video frame.");
	}
  }
}