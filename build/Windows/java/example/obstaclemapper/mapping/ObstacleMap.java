package mapping;

import geometry.*;
import java.util.*;
import java.awt.*;

/**
 Represents a map of obstacles being generated as a car explores an environment
*/
public class ObstacleMap
{
 /**
  The minimum ratio of matched obstacles that can be considered reliable.
  
  Smaller ratios are considered unreliable and indicate that no good transformation was discovered.
  As a result, all older obstacles to be cleared out of the map.
 */
 public static final double MIN_RELIABLE_MATCH_RATIO=0.6;

  /**
   A rotation speed higher than this is extremely weird and assumed to be an error.
   The unit is radians per second.
  */
 public static final double MAX_ROTATION_SPEED=4;
 
 /**
 The minimum distance between pylons that is expected in the track.
 measured in centimeters
 */
 public static final double TRANSFORMATION_DISPLACEMENT_DISTANCE_LIMIT = 15;
 
 /**
 If 2 obstacles are closer than this number of centimeters, 
 it indicates a more likely close match for the transformation used so it gets rewarded.
 */
 public static final double CLOSE_MATCH_REWARD_THRESHOLD = 7;
 
  /**
 If 2 obstacles are closer than this number of centimeters, 
 it indicates a much more likely close match for the transformation used so it gets rewarded more.
 */
 public static final double VERY_CLOSE_MATCH_REWARD_THRESHOLD = 4;
 
 private LinkedList<Obstacle> obstacles=new LinkedList<Obstacle>();
 private LidarSensorPerspective perspective=new LidarSensorPerspective();
 private LinkedList<Obstacle> tempObstacles = new LinkedList<Obstacle>();
 private double lastMapMergeScore=0;
 private double ratioMatched=0;
 private long lastReliableUpdateTime=0;
 private long lastMergeTime=0;
  
  /**
  Estimates current speed of the lidar moving through the map in cm/second
  */
  public double getSpeedEstimate()
  {
    return perspective.getVelocity().getMagnitude();
  }
  
  public double getRotationEstimate()
  {
     return perspective.getRotationRadians();
  }
  
  public double getRotationSpeedEstimate()
  {
    return perspective.getRotationChangeRate();
  }
  
  public LidarSensorPerspective getLidarSensorPerspective()
  {
    return perspective;
  }
  
  public void allStop()
  {
     perspective.allStop();
  }
  
  public boolean isUnstable()
  {
     return ratioMatched<MIN_RELIABLE_MATCH_RATIO || 
	   Math.abs(perspective.getRotationChangeRate())>MAX_ROTATION_SPEED;
  }
  
  /**
   Sets each Obstacle's patternIdenficationValue to a value between 0 and 1000.
   The value can be used to help match obstacles when merging 2 maps together.
  */
  public synchronized void updateObstaclePatternIdentificationValues()
  {
   Obstacle[] obstacleArray = new Obstacle[obstacles.size()];
    System.arraycopy(obstacles.toArray(),0,obstacleArray,0,obstacleArray.length);
  
    // loop through all obstacles.
	for (Obstacle o: obstacles)
	{
	   // find the identification value.
	   double val = 0;
	   if (o instanceof PylonObstacle)
	      val+=333;
		  
	   // find the nearest 2 pylons.
	   sortByPylonObstacleAndDistanceFrom(o,obstacleArray);
	   if (obstacleArray.length>=2)
	   {
	     Vector2D o_pos = o.getPositionAsVector2D();
	     double prevAngle=0;
		 assert val!=Double.NaN;
		 
	      for (int i=0;i<2;i++)
		  {
		    Vector2D pos = obstacleArray[i].getPositionAsVector2D().minus(o_pos);
	        double a1 = Math.atan2(pos.getY(),pos.getX());
			double angle_difference=0;
			
			if (i!=0)
			   angle_difference=a1-prevAngle;
			   
			   if (new Double(pos.getX()).isNaN())
			       break;
				   
			  // System.out.println("pos = "+pos);
			double m = pos.getMagnitude();
			assert !(new Double(m).isNaN());
			if (m>333 || m==Double.NaN)
			   break;
			   
			assert !(new Double(val).isNaN());

			   
			val+=m*(1+Math.abs(angle_difference)/Math.PI)/2;
			assert !(new Double(val).isNaN());

			
			prevAngle=a1;
		  }
	   }
	   assert val<1000;
	   o.setPatternIdentificationValue(val);
	}
   
  }
  
  /**
   Returns a grid that maps out distances from various points 
   on the grid to their nearest obstacles
  */
  public double[][] createObstacleDistanceGrid(int size,double granularity)
  {
     double [][] distances = new double[size][size];
	 
	 for (int x=0;x<size;x++)
	 {
	   for (int y=0;y<size;y++)
	   {
	     double x3d=(x-size/2)*granularity,y3d=0,z3d=(size*3/10-y)*granularity;
		 Vector2D v = new Vector2D(x3d,z3d);
		   v.rotateBy(-Math.PI*3/4+perspective.getRotationRadians());
		   v = v.plus(perspective.getPositionAsVector2D());
		 
	     Obstacle nearestObstacle = getClosestMatchFor(v);
		 double distance = Double.MAX_VALUE;
		 
		  if (nearestObstacle!=null)
	         distance=nearestObstacle.distanceFrom(v);
			 
		  distances[x][y]=distance;
	   }
	 }
	 
	 return distances;
  }
  
  /**
  Returns a value that is most negative when the transformation works best.
  
  The value returned is roughly the sum of all distances from obstacles in 
  the specified map to their closest corresponding obstacles in this map.
  
  Some priority is given to very close matches between individual obstacles which can cause negative results.
  
  */
  private double getTransformationScore(ObstacleMap map, DisplaceAndRotateTransformation transform) 
  {
    double result = 0;
  
	   for (Obstacle o: map.obstacles)
	   {
	      Obstacle copy = (Obstacle)o.clone();
	      copy.transformPositionXZBy(transform);
		  Obstacle other = getClosestMatchFor(copy);
		  double distanceFromClosestMatch = Double.MAX_VALUE;
		  
		  if (other!=null)
		     distanceFromClosestMatch= other.distanceFrom(copy);
			 
		  // encourage very close matches.
		  if (distanceFromClosestMatch<CLOSE_MATCH_REWARD_THRESHOLD)
		     distanceFromClosestMatch = distanceFromClosestMatch
			  +(distanceFromClosestMatch-CLOSE_MATCH_REWARD_THRESHOLD)*8;
			  
		  if (distanceFromClosestMatch<VERY_CLOSE_MATCH_REWARD_THRESHOLD)
		      distanceFromClosestMatch = distanceFromClosestMatch
			  +(distanceFromClosestMatch-VERY_CLOSE_MATCH_REWARD_THRESHOLD)*15;
			 
		   // make minor adjustments to position assuming the detection of the obstacle may increase accuracy.
		  result+=Math.min(distanceFromClosestMatch,TRANSFORMATION_DISPLACEMENT_DISTANCE_LIMIT);
	   }  
	   
	   return result;
  }
  
  private boolean lidarDataShouldButDidntDetect(double[] lidarData,Obstacle o)
  {
      Vector2D pv = perspective.getPositionAsVector2D();
	  Vector2D v = o.getPositionAsVector2D();
  	  double distance = v.distanceFrom(pv);
	  
	  if (distance>perspective.getMaxDepth())
		  return false;
		  
      Transformation t =perspective.getTransformation();
      v=t.getInverse().transform(v);
	  
	  double angleRange = perspective.getMaxAngleRange()*Math.PI/180;
	  // angleRange could be pi*3/2 (270 degrees)
	  
	  //System.out.println("v="+v);

      double obstacleAngle = Math.atan2(v.getY(),v.getX())-angleRange/2;
	  // obstacleAngle is from -pi to pi (-180 to 180 degrees).

	  double da = Math.abs(obstacleAngle+Math.PI)-(Math.PI/4*1.2);
	  
	  // a value between -pi and pi, typically
	  int s=(int)Math.abs(da*80);
	  
	  if (s>255) s=255;
	  
	  if (da>0) // if in detectable region
	  {
	    //  o.setColour(new Color(s,s,s));
		  return true;
	  }
	//  else
	  //   o.setColour(new Color(s,255,0));
	  
	 return false;
  }
  
  /**
  Removes obstacles that should be detected in the most recent merged data but are not.
  
  @param lidarData is the lidar data for a current scan.
  */
  public synchronized void removeUnlikelyObstacles(double [] lidarData,long time_ms)
  {
    LinkedList<Obstacle> toRemove=new LinkedList<Obstacle>();
	
     // loop through obstacles.
	 for (Obstacle o: obstacles)
	 {
	   assert o.getLastMatchedTime()!=0;
	   
	   if (o.getLastMatchedTime()==time_ms)
	      continue;
		  
	  if (o.getLastMatchedTime()<lastReliableUpdateTime && lastReliableUpdateTime!=0)
         toRemove.add(o);
	  else if (lidarDataShouldButDidntDetect(lidarData,o))
	       toRemove.add(o);
	 }

	 for (Obstacle o: toRemove)
	 {
	    obstacles.remove(o);
	 }
  }
  
  public long getLastMergeTime()
  {
     return lastMergeTime;
  }
  
  /**
  Tries to merge the specified map on top of this one.
  It is assumed that the specified map was generated from 
  the same lidar that is represented in this map already but that 
  time passed and along with it some rotation and displacement may have happened.
  
  The rotation(steering motion) and displacement from driving forward or reverse 
  is found by finding a good fit of the specified map obstacles over the existing ones.
  
  Updates the position and orientation of this map's lidar perspective.
  
  @param time_ms is the number of miliseconds after some set point.  
  time_ms is used to predict the orientation and position of the lidar 
  as the car moves and steers through time.
    The set initial time point should not change without clearing this map.
  */
  public synchronized void merge(ObstacleMap map,long time_ms)
  {
     lastMergeTime=time_ms;
     tempObstacles.clear();
	 
     if (map.obstacles.isEmpty()) // nothing to do.
	    return;
		
	 /*  if (ratioMatched<MIN_RELIABLE_MATCH_RATIO)
	   {
	      lastReliableUpdateTime=time_ms;
		  clear();
		  perspective.reinit();
		  ratioMatched=1;
		  map.setLastMatchedTime(time_ms);
          this.obstacles.addAll(map.obstacles);
		  return;
	   }*/
	   if (isUnstable())
	      clear();
		  
	//clear();
	 
	// updateObstaclePatternIdentificationValues();
	 //map.updateObstaclePatternIdentificationValues();
	 
	 Vector3D oldPerspectivePos = perspective.getPosition();
	 Vector2D oldPerspectivePos2D = oldPerspectivePos.getXZAsVector2D();
	 long oldTime = perspective.getLastUpdateTime();
	 long timeChange = time_ms-oldTime;
	 double oldRotation = perspective.getRotationRadians();
	 
	 if (perspective.getLastUpdateTime()!=0)
	    perspective.goToPredictedFutureState(time_ms-perspective.getLastUpdateTime());
	 
  	 // displace and rotate the trackables based on previous position and orientation of the lidar sensor.
	 {
	  DisplaceAndRotateTransformation transform = perspective.getTransformation();
	  
	   if (!transform.isIdentity())
	   {
	     for (Obstacle o: map.obstacles)
	     {
	        o.transformPositionXZBy(transform);
	     }
	   }
	 }
	 
     if (this.obstacles.size()<2)
     { // if the current map has almost nothing in it to match up with, just add all obstacles.
	       map.setLastMatchedTime(time_ms);
           this.obstacles.addAll(map.obstacles);
		   ratioMatched=1;
		   
		   return; // that's all.
     }
     // find some highly trackable obstacles out of the specified map.
	 Obstacle[] trackables = map.getObstaclesWithin(perspective.getMaxDepth(),perspective.getPositionAsVector2D());
     
     sortByTrackability(trackables);
	 
	 DisplaceAndRotateTransformation bestTransform=null;
	 double bestScore=Double.MAX_VALUE;
	// System.out.println("trackables.length: "+trackables.length);
	 for (int i=0;i<trackables.length-2;i++)
     {
	    // try connecting obstacles 0 to the nearest one in this map.
		Obstacle[] matched = new Obstacle[3];
        boolean allMatchesFound=true;
		
		for (int index=0;index<3;index++)
        {
		   matched[index]=getClosestMatchFor(trackables[i+index]);
		   if (matched[index]==null)
		   {
		      allMatchesFound=false;
		      break;
		   }
		}
		if (!allMatchesFound)
		    continue;
		
		DisplaceAndRotateTransformation transform = PointClusterTracking.getDisplaceAndRotateTransformation(
		   oldPerspectivePos2D,
		   trackables[i].getPositionAsVector2D(),
		   trackables[i+1].getPositionAsVector2D(),
		   trackables[i+2].getPositionAsVector2D(),
		   matched[0].getPositionAsVector2D(),matched[1].getPositionAsVector2D()
		   ,matched[2].getPositionAsVector2D()
		  );
		  
		if (transform==null)
		    continue;
		   
		if (transform.getDisplacementVector2D().getMagnitude()>TRANSFORMATION_DISPLACEMENT_DISTANCE_LIMIT)
		   continue;
		  
		 double score = getTransformationScore(map,transform); 
		 if (score< bestScore)
		 {
		    bestTransform = transform;

			bestScore=score;
		 }
	 }
	 
	 if (bestTransform!=null)
	 {
	    lastMapMergeScore = bestScore;
	   int matchCount=0;
	   
	   // loop through map obstacles transforming each by transform before adding it to the resulting map.
	   for (Obstacle o: map.obstacles)
	   {
	      o.transformPositionXZBy(bestTransform);
		  Obstacle other = getClosestMatchFor(o);
		  double distanceFromClosestMatch = Double.MAX_VALUE;
		  
		  if (other!=null)
		     distanceFromClosestMatch= other.distanceFrom(o);
			 
		  if (distanceFromClosestMatch<TRANSFORMATION_DISPLACEMENT_DISTANCE_LIMIT)
		  {
		      // make minor adjustments to position assuming the detection of the obstacle may increase accuracy.
			  if (o instanceof PylonObstacle)
			     o.setColour(new Color(100,60,0));
			  
			  other.averagePositionXZWith(o.getPositionAsVector2D(),0.05);
			  other.setLastMatchedTime(time_ms);
			  o.setLastMatchedTime(time_ms);
			  tempObstacles.add(o);
			  matchCount++;
		  }
		  else
		  {
		     o.setLastMatchedTime(time_ms);
		     obstacles.add(o);
		  }
	   }
	   ratioMatched = (double)matchCount/(double)map.obstacles.size();

	   perspective.rotateBy(bestTransform.getRotationRadians());
	   perspective.displaceBy(bestTransform.getDisplacementVector2D());
	   perspective.blendInVelocity(perspective.getPosition().minus(oldPerspectivePos).times((time_ms-oldTime)/1000.0));
	   perspective.blendInRotationRate((perspective.getRotationRadians()-oldRotation)*(time_ms-oldTime)/1000.0);
	   perspective.setLastUpdateTime(time_ms);
	 }
	 else
	 {
	    // otherwise, unable to merge.
		ratioMatched =0;

		// System.out.println("Unable to merge.");
     }
	 

  }
  
  public double getMatchRatio()
  {
     return ratioMatched;
  }
  
  private void setLastMatchedTime(long time_ms)
  {
     for (Obstacle o: obstacles)
	     o.setLastMatchedTime(time_ms);
  }
  
  public double getLastMapMergeScore()
  {
     return lastMapMergeScore;
  }
  
  /**
  Gets the obstacle nearest the specified point.
  */
  public synchronized Obstacle getClosestMatchFor(Vector2D pos)
  {
    double x,y,z;
	Obstacle result=null;
	double maxDifferenceValue=Double.MAX_VALUE;
	 
	 // loop through all obstacles.
	 for (Obstacle o: obstacles)
	 {   
	    double differenceValue = o.distanceFrom(pos);

		
		   if (differenceValue<maxDifferenceValue)
		   {
		       maxDifferenceValue=differenceValue;
			   result=o;
		   }
	 }

	 return result;
  }
  
  /**
  Returns the obstacle from this map that is closest matched with the specified other obstacle.
  Returns null if no obstacle can be found that sufficiently matches with other.
  */
  public synchronized Obstacle getClosestMatchFor(Obstacle other)
  {
    double x,y,z;
	Obstacle result=null;
	double patternIdentificationValueDifference=0;
	double maxDifferenceValue=Double.MAX_VALUE;
	boolean mustBePylon=(other instanceof PylonObstacle);
	 
	 // loop through all obstacles.
	 for (Obstacle o: obstacles)
	 {
	    // if the match must be a Pylon but o is not a Pylon, skip o.
	    if (mustBePylon && !(o instanceof PylonObstacle))
		   continue; 
		   
		if (o==other) // don't try to find the exact same obstacle.
		   continue;
		   // continue with the next obstacle.
		     
	    double differenceValue = o.distanceFrom(other);
		 /*
			if (differenceValue>150) // that's too big to be a match.
		       continue;
			 
		double patternDifferenceValue = 
		   o.getPatternIdentificationValue()-other.getPatternIdentificationValue();
		  */
		  
		   if (differenceValue<maxDifferenceValue /*&& patternDifferenceValue<100*/)
		   {
		       maxDifferenceValue=differenceValue;
			  // patternIdentificationValueDifference=patternDifferenceValue;
			   result=o;
		   }
	 }
	 
	 if (result==null)
	 {
	   // System.out.println("Unable to find an obstacle close to: "+other.getPositionAsVector2D());
	 }
	 else
	 {
	 //   System.out.println("match found, distance: "+maxDistance);
	 }
	// System.out.println("patternIdentificationValueDifference = "+patternIdentificationValueDifference);
	 return result;
  }
  
  /**
   Removes all obstacles from this map.
  */
  public synchronized void clear()
  {
    obstacles.clear();
	perspective.reinit();
  }
  
  /**
  Removes all obstacles that are not pylons or lines.
  This is a way of simplifying or cleaning out many difficult to track or noisy data points.
  */
  public synchronized void clearNonTrivialObstacles()
  {
      LinkedList<Obstacle> nontrivials = new LinkedList<Obstacle>();
	
     for (Obstacle obstacle: obstacles)
	 {
	    if ((obstacle instanceof PylonObstacle))
		   nontrivials .add(obstacle);
	 }
	 
	 obstacles = nontrivials ;
  }
  
  /**
   Removes all obstacles that are not pylons.
  */
  public synchronized void clearNonPylons()
  {
    LinkedList<Obstacle> pylons = new LinkedList<Obstacle>();
	
     for (Obstacle obstacle: obstacles)
	 {
	    if (obstacle instanceof PylonObstacle)
		   pylons.add(obstacle);
	 }
	 
	 obstacles = pylons;
  }
  
  /**
  Returns all obstacles within the specified distance of the specified position.
  */
  public synchronized Obstacle[] getObstaclesWithin(double distance,Vector2D position)
  {
   LinkedList<Obstacle> result = new LinkedList<Obstacle>();
   
     for (Obstacle obstacle: obstacles)
	 {
	    if (obstacle.distanceFrom(position)<distance)
		{
		   result.add(obstacle);
		}
	 }
	 
	 Obstacle[] resultArray = new Obstacle[result.size()];
	 
	   System.arraycopy(result.toArray(),0,resultArray,0,resultArray.length);
	 
	   return resultArray;
  }
  
  /**
  Sorts by the easiness of tracking each obstacle.  
  Easiest to track are put at index 0 of the array.
  Most difficult is put at the end.
  */
  public void sortByTrackability(Obstacle[] obstacles)
  {
     Arrays.sort(obstacles,new TrackabilityComparator(perspective));
  }
  
  public void sortByPylonObstacleAndDistanceFrom(Obstacle o,Obstacle[] obstacles)
  {
     Arrays.sort(obstacles,new DistanceFromPylonToObstacleComparator(o));
  }
  
  /**
  Adds the specified obstacle to this map
  */
  public synchronized void add(Obstacle o)
  {
  	   assert o.getLastMatchedTime()!=0;
    obstacles.add(o);
  }
  
  /**
  Returns a 3D vector containing the minimum components of all obstacle positions.
  If no obstacles are in this map, the returned vector will contain huge values for x,y, and z.
  */
  public synchronized Vector3D getMinRange()
  {
     Vector3D result = new Vector3D(perspective.getPosition());
   
  	for (Obstacle obstacle: obstacles)
	{
	   if (obstacle.getX()<result.getX())
	      result.setX(obstacle.getX());
		  
	   if (obstacle.getY()<result.getY())
	      result.setY(obstacle.getY());
		  
	   if (obstacle.getZ()<result.getZ())
	      result.setZ(obstacle.getZ());		  
	}
	
	return result;
  }
  
  /**
  Returns a 3D vector containing the maximum components of all obstacle positions.
  If no obstacles are in this map, the returned vector will contain very large but negative values for x,y, and z.
  */
  public synchronized Vector3D getMaxRange()
  {
   Vector3D result = new Vector3D(perspective.getPosition());
   
  	for (Obstacle obstacle: obstacles)
	{
	   if (obstacle.getX()>result.getX())
	      result.setX(obstacle.getX());
		  
	   if (obstacle.getY()>result.getY())
	      result.setY(obstacle.getY());
		  
	   if (obstacle.getZ()>result.getZ())
	      result.setZ(obstacle.getZ());		  
	}
	
	return result;
  }
  
  /**
  Paints all obstacles on the specified Graphics object, offsetted by offset and scaled by scale.
  @param g is the object painted on.
  @param offset is a vector representing how much to displace everything as it gets drawn
  @param scale is a factor for scaling coordinates of everything.  
    The smaller scale is, the more zoomed out the view is.  
	A scale of 1 means 1 pixel = 1cm.  A scale of 0.5 means 1 pixel = 2cm.
  */
  public synchronized void paint(Graphics g,Vector3D offset,double scale)
  {
     // loop through the obstacles.
	 for (Obstacle o: obstacles)
	 {
	    o.paint(g,offset,scale);
	 }
	 
	 for (Obstacle o: tempObstacles)
	   o.paint(g,offset,scale);
	 
	 perspective.paint(g,offset,scale);
  }
  
  /**
   Returns a description of all obstacles in this map.
  */
  public synchronized String toString()
  {
   StringBuffer result=new StringBuffer();
  
    // loop through obstacles.
	for (Obstacle obstacle: obstacles)
	{
	   result.append(obstacle.toString());
	}
	
	return result.toString();
  }
}

/**
Compares first by being a PylonObstacles before any other kind of Obstacle.
Compares second by distance.
*/
class DistanceFromPylonToObstacleComparator implements Comparator<Obstacle>
{
 private Obstacle fromObstacle;
 
   public DistanceFromPylonToObstacleComparator(Obstacle o)
   {
      this.fromObstacle=o;
   }
   
   public int compare(Obstacle o1,Obstacle o2) 
   {
      if (o1==fromObstacle)
	     return 1; // o2 is preferred.
	  if (o2==fromObstacle)
	     return -1; // o1 is preferred.
		 
      if ((o1 instanceof PylonObstacle) && !(o2 instanceof PylonObstacle))
	     return -1; // o1 is preferred.
	  else if ((o2 instanceof PylonObstacle) && !(o1 instanceof PylonObstacle))
	     return 1; // o2 is preferred.
		 
	 double d1=fromObstacle.distanceFrom(o1);
	 double d2=fromObstacle.distanceFrom(o2);
	 
	  if (d1<d2)
	     return -1;
	  else if (d1==d2)
	      return 0;
	  else
	      return 1;
   }
}

/**
 Compares how easily obstacles can be tracked
*/
class TrackabilityComparator implements Comparator<Obstacle> 
{
  public LidarSensorPerspective perspective;
  
  public TrackabilityComparator(LidarSensorPerspective perspective)
  {
    this.perspective=perspective;
  }

  public int compare(Obstacle o1,Obstacle o2)
  {
     // pylons are preferred over other types of obstacles.

     if ((o1 instanceof PylonObstacle)^(o2 instanceof PylonObstacle))
	 { // if 1 and only 1 of the obstacles is a pylon, prefer that pylon over the other one.
	 
	     if (o1 instanceof PylonObstacle)
		    return -1;
		 else
		    return 1;
	 }
	 
	 // the closer 1 is generally preferred.
	 return (int)(perspective.distanceFrom(o1)-perspective.distanceFrom(o2));
  }
  

}