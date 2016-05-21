package mapping;

import geometry.*;
import java.awt.*;

/**
 Represents a Pylon in a generated map
*/public class PylonObstacle extends Obstacle
{
  private static final double RADIUS = 5;
   public PylonObstacle(double x,double y,double z)
   {     super(x,y,z);
	 setColour(Color.ORANGE);   }
   
   public Object clone()
   {
      PylonObstacle result = new PylonObstacle(getX(),getY(),getZ());
	  result.setColour(this.getColour());
	  return result;
   }
   public void paint(Graphics g,Vector3D offset,double scale)  
   {
     g.setColor(getColour());     	 
     double x=scale*(getX()-offset.getX()),
        y=scale*(getY()-offset.getY()),
        z=scale*(getZ()-offset.getZ());
     double r = RADIUS*scale;
	 
	 int diameter = (int)(r*2+1); 
	   // the +1 to keep it visible even when it would otherwise go hidden with diameter=0.	 	 
       // fill a circle to represent this pylon
     g.fillOval((int)(x-r),(int)(z-r),diameter,diameter);  
   }
}