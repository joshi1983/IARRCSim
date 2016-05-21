#ifndef PATH_FINDER_HEADER
#define PATH_FINDER_HEADER

#include "../vectors/Vector2D.hpp"
#include <list>
#include <vector>

class PathFinder
{
 public:

   /**
    Creates a distance map on a grid.  This can be useful in obstacle avoidance.
   */
   static void createDistanceMap(const std::list<Vector2D> &points
	   ,double scaleFactor,std::vector< std::vector<int> > &map);

   /**
    Finds a good direction to drive in.
   */
   static void findDirectionAndSpeed(std::list<Vector2D> &points,
	   Vector2D &position,double &curDirection,double &speed);

};

#endif

