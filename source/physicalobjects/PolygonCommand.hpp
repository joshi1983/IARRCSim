#ifndef POLYGON_COMMAND_HEADER
#define POLYGON_COMMAND_HEADER

#include "../stdafx.h"
#include "../vectors/Vector3D.hpp"
#include "GCommand.hpp"
#include <list>


class PolygonCommand: public GCommand
{
 private:
   std::list<Vector3D> vertices;
   Vector3D normal; // for lighting

   double minX,maxX;
   double minY,maxY;
     // for texture coordinates.

   double mapX(double x) const;
   double mapY(double y) const;

   void updateNormal();

 public:
   void updateRanges();
   void add(Vector3D v);
   virtual void draw(int windowid,const Vector3D &viewpoint);// draws using OpenGL functions.
   virtual void drawForDesign(int windowid);
   virtual void getAsLines(std::vector<LineSegment> &lines) const;
};

#endif
