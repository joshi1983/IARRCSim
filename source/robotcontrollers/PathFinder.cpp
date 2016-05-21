#include "PathFinder.hpp"

#include "../stdafx.h"
#include "../graphics/BitmapImage.hpp"
#include "../vectors/Vector2DInt.hpp"
#include <cmath>
#include <list>

using namespace std;

const Vector2DInt offset_points[]={
 Vector2DInt(-1,0),Vector2DInt(-1,1),Vector2DInt(0,1),Vector2DInt(1,1),Vector2DInt(1,0)};

 const int NUM_OFFSET_POINTS=5;

/**
 Creates a distance map on a grid.  This can be useful in obstacle avoidance.

 @param map is the resulting map or grid to hold the distance measurements.
 The map must contain a consistant length of inner vector.  
 The grid should form a smooth rectangular shape as would be expected.
  In other words map[x].size()==map[x2].size() for every x,x2 between 0 and map.size().

 */
void PathFinder::createDistanceMap(const std::list<Vector2D> &points,double scaleFactor,
 vector<vector<int> > &map)
{
  list<Vector2D>::const_iterator it = points.begin();
  unsigned int width=map.size();
  if (width<1)
	  return;

  unsigned int height=map[0].size();

    for (;it!=points.end();it++)
    {
	    double cx = it->getX();
	    double cy = it->getY();

      // loop through points in the grid.
	    for (int x=0;(unsigned int)x<width;x++)
	    {
		 if (map[x].size()<height)
			 return;

	      for (int y=0;(unsigned int)y<height;y++)
		 {
		   double dx=(x*scaleFactor-cx);
		   double dy=(-y*scaleFactor-cy);
		   double distance=sqrt(dx*dx+dy*dy);

		   if (distance<map[x][y])
                 map[x][y]=(int)distance;
		 }
	    }
    }
}

   /**
    Finds a good direction to drive in.
   */
void PathFinder::findDirectionAndSpeed(std::list<Vector2D> &points,
			Vector2D &position,double &curDirection,double &speed)
{
  vector< vector<int> > map;
  int width=10;
  int height=6;

  /**
  Initialize all cells to huge numbers.
  */
  for (int I=0;I<width;I++)
  {
   vector<int> v;

     for (int i=0;i<height;i++)
	   v.push_back(9999);

     map.push_back(v);
  }

  double dir=curDirection * PI/180;
  // convert from degrees to radians
 // dir+=PI;
  dir=PI-dir;

  double cos_dir=cos(dir);
  double sin_dir=sin(dir);
  int scale=15;
  int x_offset=width*scale/2;

  list<Vector2D>::iterator it = points.begin();

  // translate and rotate the points.
  for (;it!=points.end();it++)
  {
    double px=it->getX()-position.getX(),py=it->getY()-position.getY();

/*
See http://en.wikipedia.org/wiki/Rotation_%28mathematics%29#Matrix_algebra
for the math involved in the rotation.
*/
    it->set(px*cos_dir+py*sin_dir+x_offset,
	    py*cos_dir-px*sin_dir);
  }
  
  createDistanceMap(points,scale,map);

  // find a nearby path of maximum distance

  speed=3;

//  list<Vector2D> pathPoints;
  int pos_x=(width/2);
  int pos_y=0;
  int best_offset_index;
  double maxNext;
  // loop through path.
  for (int i=width*2;i>0;i--)
  {
     maxNext=-1;
	best_offset_index=0;
     // loop through offset points.
	for (int offset_index=0;offset_index<NUM_OFFSET_POINTS;offset_index++)
     {
	  int x=pos_x+offset_points[offset_index].x,y=pos_y+offset_points[offset_index].y;
	  if (x<0 || y<0 || x>=width || y>=height)
		  continue;

	  int val=map[x][y];

	  if (val>=0 && val>maxNext)
	  {
         best_offset_index=offset_index;
	    maxNext=val;
	  }
	}
	// move to the best option.
	{
		int x=pos_x+offset_points[best_offset_index].x,
			y=pos_y+offset_points[best_offset_index].y;

		// make the move.
		pos_x=x;
		pos_y=y;

		map[x][y]=-1; // mark as visited.
	}
	if (pos_x==0 || pos_y>=height-1 || pos_x>=width-1)
	{ // done.
	  break;
	}
  }

  // get a steering direction offset by aiming toward the desired path.
  // to find an angle, get a point on the path along a rectangular arc.
  {
   int path_x=-1;
   int path_y=-1;
   int edge_y=height-1;

    // loop over front edge
    for (int i=0;i<width;i++)
    {
       if (map[i][edge_y]<0)
	  {
	     path_x=i;
		path_y=edge_y;
	     break;
	  }
    }
    if (path_x<0) // if not found yet
    {
      // loop down sides.
      for (int i=edge_y;i>=0;i--)
	 {
	  if (map[0][i]<0)
	  {
	     path_x=0;
		path_y=i;
	     break;
	  } 
	  else if (map[width-1][i]<0)
	  {
	  	path_x=width-1;
		path_y=i;
	     break;
	  }
	 }
    }
    if (path_x<0 || path_y<0)
       speed=1; // go very slowly.
    else
    {
	  int dx=path_x-width/2;
       double angle=atan2(((double)dx),(double)path_y);

	    angle*=-180/PI;
	
	    curDirection+=angle;
	    speed=abs(angle*0.2);
	    if (speed>25)
		    speed=25;

	    speed=27-speed;
	    if (speed>10)
		    speed=10;

	    map[path_x][path_y]=-2;
    }
  }

/*
   // convert to bitmap image and save.
  BitmapImage img(width,height);

  for (int y=0;y<height;y++)
  {
    for (int x=0;x<width;x++)
    {
	    if (map[x][y]==-2)
	       img.setPixelRGB(x,y,0,255,0);
         else if (map[x][y]==-1)
	       img.setPixelRGB(x,y,255,0,0);
	    else if (map[x][y]<10)
		    img.setPixelRGB(x,y,255,255,255);
	    else
             img.setPixelRGB(x,y,(int)(map[x][y]/10),(int)(map[x][y]*5),
		     ((int)(map[x][y]*20))%255);
    }
  }  
  img.saveToBitmapFile("map.bmp");
*/

}
