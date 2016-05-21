#include "ConeTester.hpp"

#include <iostream>
#include <cmath>

#define TOLERANCE 0.0001

using namespace std;

void ConeTester::runTestCases()
{
 Vector3D position(0,0,0);
 Vector3D direction(0,0,1);
 Vector3D cPos(0,20,0);
 Ray3D r(position,direction);
 Cone c(cPos);

    c.setSlope(1);
    c.setHeight(20);

 double depth=  c.getNearestPositiveIntersection(r,0,100);
  
  if (abs(depth-20)<TOLERANCE)
     cout << "Cone test case 1 failed.  answer should be 20 but is " <<depth<<endl;

  r.setDirection(Vector3D(0,1,0));
  depth=  c.getNearestPositiveIntersection(r,0,100);
  if (abs(depth-20)>TOLERANCE)
     cout << "Cone test case 2 failed. should be 20 but is " <<depth<<endl;

  position.set(0,0,-50);
  direction.set(0,0,1);
  r.setPosition(position);
  r.setDirection(direction);
  depth=  c.getNearestPositiveIntersection(r,0,100);

  if (abs(depth-30)>TOLERANCE)
     cout << "Cone test case 3 failed. should be 30 but is " <<depth<<endl;

}
