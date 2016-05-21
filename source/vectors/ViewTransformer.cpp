#include "ViewTransformer.hpp"

#include <iostream>

using namespace std;

ViewTransformer::ViewTransformer(double zoom): zoom(zoom)
{

}

void ViewTransformer::setZoom(double zoom)
{
  this->zoom=zoom;
}

bool ViewTransformer::transform(
	   const Vector3D &v3d,int left,int top,int width,int height
	   ,int &x,int &y) const
{
	cout << "ViewTransformer::transform called."<<endl;
  return transform(v3d,left+width/2,top+height/2,x,y);
}