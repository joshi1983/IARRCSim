package mapping;

import geometry.*;
import java.awt.*;
/**
 Represents the position and direction the lidar is pointing from and to.
*/
public class LidarSensorPerspective
{
 // where is the sensor looking from on the map?
 private Vector3D position;
 

 
 // how much is the sensor rotated?
 private Rotator2D rotator;
 
 /**
  Maximum angle range in degrees
 */
 private double maxAngleRangeDegrees=270;
  
 private double maxDepth;
 private long lastUpdateTime=0;
 
 /**
  Rate of change in rotation over time.
  Measured in radians per second
 */
 private double rotationChangeRate=0;
 
 /**
  Rate of change in position over time.
  Measured in cm per second
 */
 private Vector3D velocity=new Vector3D(0,0,0);
 
 
  public LidarSensorPerspective()
  {
     this.maxDepth = 400;
     position = new Vector3D(0,0,0);
	 rotator=new Rotator2D(0);
  }
  
  /**
  Predicts based on current velocity and rate of rotation 
  what position and rotation this sensor will have and sets it that way.
  
  @param time_ms is the number of milliseconds into the future to go.
  */
  public void goToPredictedFutureState(long time_ms)
  {
     position = position.plus(velocity.times(time_ms/1000.0));
	 rotator.setRotationRadians( rotator.getRotationAngleRadians()+rotationChangeRate*time_ms/1000.0);
  }
  
  public void blendInVelocity(Vector3D v)
  {
    double ratio=0.1;
     this.velocity = this.velocity.times(1-ratio).plus(v.times(ratio));
  }
  
  public void setVelocity(Vector3D v)
  {
    this.velocity=v;
  }
  
  public Vector3D getVelocity()
  {
    return this.velocity;
  }
  
  /**
   Resets properties of this lidar sensor
  */
  public void reinit()
  {
     position = new Vector3D(0,0,0);
	 rotator.setRotationRadians(getMaxAngleRange()*Math.PI/180/2);
	 lastUpdateTime=0;
	 allStop();
  }
  
  public void allStop()
  {
	 rotationChangeRate=0;
	 
	 velocity=new Vector3D(0,0,0);
  }
  
  /**
  Sets the maximum number of centimeters of depth detectable from this Lidar perspective
  */
  public void setMaxDepth(double maxDepth)
  {
     this.maxDepth = maxDepth;
  }
  
  /**
  Returns the maximum number of centimeters of depth detectable from this Lidar perspective
  */
  public double getMaxDepth()
  {
    return maxDepth;
  }
  
  public Vector3D getPosition()
  {
     return position;
  }
  
  public Vector2D getPositionAsVector2D()
  {
    return position.getXZAsVector2D();
  }
  
  public void rotateBy(double rotationRadians)
  {
    rotator.setRotationRadians(rotator.getRotationAngleRadians()+rotationRadians);
  }
  
  public void displaceBy(Vector2D v)
  {
    position.displaceXZBy(v);
  }
  
  public void setMaxAngleRange(double degrees)
  {
     this.maxAngleRangeDegrees = degrees;
  }
  
  public double getMaxAngleRange()
  {
    return maxAngleRangeDegrees;
  }
  
  public void blendInRotationRate(double rotationRadians)
  {
   double ratio=0.2;
    this.rotationChangeRate=this.rotationChangeRate*(1-ratio)+rotationRadians*ratio;
  }
  
  /**
   Sets the estimated rate of rotation the lidar sensor is currently in
  */
  public void setRotationRate(double rotationRadians)
  {
    this.rotationChangeRate = rotationRadians;
  }
  
  public void setRotationRadians(double rotationRadians)
  {
    this.rotator.setRotationRadians(rotationRadians);
  }
  
  public double getRotationRadians()
  {
    return rotator.getRotationAngleRadians();
  }
  
  /**
   Returns the current rotation change rate in radians per second
  */
  public double getRotationChangeRate()
  {
    return rotationChangeRate;
  }
 
  public void paint(Graphics g,Vector3D offset,double scale)
  {
    double x=scale*(position.getX()-offset.getX()),
	   y=scale*(position.getY()-offset.getY()),
	   z=scale*(position.getZ()-offset.getZ());
	   
	   g.setColor(Color.WHITE);
	   g.fillRect((int)x,(int)z,2,2);
	  
     double r = 100;	  
	 double dx=r*Math.cos(Math.PI*3/4+rotator.getRotationAngleRadians()),
	  dz=r*Math.sin(Math.PI*3/4+rotator.getRotationAngleRadians());
	  
	 double x2=scale*(dx+position.getX()-offset.getX());
	 double z2=scale*(dz+position.getZ()-offset.getZ());
	  
	  g.drawLine((int)x,(int)z,(int)x2,(int)z2);
  }
  
  public double distanceFrom(Obstacle o)
  {
    return o.distanceFrom(position.getXZAsVector2D());
  }
  
  public DisplaceAndRotateTransformation getTransformation()
  {
    return new DisplaceAndRotateTransformation(0,0,position.getX(),position.getZ(),rotator.getRotationAngleRadians());
  }
  
  public void setLastUpdateTime(long time_ms)
  {
    lastUpdateTime = time_ms;
  }
  
  public long getLastUpdateTime()
  {
    return lastUpdateTime;
  }
}