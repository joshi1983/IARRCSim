package geometry;

public class Vector3D
{
 private double x,y,z;
 
 public Vector3D(Vector3D v)
 {
   this(v.getX(),v.getY(),v.getZ());
 }
 
 public Vector3D(double x,double y,double z)
 {
   this.x=x;
   this.y=y;
   this.z=z;
 }
 
 public double getX()
 {
   return x;
 }
 
 public double getY()
 {
   return y;
 }
 
 public double getZ()
 {
   return z;
 }
 
 public void setX(double x)
 {
   this.x=x;
 }
 
 public void setY(double y)
 {
   this.y=y;
 }
 
 public void setZ(double z)
 {
    this.z=z;
 }
 
 public Vector3D plus(Vector3D v)
 {
   return new Vector3D(v.x+x,v.y+y,v.z+z);
 }
 
 public Vector3D minus(Vector3D v)
 {
   return new Vector3D(x-v.x,y-v.y,z-v.z);
 }
 
 public Vector3D times(double scalar)
 {
   return new Vector3D(x*scalar,y*scalar,z*scalar);
 }
 
 public double getMagnitude()
 {
    return Math.sqrt(x*x+y*y+z*z);
 }
 
 public void displaceXZBy(Vector2D v)
 {
    this.x+=v.getX();
	this.z+=v.getY();
 }
 
 public Vector2D getXZAsVector2D()
 {
   return new Vector2D(x,z);
 }
 
   public String toString()
  {
    return "("+x+", "+y+", "+z+")";
  }
}