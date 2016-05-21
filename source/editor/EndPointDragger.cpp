#include "EndPointDragger.hpp"

EndPointDragger::EndPointDragger(VerticalWall * wall,int pointNum): wall(wall),pointNum(pointNum)
{
    if (pointNum==0)
       otherP = wall->getEndPoint2XZ();
    else
       otherP = wall->getEndPoint1XZ();
}

void EndPointDragger::mouseMoved(double x,double y)
{
  if (pointNum==0)
     wall->setEndPoints(x,y,otherP.getX(),otherP.getY());
  else
     wall->setEndPoints(otherP.getX(),otherP.getY(),x,y);

}
