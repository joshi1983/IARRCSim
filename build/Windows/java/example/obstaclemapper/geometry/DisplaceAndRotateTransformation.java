package geometry;

public class DisplaceAndRotateTransformation extends Transformation
{
 private Rotator2D rotator;
  
 private DisplaceTransformation displacement;
 private boolean rotateFirst;

  public DisplaceAndRotateTransformation(double cx,double cy,double dx,double dy,double rotationRadians)
  {
     assert !(new Double(cx).isNaN());
	 assert !(new Double(cy).isNaN());
     this.rotator = new Rotator2D(cx,cy,rotationRadians);
	 displacement = new DisplaceTransformation(dx,dy);
	 rotateFirst=true;
  }
  
  public DisplaceAndRotateTransformation(DisplaceTransformation displacement,Rotator2D rotator)
  {
     if (displacement==null || rotator==null)
        throw new NullPointerException("displacement and rotator must not be null.");
  
     this.rotator = rotator;
	 this.displacement = displacement;
  }
  
  public Transformation getInverse()
  {
    DisplaceAndRotateTransformation result=new DisplaceAndRotateTransformation(
	   (DisplaceTransformation)displacement.getInverse(),
	   (Rotator2D)rotator.getInverse());
	   
	   result.rotateFirst=!this.rotateFirst;
	   
	   return result;
  }
  
  public Vector2D transform(Vector2D v)
  {
    if (rotateFirst)
	{
     assert !(new Double(v.getX()).isNaN());
	 assert !(new Double(v.getY()).isNaN());
	 v=rotator.transform(v);
	 }
	 else
	 {
	   v=displacement.transform(v);
	 }
     assert !(new Double(v.getX()).isNaN());
	 assert !(new Double(v.getY()).isNaN());	 
     if (rotateFirst)
	    v=displacement.transform(v);
	 else
	    v=rotator.transform(v);
		
     assert !(new Double(v.getX()).isNaN());
	 assert !(new Double(v.getY()).isNaN());	 
	 return v;
  }
 
  public Vector2D getDisplacementVector2D()
  {
    return displacement.getDisplacementVector2D();
  }
 
  public double getRotationRadians()
  {
    return rotator.getRotationAngleRadians();
  }

  public boolean isIdentity()
  {
    return rotator.isIdentity()&&displacement.isIdentity();
  }
  
  public String toString()
  {
    return "transforming by displacement and rotation: displacement="
	 +getDisplacementVector2D()+", rotation: "+getRotationRadians()+" radians";
  }
  
  public boolean equals(Object o)
  {
     if (!(o instanceof DisplaceAndRotateTransformation))
	    return false;
	 else
	 {
	    DisplaceAndRotateTransformation t = (DisplaceAndRotateTransformation)o;
		
		return t.rotator.equals(this.rotator) && t.displacement.equals(this.displacement);
	 }
  
  }
}