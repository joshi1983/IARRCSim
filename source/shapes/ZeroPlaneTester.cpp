#include "ZeroPlaneTester.hpp"
#include <iostream>
#include <cmath>

#define TOLERANCE 0.0001

using namespace std;

void ZeroPlaneTester::runTestCases()
{
 Vector3D position(0,0,0);
 Vector3D direction(0,0,1);
 Ray3D r(position,direction);
 ZeroPlane p;

 double depth=  p.getNearestPositiveIntersection(r,0,100);
  
  if (depth>TOLERANCE)
     cout << "ZeroPlane test case 1 failed.  answer should be at most 0 but is " 
	<<depth<<endl;

  position.set(0,10,0);
  direction.set(0,1,0);
  r.setPosition(position);
  r.setDirection(direction);
  depth=  p.getNearestPositiveIntersection(r,0,100);

  if (abs(depth-10)>TOLERANCE)
     cout << "ZeroPlane test case 2 failed." <<endl;

}
