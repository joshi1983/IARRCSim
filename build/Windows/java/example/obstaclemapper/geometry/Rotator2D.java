package geometry;

public class Rotator2D extends Transformation
{
 private double rotationAngle;
 private double sin_a;
 private double cos_a;
 
 /**
  The centre of rotation
 */
 private double cx=0,cy=0;
 
 /**
   Initializes a Rotator that rotates the specified number of radians around origin(0,0).
 */
  public Rotator2D(double radians)
  {
    this(0,0,radians);
  }
  
  public Rotator2D(double cx,double cy,double radians)
  {
     assert !(new Double(cx).isNaN());
	 assert !(new Double(cy).isNaN());
	 
    this.cx=cx;
	this.cy=cy;
    setRotationRadians(radians);
  }
 
 /**
 Sets rotation to a value between 0 and 2 pi
 */
  public void setRotationRadians(double radians)
  {
     // let's make radians be in the range from -2 pi to 2 pi.
     if (Math.abs(radians)>=Math.PI*2) // if it cycles around the circle, remove the extra cycles.
	    radians-=((int)(radians/(2*Math.PI)))*2*Math.PI;
	if (radians<0)
       radians+=Math.PI*2;
	
		
	 assert radians>=0;
	 assert radians<=Math.PI*2;
	 
     this.rotationAngle=radians;
     sin_a = Math.sin(radians);
	 cos_a = Math.cos(radians);
  }
  
  public double getRotationAngleRadians()
  {
    return rotationAngle;
  }
 
  public Vector2D transform(Vector2D v)
  {
     assert !(new Double(cx).isNaN());
	 assert !(new Double(cy).isNaN());
	 
    double vx = v.getX()-cx;
	double vy = v.getY()-cy;
	
	 assert !(new Double(vx).isNaN());
	 assert !(new Double(vy).isNaN());
    double 
	 x=cx+(vx*cos_a-vy*sin_a),
	 y=cy+(vx*sin_a+vy*cos_a);
	
	     assert !(new Double(x).isNaN());
	 assert !(new Double(y).isNaN());
	 return new Vector2D(x,y);
  }
  
  public boolean isIdentity()
  {
    return rotationAngle==0;
  }

  public Transformation getInverse()
  {
    return new Rotator2D(cx,cy,-rotationAngle);
  }
  
  public boolean equals(Object o)
  {
     if (!(o instanceof Rotator2D))
	    return false;
	Rotator2D r = (Rotator2D)o;
	
	 return r.rotationAngle==this.rotationAngle;
  }
}