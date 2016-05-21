#include "RepositionMouseMotionListener.hpp"

RepositionMouseMotionListener::RepositionMouseMotionListener(PhysicalObject *po): po(po)
{

}

void RepositionMouseMotionListener::mouseMoved(double x,double y)
{
  po->setX(x);
  po->setZ(y);
}

RepositionMouseMotionListener::~RepositionMouseMotionListener()
{

}

