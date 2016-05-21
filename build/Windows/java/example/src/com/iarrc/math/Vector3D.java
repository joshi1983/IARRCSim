package com.iarrc.math;

/**
 Each instance of Vector3D represents a vector with 3 components.
 
 It can be used to represent points or directions in R3.
*/
public class Vector3D
{
 private double x;
 private double y;
 private double z;
 
 public Vector3D()
 {
   this(0,0,0);
 }
 
 public Vector3D(double x,double y,double z)
 {
   setX(x);
   setY(y);
   setZ(z);
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
 
 public void setX(double newX)
 {
   this.x=newX;
 }

 public void setY(double newY)
 {
   this.y=newY;
 }
 
 public void setZ(double newZ)
 {
   this.z=newZ;
 }
 
 public void setProperty(String propertyName,String value)
 {
  double dVal = Double.parseDouble(value);
  
    if (propertyName.endsWith("x"))
        setX(dVal);
	else if (propertyName.endsWith("y"))
	    setY(dVal);
	else if (propertyName.endsWith("z"))
	   setZ(dVal);
	else
	    throw new IllegalArgumentException("Invalid vector property: '"+propertyName
		 +"' must end with x,y, or z.");
 }
}