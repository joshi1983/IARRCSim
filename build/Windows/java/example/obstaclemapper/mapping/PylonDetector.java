package mapping;

import graphics.*;

public class PylonDetector
{
  public static void thresholdForPylons(QuickAccessImage img)
  {
   int[] pixelData = img.getPixelData();
   
    // loop through all pixels.
    for (int i=0;i<pixelData.length;i++)
	{
	   int val = pixelData[i];
	   int red=(val>>16)&0xff,green=(val>>8)&0xff,blue=val&0xff;
	   
	   // is the pixel a pylon-like colour or not?
	    if (red-green>70 && red-blue>100 && blue<100 && red>150 && green>70)
		{
	       
		}
		else
		    pixelData[i]=0; // black
	}
  }
  
  /**
  Guesses some drive settings based on the specified image 
  from a camera aiming infront of the car with a field of view of roughly 40 to 60 degrees.
  
  Uses angles to determine steering direction based on ideas used by 
   University of Windsor's Team Awesome and Team Invinsible for the drag race competition.
  
  Ignores stop signs, traffic lights, unique aspects of ramps and tunnels.
  
  @param img is assumed to be thresholded so non-pylon pixels are black.
  */
  public static DriveSettings getDriveSettingsBasedOnPylonThresholding(QuickAccessImage img)
  {
    int[] y_coordinates=new int[img.getWidth()];
  
     for (int x=0;x<y_coordinates.length;x++)
     {
	     y_coordinates[x]=-1; // indicate no pylon pixel found.
		 
	     for (int y=img.getHeight()-1;y>=0;y--)
		 {
		    if (img.getPixelUnchecked(x,y)!=0)
			{
			    y_coordinates[x]=y;
			    break;
			}
		 }
	 }
	 // get slope of right and left sides separately.
	 
	 
	 // average the slopes together.
	 
  
    double angle=0;
	
     return new DriveSettings(angle,3);
  }

}