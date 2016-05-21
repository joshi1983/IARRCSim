package mapping;

public class DriveSettings
{
 private double speedValue;
 private double steeringValue;
 
  public DriveSettings(double speedValue,double steeringValue)
  {
    this.speedValue=speedValue;
	this.steeringValue=steeringValue;
  }

  public double getSpeedValue()
  {
    return speedValue;
  }
  
  public double getSteeringValue()
  {
    return steeringValue;
  }
}