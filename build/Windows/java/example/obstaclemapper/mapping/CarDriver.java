package mapping;

/**
Represents anything capable of determining 
what steering and driving speed values to use.

Each instance is like the virtual form of a human driver of 
a car in that it looks at current and past information to determine how to steer or control the vehicle's speed.
*/
public abstract class CarDriver
{

  public abstract DriveSettings getNewDriveSettingsFrom(ObstacleMap map);
}