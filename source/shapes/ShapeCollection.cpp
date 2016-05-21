#include "../stdafx.h"
#include "ShapeCollection.hpp"
#include <cmath>
using namespace std;

ShapeCollection::~ShapeCollection()
{
  clear();
}

void ShapeCollection::clear()
{
 list<Shape*>::iterator it = shapes.begin();

   for (;it!=shapes.end();it++)
   {
      delete (*it);
   }

   shapes.clear();
}

void ShapeCollection::add(Shape *s)
{
   shapes.push_back(s);
}

double ShapeCollection::getNearestPositiveIntersection(const Ray3D &ray,double minDepth,double maxDepth) const
{
  list<Shape*>::const_iterator it = shapes.begin();
  double result=99999999;

   for (;it!=shapes.end();it++)
   {
      double val=(*it)->getNearestPositiveIntersection(ray,minDepth,maxDepth);
      if ( result<0)
         result=val;
      else if (val>0 && val<result)
         result=val;
   }

  return result;
}

void ShapeCollection::getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,int numRays,double *results)
{
 Vector3D direction;

 double radianAngle=PI/2-degreesToScan*PI/180*0.5;
 double angleChange=degreesToScan*PI/180/numRays;
 Vector3D *directions=new Vector3D[numRays];

 // loop through rays.
 for (int i=0;i<numRays;i++)
 {
   radianAngle+=angleChange;
   double coord1=cos(radianAngle),coord2=sin(radianAngle);
   direction=axis1*coord1;
   direction.add(axis2*coord2);

   directions[i]=direction;
  // cout <<"ShapeCollection: i = "<<i<<", ";
  // directions[i].writeTo(cout);
  // cout << endl;

   results[i]=maxDepth; // initialize to max depth.
 }
 list<Shape*>::const_iterator it = shapes.begin();

 // loop through shapes.
 for (;it!=shapes.end();it++)
 {
	 (*it)->getDepthsOnPlane(pos,axis1,axis2,minDepth,
		 maxDepth,degreesToScan,directions,numRays,results);
 }

 delete [] directions;
}

void ShapeCollection::removeShapesNotConsideredWithin(
	const Vector3D &pos,double maxDepth)
{
	list<Shape*>::iterator it=shapes.begin();
     list<Shape*> toRemove;

	for (;it!=shapes.end();it++)
	{
		if (!(*it)->consideredWithin(pos,maxDepth))
		{
			Shape *s=(*it);
			toRemove.push_back(s);
		}
	}
     it=toRemove.begin();

	for (;it!=toRemove.end();it++)
	{
		Shape *s=(*it);
		shapes.remove(s);
	     delete s;
	}
}

int ShapeCollection::size() const
{
  return shapes.size();
}

