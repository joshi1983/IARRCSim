#include "Rotator3DTester.hpp"
#define TOLERANCE 0.0001

#include "../stdafx.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void checkEquality(const Vector3D &v1,const Vector3D &v2,const string &msg)
{

    if (abs(v1.getX()-v2.getX())>TOLERANCE)
       cout << msg<<" failed x should be "<<v2.getZ()<<" but is "
	  <<v1.getX()<<"."<<endl;
    if (abs(v1.getY()-v2.getY())>TOLERANCE)
       cout << msg<<" failed y should be "<<v2.getY()
	  <<" but is "<<v1.getY()<<"."<<endl;
    if (abs(v1.getZ()-v2.getZ())>TOLERANCE)
       cout << msg<<" failed z should be "<<v2.getZ()
	  <<" but is "<<v1.getZ()<<"."<<endl;
}

void Rotator3DTester::runTestCases()
{
  Rotator3D r(0,1,0,0);
  Vector3D point(1,0,0);

   // no rotation.
    r.rotate(point);

   checkEquality(point,Vector3D(1,0,0),string("test 1"));

   // 90 degree rotation
   r.setRotation(PI/2);

   point.set(1,0,0);
   r.rotate(point);

   checkEquality(point,Vector3D(1,0,0),string("test 2"));   
   point.set(1,1,0);
   r.rotate(point);

   checkEquality(point,Vector3D(1,0,1),string("test 3"));

   point.set(1,0,1);
   r.rotate(point);
   checkEquality(point,Vector3D(1,-1,0),string("test 4"));

   point.set(1,1,0);
   r.setRotation(PI/4); // 45 degrees.
   r.rotate(point);
   checkEquality(point,Vector3D(1,sqrt(0.5),sqrt(0.5)),string("test 5"));

   r.setAxis(0,1,0);
   r.setRotation(PI/2); // 90 degrees.
   point.set(1,1,0);
   r.rotate(point);

   checkEquality(point,Vector3D(0,1,-1),string("test 6"));

}
