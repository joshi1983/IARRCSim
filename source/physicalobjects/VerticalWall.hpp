#ifndef VERTICAL_WALL_HEADER
#define VERTICAL_WALL_HEADER

#include "../shapes/VerticalRectangle.hpp"
#include "PhysicalObject.hpp"
#include "Rotatable.hpp"

class VerticalWall: public PhysicalObject, public Rotatable
{
 private:
   double height,length;

 public:
    VerticalWall();
    void setHeight(double height);
    double getHeight() const;
    double getLength() const;
    void setLength(double len);

    /**
    Sets length, position, rotation... such that the
    specified coordinates fit on the ends of this wall.
    */
    void setEndPoints(double x1,double z1,double x2,double z2);

    Vector2D getEndPoint1XZ() const;
    Vector2D getEndPoint2XZ() const;
    void setEndPoint1XZ(const Vector2D & p);
    void setEndPoint2XZ(const Vector2D & p);

    virtual void setProperty(const std::string &propertyName,
                             const std::string & value);

    virtual std::string getName() const;
    virtual void addShapeRepresentation(ShapeCollection &shapes) const;
    virtual void draw(int windowid,const Vector3D &viewpoint);
    virtual void drawForDesign(int windowid);
	virtual void writeTo(std::ostream & out) const;
	virtual void readFrom(std::istream &in);

	virtual void getActiveXZPointsInRange(
		double x,double z,double maxDistance,
		std::list<ActiveVector2D> &results);

    // so the self driving cars can detect it and drive around
    virtual void addObstaclePoints(std::list<Vector2D> &points) const;
    void transform(Vector3D &v) const;

    static void loadTextures();
};


#endif
