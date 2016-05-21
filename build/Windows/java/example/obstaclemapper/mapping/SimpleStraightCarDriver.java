package mapping;

/**
 A very simple driver that doesn't look at the map at all.  
It simply drives forward at a constant speed value and disregards any obstacles in the way.

*/
public class SimpleStraightCarDriver extends CarDriver
{

  public DriveSettings getNewDriveSettingsFrom(ObstacleMap map)
  {
      return new DriveSettings(2,0);
  }
}