package geometry;



public class DisplaceTransformation extends Transformation
{
 private double dx,dy;

  public DisplaceTransformation(double dx,double dy)
  {
    this.dx=dx;
	this.dy=dy;
  }
 
  public Vector2D getDisplacementVector2D()
  {
    return new Vector2D(dx,dy);
  }
 
  public Vector2D transform(Vector2D v)
  {
     return new Vector2D(v.getX()+dx,v.getY()+dy);
  }
  
  public boolean isIdentity()
  {
     return dx==0 && dy==0;
  }
  
  public Transformation getInverse()
  {
    return new DisplaceTransformation(-dx,-dy);
  }
  
  public boolean equals(Object o)
  {
    if (!(o instanceof DisplaceTransformation))
        return false;
	else
	{
	   DisplaceTransformation t=(DisplaceTransformation)o;
	   return t.dx==this.dx && t.dy==this.dy;
	
	}
  }
}