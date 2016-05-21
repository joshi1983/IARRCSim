#ifndef TRANSLATE_OBJECT_WRAPPER_HEADER
#define TRANSLATE_OBJECT_WRAPPER_HEADER

#include "GCommand.hpp"
#include "../physicalobjects/PhysicalObject.hpp"
#include "../vectors/Vector3D.hpp"
#include <string>


class TranslateObjectWrapper: public GCommand
{
 private:
   Vector3D translation;
   PhysicalObject *c;
   std::string name;
   bool forHighResOnly;
   double xRotation; // rotation in degrees
   double yRotation; // rotation in degrees
   double zRotation;

 public:
   TranslateObjectWrapper(const Vector3D &v,PhysicalObject *c);

   bool isForHighResOnly() const;
   void setForHighRes(bool forHighRes);
   void setName(const std::string &name);
   const std::string &getName() const;

   virtual bool matchesName(const char *name) const;
   void setXRotation(double degrees);
   void setYRotation(double degrees);
   void setZRotation(double degrees);

   double getXRotation() const;
   double getYRotation() const;
   double getZRotation() const;
   virtual void draw(int windowid,const Vector3D &viewpoint);
   virtual void drawForDesign(int windowid);
   virtual void getAsLines(std::vector<LineSegment> &lines) const;

};

#endif
