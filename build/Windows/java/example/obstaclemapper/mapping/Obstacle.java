package mapping;

import geometry.*;
import java.awt.*;

public class Obstacle
{
 private double x,y,z;
 private Color colour;
 
 /** The last time this obstacle was detected */
 private long lastMatchedTime=0;
 
 /**
  A value calculated to help match this obstacle with another one.
 */
 private double patternIdentificationValue=0;
 
  public Obstacle(double x,double y,double z)
  {
    assert !(new Double(x).isNaN());
	assert !(new Double(y).isNaN());
	assert !(new Double(z).isNaN());
	
    this.x=x;
	this.y=y;
	this.z=z;
	
	setColour(null);
  }
  
  public void setPatternIdentificationValue(double val)
  {
    this.patternIdentificationValue = val;
  }
  
  public double getPatternIdentificationValue()
  {
    return this.patternIdentificationValue;
  }
  
  public void setLastMatchedTime(long time_ms)
  {
    this.lastMatchedTime=time_ms;
  }
  
  public long getLastMatchedTime()
  {
    return lastMatchedTime;
  }
  
  /**
  Makes a copy of this obstacle.
  
  This method is to be overridden by any subclass.
  */
   public Object clone()
   {
     Obstacle result= new Obstacle(x,y,z);
	 result.colour = this.colour;
	 return result;
   }
  
  /**
   Calculates distance between the centre of this obstacle and the centre of the specified one.
  */
  public double distanceFrom(Obstacle o)
  {
     return o.getPositionAsVector2D().distanceFrom(getPositionAsVector2D());
  }
  
  /**
   Calculates distance between the specified 2D point and the 2D version of this obstacle's position.
  */
  public double distanceFrom(Vector2D p)
  {
    return p.distanceFrom(x,z);
  }
  
  /**
   Sets the colour this obstacle is drawn as.
  */
  public void setColour(Color c)
  {
    if (c==null)
	   this.colour = Color.WHITE;
	   
    this.colour = c;
  }
  
  public Color getColour()
  {
    return this.colour;
  }
 
  public void paint(Graphics g,Vector3D offset,double scale)
  {
     g.setColor(colour);     
	 double x=scale*(getX()-offset.getX()),
	   y=scale*(getY()-offset.getY()),
	   z=scale*(getZ()-offset.getZ());
	   
	   g.fillRect((int)x,(int)z,2,2);
  }
 
  public double getX()
  {
     return x;
  }
 
  public double getY()
  {
     return y;
  }
 
  public double getZ()
  {
    return z;
  }
 
  /**
  Returns a point made out of the x and z coordinates of this obstacle's position 
  */
  public Vector2D getPositionAsVector2D()
  {
    return new Vector2D(x,z);
  }
  
  public void transformPositionXZBy(Transformation transform)
  {
     Vector2D v = transform.transform(getPositionAsVector2D());
	 
	assert !(new Double(v.getX()).isNaN());
	assert !(new Double(v.getY()).isNaN());
	
	 this.x=v.getX();
	 this.z=v.getY();
  }
  
  /**
  Averages the x and z components of this obstacle's position 
  with the x and y components of the specified v vector.
  
  The main use of this method is to move obstacles slightly 
  as new data reveals some error in previous position estimations.
  
  @param ratio is the ratio by which the averaging is to happen.  
   if ratio=0, nothing is changed.  
   if ratio=1, (x,z) of this obstacle's position is replaced with v.
   if ratio=0.5, the values are averaged together evenly.
  */
  public void averagePositionXZWith(Vector2D v,double ratio)
  {
    double ratio2 = 1-ratio;
	assert !(new Double(ratio).isNaN());
	assert !(new Double(v.getX()).isNaN());
	assert !(new Double(v.getY()).isNaN());
	
     this.x=ratio2*this.x+ratio*v.getX();
	 this.z=ratio2*this.z+ratio*v.getY();
  }
}