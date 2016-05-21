package geometry;

/**
 Represents the approximation of a circle
*/
public class CircleApproximation
{
 /**
  Approximated radius of the circle
 */
 private double radius;
 
 /**
  Coordinates of the centre.
 */
 private double x,y;
 
 /**
  A value measuring the accuracy of this approximation.
  
  The lower the number, the more accurately the original shape represents a circle.
 */
 private double accuracyMeasure;
 
  public CircleApproximation(double radius,double x,double y,double accuracy)
  {
    this.x=x;
	this.y=y;
	this.radius=radius;
	this.accuracyMeasure=accuracy;
  }
  
  public double getAccuracy()
  {
     return accuracyMeasure; 
  }
 
  public double getCentreX()
  {
     return x;
  }
 
  public double getCentreY()
  {
    return y;
  }
 
  public double getRadius()
  {
    return this.radius;
  }
}