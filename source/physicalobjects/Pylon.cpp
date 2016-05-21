
#include "Pylon.hpp"
#include "../shapes/Cone.hpp"
#include "../app/Application.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

#define RESOLUTION 10
#define PI 3.14159265358979
#define TOP_RATIO 0.1
#define Y_PADDING 0.1 // elevation of shadow

using namespace std;

double Pylon::global_height = 20;
double Pylon::global_radius = 7;

static const double ySizes[3]={TOP_RATIO,0.5,1};

bool Pylon::setGlobalHeight(double height)
{
  if (height<1)
  {
      cout << "Can't set height of pylons to "<<height
		 <<" or any value < 1."<<endl;

	  return false;
  }

  global_height = height;
  return true;
}

double Pylon::getGlobalHeight()
{
  return global_height;
}

bool Pylon::setGlobalRadius(double radius)
{
  if (radius<1)
  {
      cout << "Can't set radius of pylons to "<<radius
		 <<" or any value < 1."<<endl;

	  return false;
  }

  global_radius = radius;
  return true;
}

double Pylon::getGlobalRadius()
{
  return global_radius;
}

Pylon::Pylon(): InteractiveObject(0,0,0,50.0)
 ,brightness(0.99),height(global_height),radius(global_radius)
{
  updateRadiusOverHeight();
}

Pylon::Pylon(double x,double y,double z,double brightness):
 InteractiveObject(x,y,z,50.0),brightness(brightness)
	 ,height(global_height),radius(global_radius)
{
  updateRadiusOverHeight();
}

void Pylon::drawForDesign(int windowid)
{

  glPushMatrix();
  glTranslated(getX(),getZ(),0);

  glColor3d(1*brightness,0.5*brightness,0.1*brightness);
 double r=10;

    glBegin(GL_POLYGON);

     // loop through points.
    for (int i=1;i<=RESOLUTION;i++)
    {
	 double t=i*2*PI/RESOLUTION;
	 double x=r*cos(t),z=r*sin(t);

       glVertex2d(x,z);
    }

   glEnd();

    glPopMatrix();
}

double Pylon::getBrightness() const
{
  return this->brightness;
}

void Pylon::drawShadows(const Vector3D &lightDirection) const
{
  // draw shadow for plane y=0.
  // assume the color is already set to black or whatever is desired.
  // assume lightDirection is a unit vector.

  double shadowX = -lightDirection.getX()*height/lightDirection.getY();
  double shadowZ = -lightDirection.getZ()*height/lightDirection.getY();
  double dx=lightDirection.getX(),dz=lightDirection.getZ();
  double r=sqrt(dx*dx+dz*dz);
  double startX1 = -radius*lightDirection.getZ()/r;
  double startZ1 = radius*lightDirection.getX()/r;

  glPushMatrix();

  glTranslated(getX(),getY(),getZ());

  // draw a black triangle at y=0.5.
  glBegin(GL_TRIANGLES);

    glVertex3d(startX1,Y_PADDING,startZ1);
    glVertex3d(-startX1,Y_PADDING,-startZ1);
    glVertex3d(shadowX,Y_PADDING,shadowZ);

  glEnd();

  glPopMatrix();
}

void Pylon::draw(int windowid,const Vector3D &viewpoint)
{
 int resolution;
 bool highres=false;

  glDisable(GL_TEXTURE_2D);

  double distance=viewpoint.distanceFrom(position);
  if (distance>1000)
     resolution=4;
  else if (distance>600)
     resolution=8;
  else if (distance>300)
	  resolution=10;
  else if (distance>150)
  {
       resolution=12;
	  highres=true;
  }
  else
  {
	  resolution=16;
	  highres=true;
  }

   glPushMatrix();
   glTranslated(getX(),getY(),getZ());
   bool lightingEnabled=Application::isLightingEnabled();

    // Draw a square
    glBegin(GL_QUADS);

   double t=0;
   double t2=PI*2.0/resolution;

    glColor4d(1*brightness,0.5*brightness,0.1*brightness,0.999);
	double cos_t2=0;
	double sin_t2=0;
     double cos_t=cos(t);
	double sin_t=sin(t);

    if (lightingEnabled)
    {
      if (highres)
	 {
	 		 // loop through points.
		 for (int i=1;i<=resolution;i++)
		 {
			 cos_t2=cos(t2);
			 sin_t2=sin(t2);

			 for (int yIndex=0;yIndex<2;yIndex++)
			 {
				 double ySmall=height*(1-ySizes[yIndex]);
				 double rSmall=radius*ySizes[yIndex];
				 double yLarge=height*(1-ySizes[yIndex+1]);
				 double rLarge=radius*ySizes[yIndex+1];

				 double x1=rLarge*cos_t,z1=rLarge*sin_t;
				 double x2=rLarge*cos_t2,z2=rLarge*sin_t2;
				 double x3=rSmall*cos_t,z3=rSmall*sin_t;
				 double x4=rSmall*cos_t2,z4=rSmall*sin_t2;

				 glNormal3d(x1,radiusSqr_over_height*ySizes[yIndex+1],z1);
				 glVertex3d(x1, yLarge, z1); // bottom corner 1
				  glNormal3d(x1,radiusSqr_over_height*ySizes[yIndex+1],z1);
				 glVertex3d(x3, ySmall, z3); // top point 1

				 glNormal3d(x2,radiusSqr_over_height*ySizes[yIndex+1],z2);
				 glVertex3d(x4, ySmall, z4); // top point 2
				  glNormal3d(x2,radiusSqr_over_height*ySizes[yIndex+1],z2);
				 glVertex3d(x2, yLarge, z2); // bottom corner 2
			 }


			 t=t2;
			 cos_t=cos_t2;
			 sin_t=sin_t2;
			 t2=(i+1)*PI*2.0/resolution;
		 }
	     }
	     else // not high res.
	     {
			double rLarge=radius;
			double rSmall=TOP_RATIO*radius;
			double yLarge=0;
			double ySmall=(1-TOP_RATIO)*height;

	 		 // loop through points.
		 for (int i=1;i<=resolution;i++)
		 {
			 cos_t2=cos(t2);
			 sin_t2=sin(t2);

			 {
				 double x1=rLarge*cos_t,z1=rLarge*sin_t;
				 double x2=rLarge*cos_t2,z2=rLarge*sin_t2;
				 double x3=rSmall*cos_t,z3=rSmall*sin_t;
				 double x4=rSmall*cos_t2,z4=rSmall*sin_t2;

				 glNormal3d(x1,radiusSqr_over_height,z1);
				 glVertex3d(x1, yLarge, z1); // bottom corner 1
				 // glNormal3d(x1,radiusSqr_over_height,z1);
				 glVertex3d(x3, ySmall, z3); // top point 1

				 glNormal3d(x2,radiusSqr_over_height,z2);
				 glVertex3d(x4, ySmall, z4); // top point 2
				 // glNormal3d(x2,radiusSqr_over_height,z2);
				 glVertex3d(x2, yLarge, z2); // bottom corner 2
			 }

			 cos_t=cos_t2;
			 sin_t=sin_t2;

			 t=t2;
			 t2=(i+1)*PI*2.0/resolution;
		 }
	     }
	 }
	 else // no lighting
	 {
		 // loop through points.
		 for (int i=1;i<=resolution;i++)
		 {
			 double cos_t2=cos(t2);
			 double sin_t2=sin(t2);

			 double x1=radius*cos_t,z1=radius*sin_t;
			 double x2=radius*cos_t2,z2=radius*sin_t2;

			 glVertex3d(x1, 0, z1); // bottom corner 1
			 glVertex3d(TOP_RATIO*cos_t, height, TOP_RATIO*sin_t); // top point 1
			 glVertex3d(TOP_RATIO*cos_t2, height, TOP_RATIO*sin_t2); // top point 2
			 glVertex3d(x2, 0, z2); // bottom corner 2

			 t=t2;
			 cos_t=cos_t2;
			 sin_t=sin_t2;
			 t2=(i+1)*PI*2.0/resolution;
		 }
	 }

    glEnd();

    glPopMatrix();
}


void Pylon::writeTo(std::ostream &out) const
{
	InteractiveObject::writeTo(out);
	out << " " << brightness;
}

void Pylon::readFrom(std::istream &in)
{
   InteractiveObject::readFrom(in);
   in >> brightness;
   updateRadiusOverHeight();
}

void Pylon::setToRandomBrightness()
{
 double rVal=0.001*(rand()%1000);

  brightness=0.7+rVal*0.3;
}

std::string Pylon::getName() const
{
	return std::string("Pylon");
}

void Pylon::ticksPerformed(Environment * e,int milis)
{
 // InteractiveObject::ticksPerformed(e,milis);
}

Pylon::~Pylon()
{
}

Cone *Pylon::getAsCone() const
{
 Cone * cone = new Cone(Vector3D(position.getX(),
	 this->height+position.getY(),position.getZ()));

  cone->setHeight(this->height);
  cone->setSlope(radius_over_height);

  return cone;
}

void Pylon::addShapeRepresentation(ShapeCollection &shapes) const
{
 // add a Cone.
 Cone * cone = getAsCone();

  shapes.add(cone);
}

void Pylon::updateRadiusOverHeight()
{
  radius_over_height=radius/height;
  radiusSqr_over_height=radius*radius_over_height;
}
