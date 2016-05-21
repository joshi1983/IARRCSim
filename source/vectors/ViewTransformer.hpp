#ifndef VIEW_TRANSFORMER_HEADER
#define VIEW_TRANSFORMER_HEADER

#include "Vector3D.hpp"

class ViewTransformer
{
 protected:
   double zoom;

 public:
   ViewTransformer(double zoom=1.0);
   void setZoom(double zoom);
   virtual bool transform(
	   const Vector3D &v3d,int left,int top,int width,int height
	   ,int &x,int &y) const;

   virtual bool transform(
	   const Vector3D &v3d,int cx,int cy
	   ,int &x,int &y) const=0;

};

#endif