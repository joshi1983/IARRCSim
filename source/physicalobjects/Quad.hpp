#ifndef QUAD_HEADER
#define QUAD_HEADER

#include "../vectors/Vector3D.hpp"
#include "GCommand.hpp"

class Quad: public GCommand
{
 private:
  Vector3D points[4];
  Vector3D normal; // used for lighting

  void updateNormal();
 public:
  Quad(const Vector3D& p1,const Vector3D& p2,const Vector3D& p3,const Vector3D& p4);
  virtual void draw(int windowid,const Vector3D &viewpoint);
  virtual void drawForDesign(int windowid);
  void negateNormal();

  virtual void getAsLines(std::vector<LineSegment> &lines) const;
};


#endif
