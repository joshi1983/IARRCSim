#ifndef GCOMMAND_HEADER
#define GCOMMAND_HEADER

#include "../vectors/Vector3D.hpp"
#include "LineSegment.hpp"
#include <vector>

class GCommand
{

 public:
   virtual bool matchesName(const char *name) const;

   virtual void draw(int windowid,const Vector3D &viewpoint)=0;
   virtual void drawForDesign(int windowid)=0;
   virtual void getAsLines(std::vector<LineSegment> &lines) const;
};

#endif
