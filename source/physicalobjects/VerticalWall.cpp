#include "VerticalWall.hpp"
#include "../app/Application.hpp"
#include "../graphics/Texture.hpp"


#include <cmath>
#include <sstream>

#include "../editor/EndPointDragger.hpp"

using namespace std;

static Texture *wallTexture=NULL;

void VerticalWall::loadTextures()
{
    if (wallTexture!=NULL)
        return;

  wallTexture = new Texture("images/wall.bmp");
}

VerticalWall::VerticalWall(): PhysicalObject(0,0,0), height(50),length(200)
{
   loadTextures();
}

void VerticalWall::setHeight(double height)
{
  this->height=height;
}

double VerticalWall::getHeight() const
{
  return height;
}

void VerticalWall::setLength(double length)
{
  this->length=length;
}

double VerticalWall::getLength() const
{
  return length;
}


void VerticalWall::addShapeRepresentation(ShapeCollection &shapes) const
{
  // add a new VerticalRectangle.
  // plot points at every 30cm along this wall.
   Vector3D pos(0,height/2,0);
    transform(pos);
   VerticalRectangle *wall=
	  new VerticalRectangle(pos,length,height,90+Rotatable::getRotation());

     shapes.add(wall);
}

void VerticalWall::drawForDesign(int windowid)
{
    glPushMatrix();

  glTranslated(getX(),getZ(),0);
  glRotated(-rotation,0,0,1);

 double width = 20;
  glColor3d(0.7,0.7,0.7);

  // draw a line.
  glBegin(GL_QUADS);

    glVertex2d(-length/2,-width/2);
    glVertex2d(-length/2,width/2);
    glVertex2d(length/2,width/2);
    glVertex2d(length/2,-width/2);

  glEnd();

  // draw some end points.
  glColor3d(0,0.6,0);

  glBegin(GL_QUADS);

    glVertex2d(-length/2-width,width);
    glVertex2d(-length/2-width,-width);
    glVertex2d(-length/2+width,-width);
    glVertex2d(-length/2+width,width);

    glVertex2d(length/2-width,width);
    glVertex2d(length/2-width,-width);
    glVertex2d(length/2+width,-width);
    glVertex2d(length/2+width,width);

  glEnd();


  glPopMatrix();

  //Rotatable::drawForDesign(windowid,getX(),getZ(),getUIRadius()*0.4);
}

void VerticalWall::draw(int windowid,const Vector3D &viewpoint)
{
    if (wallTexture!=NULL)
    {
              // draw the outer cover
      glColor3d(1,1,1);
       glEnable(GL_TEXTURE_2D);
       glBindTexture( GL_TEXTURE_2D, wallTexture->getOpenGLTextureName(windowid) );
    }
    else
    {
      // draw the outer cover
      glColor3d(0.7,0.7,0.7);
    }
  glPushMatrix();
  glTranslated(getX(),getY(),getZ());
  glRotated(rotation,0,1,0);

  bool lightingEnabled = Application::isLightingEnabled();


  glBegin(GL_QUADS);

  // side 1
    if (lightingEnabled)
	    glNormal3d(-1,0,0);

  glTexCoord2d(-length/2/height,0);
    glVertex3d(-length/2,0,0);

      glTexCoord2d(-length/2/height,1);
    glVertex3d(-length/2,height,0);

      glTexCoord2d(length/2/height,1);
    glVertex3d(length/2,height,0);

      glTexCoord2d(length/2/height,0);
    glVertex3d(length/2,0,0);

  glEnd();

  glPopMatrix();
}

std::string VerticalWall::getName() const
{
  return string("VerticalWall");
}

void VerticalWall::writeTo(std::ostream & out) const
{
	PhysicalObject::writeTo(out);
	out << " "<<rotation;
}

void VerticalWall::readFrom(std::istream &in)
{
   PhysicalObject::readFrom(in);
   double rot;
   in >> rot;
   Rotatable::setRotation(rot);
}

void VerticalWall::getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results)
{
	// add the end points.
	Vector2D p1=getEndPoint1XZ();
	Vector2D p2=getEndPoint2XZ();

    if (p1.getDistanceFrom(x,z)<maxDistance)
       results.push_back(ActiveVector2D(x,z,new EndPointDragger(this,0)));

    if (p2.getDistanceFrom(x,z)<maxDistance)
       results.push_back(ActiveVector2D(x,z,new EndPointDragger(this,1)));
}

void VerticalWall::addObstaclePoints(std::list<Vector2D> &points) const
{
   Vector2D dir(1,0);
   Rotator2D r(getRotationInRadians());

     r.rotate(dir);

     for (double d=0;d<length/2;d+=50)
     {
       Vector2D p1(getX(),getZ());
         p1.add(dir.getTimes(d));

        points.push_back(p1);

         p1.set(getX(),getZ());
         p1.add(dir.getTimes(-d));
        points.push_back(p1);
     }
}

void VerticalWall::transform(Vector3D &v) const
{
  // rotate first.
	Rotatable::rotator.rotateXZ(v);
	v.add(position);
}

void VerticalWall::setEndPoints(double x1,double z1,double x2,double z2)
{
  double dx=x2-x1;
  double dz=z2-z1;
  length = sqrt(dx*dx+dz*dz);
  double rot = -atan2(dz,dx)*180/PI;
  setX((x1+x2)/2);
  setZ((z1+z2)/2);

  setRotation(rot);
}

void VerticalWall::setProperty(const std::string &propertyName,
                             const std::string & value)
{
    if (propertyName=="coords")
    {
      // partition value on spaces.
      istringstream in(value);
       double x1,y1,x2,y2;
      // get x1 y1 x2 y2.
      in>>x1;
      in>>y1;
      in>>x2;
      in>>y2;
      setEndPoints(x1,y1,x2,y2);
    }
}

Vector2D VerticalWall::getEndPoint1XZ() const
{
 Vector2D dir(1,0);
 Rotator2D r(-getRotationInRadians());

    r.rotate(dir);
 Vector2D result(dir.getX()*length/2,dir.getY()*length/2);

   result.add(getX(),getZ());
   return result;
}

Vector2D VerticalWall::getEndPoint2XZ() const
{
 Vector2D dir(1,0);
 Rotator2D r(-getRotationInRadians());

    r.rotate(dir);
 Vector2D result(-dir.getX()*length/2,-dir.getY()*length/2);

   result.add(getX(),getZ());

   return result;
}

void VerticalWall::setEndPoint1XZ(const Vector2D & p1)
{
 Vector2D p2 = getEndPoint2XZ();

  setEndPoints(p1.getX(),p1.getY(),p2.getX(),p2.getY());
}

void VerticalWall::setEndPoint2XZ(const Vector2D & p2)
{
 Vector2D p1 = getEndPoint1XZ();

  setEndPoints(p1.getX(),p1.getY(),p2.getX(),p2.getY());
}

