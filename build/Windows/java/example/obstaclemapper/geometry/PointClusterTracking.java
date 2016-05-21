package geometry;

public class PointClusterTracking
{
/**
 Triangulates to find where rotationCentre maps to in an unknown transformation that involves both displacement and rotation.

 Returns null if the assumptions are found to be violated.  
 This can be caused by errors stemming from sensor data to other parts of the software.
 */
 public static Vector2D getTransformedRotationCentreFrom(
 Vector2D rotationCentre,Vector2D p1,Vector2D p2,Vector2D p3,
  Vector2D tp1,Vector2D tp2,Vector2D tp3)
  {
     double d1=p1.distanceFrom(rotationCentre);
	double d2=p2.distanceFrom(rotationCentre);
	double d3=p3.distanceFrom(rotationCentre);
	if (tp1.equals(tp2))
	{
	  return null;
	}
	Line2D linetP1TotP2 = new Line2D(tp1.getX(),tp1.getY(),tp2.getX()-tp1.getX(),tp2.getY()-tp1.getY());
	double D = tp1.distanceFrom(tp2);
	double distance = (d2*d2-d1*d1-D*D)/(2*D);
	Vector2D middleP = linetP1TotP2.getVector2DAtDistance(-distance);
	Line2D perpendicularLine = linetP1TotP2.getPerpendicularFrom(middleP.getX(),middleP.getY());
	double val = d1*d1-distance*distance;
	if (val<0)
	   return null;
	   
	 // distance from the middle of the perpendicular and the possible A points.
	double lenBetweenMiddlePAndPossibleAs = Math.sqrt(val);
	
	assert !(new Double(lenBetweenMiddlePAndPossibleAs).isNaN());
	
	 // calculate the 2 possible points for transformed rotationCentre.
  Vector2D tA1=perpendicularLine.getVector2DAtDistance(lenBetweenMiddlePAndPossibleAs);
  Vector2D tA2=perpendicularLine.getVector2DAtDistance(-lenBetweenMiddlePAndPossibleAs);

   // calculate distances from the possible transformed rotationCentre points to tp3.
   double td1=tA1.distanceFrom(tp3);
   double td2=tA2.distanceFrom(tp3);
  
    // which one has a distance from tp3 that is closest to the value of d3?
    if (Math.abs(td1-d3)<Math.abs(td2-d3))
       return tA1;
    else
        return tA2;
  }

  /**
  Returns a transformation that displaces and rotates and maps p1 to tp1, p2 to tp2, p3 to tp3 
  assuming the parameters are accurate and correspond with each other.
  
  For the most accurate results, the 3 points are separated by at least 10 degrees 
   from the centre of rotation.  This will help in triangulating the transformed rotation centre.
  
  @param p1 is a point that is assumed to be displaced and rotated to tp1.
  @param rotationCentre is the point that maps to the centre of rotation.  
    This would normally be the position of the lidar sensor before the transformation.
  */
  public static DisplaceAndRotateTransformation getDisplaceAndRotateTransformation(
   Vector2D rotationCentre,Vector2D p1,Vector2D p2,Vector2D p3,Vector2D tp1,Vector2D tp2,Vector2D tp3)
  {
    // what is the centre of rotation?
    Vector2D trotationCentre = getTransformedRotationCentreFrom(rotationCentre,p1,p2,p3,tp1,tp2,tp3);
	if (trotationCentre==null)
	   return null;
	   
	assert !(new Double(trotationCentre.getX()).isNaN());
	assert !(new Double(trotationCentre.getY()).isNaN());
			 
	// what is the angle difference from transformed p1(tp1) around transformed rotationCentre(trotationCentre)?
	double a1=Math.atan2(tp1.getY()-trotationCentre.getY(),
	  tp1.getX()-trotationCentre.getX());
		
	assert !(new Double(a1).isNaN());
			 
	// what is the old angle of p1 around rotationCentre?  
	double a2=Math.atan2(p1.getY()-rotationCentre.getY(),p1.getX()-rotationCentre.getX());
	
	     assert !(new Double(a2).isNaN());

		  // the rotation of the resulting transformation is derived from the difference.
	double angle=a1-a2;
	     
		assert !(new Double(angle).isNaN());
	 
 // cx,cy,x,y,angle in radians
  return new DisplaceAndRotateTransformation(trotationCentre.getX(),trotationCentre.getY(),
    trotationCentre.getX()-rotationCentre.getX(),
    trotationCentre.getY()-rotationCentre.getY(),angle);	
  }
  
}