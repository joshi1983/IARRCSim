#ifndef PERSPECTIVE_VIEW_TRANSFORMER_HEADER
#define PERSPECTIVE_VIEW_TRANSFORMER_HEADER

#include "ViewTransformer.hpp"
#include "../graphics/Rotator2D.hpp"

class PerspectiveViewTransformer: public ViewTransformer
{
 private:
   Rotator2D yAxisRotator; // horizontal aim
   Rotator2D xAxisRotator; // vertical aim
   Vector3D viewpoint;

   double distance;

   void updateViewPoint();

 public:
   PerspectiveViewTransformer();

   /** 
    Updates viewpoint accordingly.
   */
   void setDistance(double distance);

   /**
    Updates viewpoint and yAxisRotator accordingly
   */
   void setHorizontalRotation(double radians);

   double getHorizontalRotation() const;

   /**
    Updates viewpoint and xAxisRotator accordingly
   */
   void setVerticalRotation(double radians);

   double getVerticalRotation() const;

   virtual bool transform(
	   const Vector3D &v3d,int cx,int cy
	   ,int &x,int &y) const;
};

#endif