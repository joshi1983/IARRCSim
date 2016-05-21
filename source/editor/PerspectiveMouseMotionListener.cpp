#include "PerspectiveMouseMotionListener.hpp"

using namespace std;


PerspectiveMouseMotionListener::PerspectiveMouseMotionListener(
int x,int y,PerspectiveViewTransformer * transformer): x(x),y(y),
 horizontal_radians(transformer->getHorizontalRotation()),
 vertical_radians(transformer->getVerticalRotation()),
 transformer(transformer)
{

}

void PerspectiveMouseMotionListener::mouseMoved(double x,double y)
{
 double dx=x- (this->x),dy=y- (this->y);

  transformer->setHorizontalRotation(dx*0.03+horizontal_radians);
  transformer->setVerticalRotation(dy*0.03+vertical_radians);
}



