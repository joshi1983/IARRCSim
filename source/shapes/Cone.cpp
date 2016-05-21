#include "Cone.hpp"

#include "../stdafx.h"
#include <iostream>
#include <cmath>

using namespace std;

Cone::Cone(const Vector3D& position): position(position),height(15)
{
	setSlope(1);
}

void Cone::setHeight(double height)
{
  if (height<0.001)
  {
	cout << "Can't set Cone height to value,"
		<<height<<", which is less than 0.001." << endl;
	return;
  }
  this->height=height;
  updateMaxDistanceFromPosition();
}

void Cone::setSlope(double slope)
{
  this->slope=slope;
  this->slope_sqr=slope*slope;
  updateMaxDistanceFromPosition();
}


void Cone::updateMaxDistanceFromPosition()
{
 double v=(height*slope);

   v=v*v;
   maxDistanceFromPosition=sqrt(height*height+v*v);
}

bool Cone::consideredWithin(const Vector3D &pos,double distance) const
{
     if (position.distanceFrom(pos)>distance+maxDistanceFromPosition)
	    return false;
	else
	    return true;
}

double Cone::getNearestPositiveIntersection(const Ray3D &ray,double minDepth,double maxDepth) const
{
  Vector3D pos(ray.getPosition());
     pos.sub(position);

     /*
	solve the quadratic equation for smallest positive solution
	that intersects the cone at a negative y.
	*/

	// components of the quadratic formula.
	double a,b,c;

	Vector3D dir(ray.getDirection());
	double dir_x=dir.getX();
	double dir_y=dir.getY();
	double dir_z=dir.getZ();
	double pos_x=pos.getX();
	double pos_y=pos.getY();
	double pos_z=pos.getZ();

     a=dir_x*dir_x+dir_z*dir_z-slope_sqr*dir_y*dir_y;
     b=2*(pos_x*dir_x+pos_z*dir_z-slope_sqr*pos_y*dir_y);
	c=pos_x*pos_x-slope_sqr*pos_y*pos_y+pos_z*pos_z;

	double determinant=b*b-4*a*c;

	//cout << "Cone height: "<<height<<endl;
	//cout << "Cone position: "<<endl;
	//position.writeTo(cout);

     if (determinant<0) // no solutions
        return -1;
	else
	{
		if (a==0)
		{ // graphing a straight line instead of parabola.
			if (b==0)
				return -1;
			else
			    return -c/b;
		}

	 if (determinant==0) // only 1 solution.
         return -b/(2*a);
	 else // 2 solutions.  check for most desirable one.
	 {
	   //cout << "positive determinant so 2 solutions."<<endl;

	   double a_times_2=a*2;
	   determinant=sqrt(determinant);
	   double val1=(-b-determinant)/a_times_2;
	   bool useOther=val1<0;

        if (!useOther)
	   {
		   double y=pos.getY()+val1*ray.getDirection().getY();
		   if (y>0 || y<-height)
			   // intersects outside of used part of cone
			   useOther=true;
		   else
		   { // assuming can't be negative.

                if (a>=0)
			    return val1;
			 else
			 { // see if the other solution is better.
			    double val2=(-b+determinant)/a_times_2;
			    if (val2<0)
				    return val1;
			    else
			    {
			       y=pos.getY()+val2*ray.getDirection().getY();
				  if (y>0 || y<-height)
					  // intersects outside used part of cone
					  return val1;
				  else
					  return val2;
			    }
			 }

		   }
	   }
	   if (useOther)
	   {
		double val1=(-b+determinant)/a_times_2;

	     double y=pos.getY()+val1*ray.getDirection().getY();
		if (y<-height || y>0)
			return -1;
		else
	         return (-b+determinant)/a_times_2;
	   }
	 }
	}

	return -1;
}

void Cone::getDirectionRange(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2,double degreesToScan,int numRays,int &minIndex,int &maxIndex)
{
	/*
double Px=pos.getX();
double Py=pos.getY();
double Pz=pos.getZ();

double c=Px*Px-slope_sqr*Py*Py+Pz*Pz;

if (abs(c)<0.0001)
{ // avoid division by 0.

  minIndex=0;
  maxIndex=numRays-1;

  cout << "c is too close to 0."<<endl;
  return;
}

double A1x=axis1.getX();
double A1y=axis1.getY();
double A1z=axis1.getZ();
double A2x=axis2.getX();
double A2y=axis2.getY();
double A2z=axis2.getZ();



double E=Px*A1x+Pz*A1z-slope_sqr*Py*A1y;
double F=Px*A2x+Pz*A2z-slope_sqr*Py*A2y;

double A=E*E+c*(-A1x*A1x-A1z*A1z+slope_sqr*A1y*A1y);
double B=2*E*F-2*c*(A1x*A2x+A1z*A2z-slope_sqr*A1y*A1y);
double C=F*F-c*(A2x*A2x+A2z*A2z-slope_sqr*A2y*A2y);

double discriminant=B*B-4*A*C;

  if (discriminant<=0)
  { // this would be weird.
    minIndex=0;
    maxIndex=numRays-1;

    cout << "discriminant is negative: "<<discriminant<<endl;

    // empty interval.
    return;
  }
  discriminant=sqrt(discriminant);

  double s1=(-B+discriminant)/(2*A);
  double s2=(-B-discriminant)/(2*A);

  // get as radian angles
  double a1=atan2(s1,1);
  double a2=atan2(s2,1);

  a1*=180/PI;
  a2*=180/PI;
  // now a1 and a2 are in degrees.

  cout << "a1="<<a1<<", s1="<<s1<<endl;
  cout << "a2="<<a2<<", s2="<<s2<<endl;

*/
  minIndex=0;
  maxIndex=numRays-1;
}

void Cone::getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,const Vector3D *directions,int numRays,double *results)
{
 Ray3D r(pos,axis1);
 int minIndex,maxIndex;

  getDirectionRange(pos,axis1,axis2,degreesToScan,numRays,minIndex,maxIndex);

  for (int i=minIndex;i<=maxIndex;i++)
  {
    r.setDirection(directions[i]);
    double depth=getNearestPositiveIntersection(r,minDepth,maxDepth);
    if (depth>0 && depth<results[i])
	    results[i]=depth;
  }
}

void Cone::setPosition(const Vector3D & pos)
{
  position=pos;
}

const Vector3D& Cone::getPosition() const
{
  return position;
}

void Cone::translateBy(const Vector3D &v)
{
  position.add(v);
}
