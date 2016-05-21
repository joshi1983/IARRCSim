package mapping;

import geometry.*;
import com.iarrc.roboticsio.*;
import java.awt.Color;
import java.util.ArrayList;

/**
A class handling the processing of sensor data to add more 
information to the map and update steering/speed for the car in the simulator.

*/
public class ProcessingCentre
{
/**
 Beyond this number of cm depth change from one scanned ray to the 
 next is assumed to imply the a change in obstacle being scanned.
*/
 private static final double DEPTH_CHANGE_THRESHOLD=5;
 
 /**
 How close does a group of detected points have to conform to the shape of a circle to be treated like one?
 */
 private static final double CIRCLE_ACCURACY_THRESHOLD=40;
 
 /**
   Gets an approximated circle based on the specified array of 2D points.
 */
  private static CircleApproximation getCircleApproximation(ArrayList<Vector2D> points)
  {
    if (points.size()<3)
	   return null;
	   
    int middleIndex = points.size()/2;
	
	Vector2D p1 = points.get(0); 
	 // get point on one side of the circle.
	 
	Vector2D p3 = points.get(points.size()-1); 
	 // get point on the other side of the circle.
	 
	Vector2D p2 = points.get(middleIndex); 
	 // get point in the middle of the scan through the circle.
	 
	 
	 Line2D bisector1=Line2D.getPerpendicularBisector(p1,p2);
	 Line2D bisector2=Line2D.getPerpendicularBisector(p1,p3);
	 Line2D bisector3=Line2D.getPerpendicularBisector(p2,p3);	

     Vector2D[] intersectionPoints= new Vector2D[]{
	   Line2D.getIntersectionPoint(bisector1,bisector2),
	   Line2D.getIntersectionPoint(bisector1,bisector3),
	   Line2D.getIntersectionPoint(bisector3,bisector2)
	   };
	 
	double x=0,y=0;
	int count=0;
	
	  for (Vector2D p: intersectionPoints)
	  {
	     if (p!=null)
		 {
		    x+=p.getX();
			y+=p.getY();
		    count++;
		 }
	  }
	  
	  if (count>0)
	  {
         x/=count;
		 y/=count;
		 double radius = 0;
		 for (Vector2D v: points)
		 {
		   radius+=v.distanceFrom(x,y);
		 }
		 radius/=count; // get average radius
		 //System.out.println("radius: "+radius);
		 
		 return new CircleApproximation(radius,x,y,0);
	  }
	  else
	  {
	  	   return new CircleApproximation(0,0,0,Double.MAX_VALUE);
		   // definitely not a circle
	  }
  }
  
  private static CircleApproximation getPylonApproximationForSmallPointCluster(ArrayList<Vector2D> points)
  {
  	Vector2D p1 = points.get(0); 
	 // get point on one side of the circle.
	 
	Vector2D p3 = points.get(points.size()-1); 
	double distance = p1.distanceFrom(p3);
	 // get point on the other side of the circle.
	 
	 if (distance>30) // huge for a pylon so it probably is not a pylon.
	 {
	    return new CircleApproximation(0,0,0,Double.MAX_VALUE);
	 }
	 else
	 {
	    /* 
		 Calculate the average intersection point to be more accurate and error-tolerant
		 than just using 1 intersection point.
		*/
	    double x=0,y=0;
		
	    for (Vector2D p: points)
	    {
		      x+=p.getX();
		   	  y+=p.getY();
		}
		
		x/=points.size();
		y/=points.size();
		
		return new CircleApproximation(distance/2,x,y,0);
	 }
  }
  
  /**
   Looks at the positions of a group of obstacles and determines 
  */
  private static CircleApproximation getPylonCircleApproximationFromObstacles(ArrayList<Obstacle> obstacles)
  {
  
    ArrayList<Vector2D> points = new ArrayList<Vector2D>();
	
	  for (Obstacle o: obstacles)
	      points.add(o.getPositionAsVector2D());
	
	  CircleApproximation circle= getCircleApproximation(points);
	  
	   if (circle.getAccuracy() + unlikelyPylonRadius(circle.getRadius())<CIRCLE_ACCURACY_THRESHOLD )
	      return circle;
	   else
	   {
	      // if the points are very close together, let's assume it is a pylon.
		  if (obstacles.size()<10000)
		     return getPylonApproximationForSmallPointCluster(points);
		  else 
		     return null;
	   }
	   
  }
 
 /**
   Returns 0 if the specified radius is typical for a pylon.
   Returns increasingly large numbers, the more unusual the radius is.
 */
  private static double unlikelyPylonRadius(double radius)
  {
     if (radius<0)
	    return 1000;
		
     if (radius<3) // unusually small. (0->60, 1->40, 2->20, 3->0)
	    return (3-radius)*20;
		
     if (radius<20) // normal size range
	    return 0;
	 
	 return (radius-20)*5; // unusually large
  }
 
  public static void processInit(IARRCGeneralInterfaceAdapter carinterface,LidarSensor lidar,ObstacleMap map)
  {
    LidarSensorPerspective perspective = map.getLidarSensorPerspective();
	perspective.setRotationRadians(lidar.getRangeScan()*Math.PI/180/2);
	perspective.setMaxAngleRange(lidar.getRangeScan());
	perspective.setMaxDepth(lidar.getMaxDepth());
  }
 
/**
Assumes there is a "lidar1" sensor in the middle of the front of the car oriented level 
with the ground and aiming in the direction the car is moving.
*/
  public static void process(IARRCGeneralInterfaceAdapter carinterface,LidarSensor lidar,
    ObstacleMap map,long time_ms) throws Exception
  {
     map.clearNonPylons();
  
     // get the lidar
	 double depths[] = lidar.getDepthsNoCache();
	 ObstacleMap curMap = new ObstacleMap();
	 
	 // get the number of degrees the lidar scans around.
	 double angleRange = lidar.getRangeScan();
	 double curDepth=-1000;
	 Color curColour = Color.WHITE;
	 ArrayList<Obstacle> curObstacleGroup = new ArrayList<Obstacle>();
	 
	 // loop through all depths.
	 for (int i=0;i<depths.length;i++)
	 {
	    if (depths[i]>0 && depths[i]<0.9*lidar.getMaxDepth())
		{
		   double r = depths[i];
		   boolean pylonDetected=false;
		   
		   if (Math.abs(r-curDepth)>DEPTH_CHANGE_THRESHOLD)
		   {
		      curColour = new Color((int)(Math.random()*255),
			    (int)(Math.random()*255),(int)(Math.random()*255));
				
			  // if curObstacleGroup approximates a circle, make a PylonObstacle out of it.
			  if (curObstacleGroup.size()>2)
			  { // must be at least 2 points in the object for it to be approximated to a circle.
			    // 4 is used instead of 2 in the condition to avoid some of the most erronious values for pylons.
			  
			    CircleApproximation circle = getPylonCircleApproximationFromObstacles(curObstacleGroup);
				if (circle!=null)
			      if (circle.getAccuracy() + unlikelyPylonRadius(circle.getRadius())<CIRCLE_ACCURACY_THRESHOLD )
			      {
			       PylonObstacle pylon = new PylonObstacle(circle.getCentreX(),0,circle.getCentreY());
		            pylon.setLastMatchedTime(time_ms);
				    curMap.add(pylon);
					pylonDetected=true;
			      }
			  }
			  if (!pylonDetected)
			  {
			     // add all the obstacles.
				 for (Obstacle o: curObstacleGroup)
				 {
				    o.setLastMatchedTime(time_ms);
				    curMap.add(o);
				 }
			  }
				
			  curObstacleGroup = new ArrayList<Obstacle>();
		   }
		   curDepth=r;
		   
		   // get the angle/direction of the current depth measurement in radians.
		   double angle = Math.PI/180*i*angleRange/depths.length;
		   double y=0;
		   double x=r*Math.cos(angle);
		   double z=r*Math.sin(angle);
		   
		   // add an obstacle to the map at calculated coordinates.
		   Obstacle o = new Obstacle(x,y,z);
		   o.setColour(curColour);
		   curObstacleGroup.add(o);
		   
		}
		else
		{
		   curDepth=-1000;
		}
		
	 } // loop through depths.
	 
	 // plot the points from lidar data as Obstacle objects in the map.
	 curMap.clearNonTrivialObstacles();
	 
     map.merge(curMap,time_ms);
	 
	 map.removeUnlikelyObstacles(depths,time_ms);
  }

}