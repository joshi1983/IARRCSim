#ifndef ACTIVE_VECTOR_2D
#define ACTIVE_VECTOR_2D

#include "MouseMotionListener.hpp"
#include "../vectors/Vector2D.hpp"

class ActiveVector2D
{
 private:
   MouseMotionListener *mlistener;
   Vector2D v;

 public:
   ActiveVector2D(double x,double y,MouseMotionListener *ml);   
   ~ActiveVector2D();

   double getX() const;
   double getY() const;
   MouseMotionListener *getMouseMotionListener() const;
   Vector2D getVector2D() const;
};

#endif