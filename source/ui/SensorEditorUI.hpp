#ifndef SENSOR_EDITOR_UI_HEADER
#define SENSOR_EDITOR_UI_HEADER


#include <list>
#include "UserInterface.hpp"
#include "../sensors/Sensor.hpp"
#include "../sensors/UltrasonicSensor.hpp"
#include "../editor/SensorRepositionMouseListener.hpp"
#include "../vectors/PerspectiveViewTransformer.hpp"

class SensorEditorUI: public UserInterface
{
private:
   static double guide_height;
   static double guide_width;
   static double guide_length;

   static double zoom;
   static PerspectiveViewTransformer perspectiveView;

     static void initGuideShape();
	static void drawPerspectivePolygon(const Vector3D *points,int numPoints);
     static void fillPerspectivePolygon(const Vector3D *points,int numPoints);

	static std::list<MouseMotionListener*> *curMouseListeners;

	virtual void close();
     void drawPerspectiveView(int windowid,const std::list<Sensor*> & sensors);

	void drawTop(int windowid,const std::list<Sensor*> & sensors,
					    int left,int top,int width,int height) const;
     void drawFront(int windowid,const std::list<Sensor*> & sensors,
					    int left,int top,int width,int height) const;
     void drawRight(int windowid,const std::list<Sensor*> & sensors,
					    int left,int top,int width,int height) const;
     void deleteSensors(double x,double y,double z,int view);
     void freeMouseListeners();
     void getPositionInformation(int x,int y,
	  double &x3d,double &y3d,double &z3d,int &view) const;

     bool isWithinBoundaries(double x,double y,double z,
		int view,const Sensor* sensor) const;

	static bool getUltrasonicSensorDirectionTip(
	  const UltrasonicSensor &us,int view,double &x2,double &y2);

     void getCoordinatesFromView(double &dx,double &dy,int view) const;
     static bool convertToViewPoint(const Vector3D &pos,
       int view,double &x,double &y);


	void drawSensor(int view,const Sensor *s) const;


	static void drawGuideRectangle(int left,int top,int width,int height);

 public:
	 SensorEditorUI();

	virtual void handleKeypress(unsigned char key,bool special);
	virtual void draw();
	virtual void update() const;

	virtual void mouseMove(int x,int y);
     virtual void mouseDown(int x,int y);
	virtual void mouseUp(int x,int y);
	virtual void mouseWheel(short amount);

		/**
	Converts the specified dx,dy into 3D coordinates and sets the associated components of p.
	*/
	static void viewPointTo3D(double dx,double dy,int view,Vector3D &p);

     static void getViewRectangle(int &left,int &top,int &w,int &h,int view);
     static bool convertToViewPoint(const Sensor* sensor,int view,
		double &x,double &y);

	/**
	Sets direction based on the specified values.
	@param dx,dy are the relative position of a cursor.
	@param view identifies what view is referred to.
	 (1-top,2-front, and 0-right)

	@param dir is the vector to update
	*/
     static void setDirection(int dx,int dy,int view, Vector3D &dir);

	static void init();
	static bool setGuideLength(double val);
	static bool setGuideWidth(double val);
	static bool setGuideHeight(double val);
};

#endif
