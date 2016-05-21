#ifndef ENVIRONMENT_HEADER
#define ENVIRONMENT_HEADER

#include "../editor/ActiveVector2D.hpp"
#include "../vectors/Vector2D.hpp"
#include <list>
#include <iostream>
#include <string>

class Environment;

#include "../physicalobjects/InteractiveObject.hpp"
#include "../threads/Mutex.hpp"

class RoboticCar;
class ComplexObject;

class Environment
{
 private:
    Mutex objectListMutex;
      std::list<PhysicalObject *> objects;
	  // all objects that must be rendered

    double time;

	 // the origin position relative to the Earth's Cartesian coordinates.
	 // 1 = 1cm of Earth
     Vector3D originEarth;

	Vector3D north;
	Vector3D east;

	/**
	 Updates vectors used for GPS calculations
	*/
	void updateGeoVectors();
	bool isSpecialObstacle(PhysicalObject * po);

 public:
    Environment();
    virtual ~Environment();
    void resetClock();
    void setClock(double v);
    double getCurrentTime();
    std::string getCurrentTime(const std::string &format);
    void drawShadows();
    void draw(int windowid,const Vector3D &viewpoint);
    void drawForDesign(int windowid);
    void clear();

    /**
    Removes and destroys everything with a few exceptions like the main car and its drone.
    */
    void clearWithExceptions();

    RoboticCar* getFirstCarNotControlledExternally();
    ComplexObject* getBackgroundObject();
    void add(PhysicalObject*object);
    void deleteObjectsWithinXZdistance(
	    double x,double z,double distance,const std::list<PhysicalObject *> exceptions);

    void getAsShapeCollection(ShapeCollection & shapes);
    double getNearestPositiveIntersection(const Ray3D &ray,double minDepth,double maxDepth);
    void getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,int numRays,double *results);

    void ticksPerformed(int milis);
    void makeCircuitTrack();
    void makeDragRaceTrack();
    void makeRandomizedDragRaceTrack();
    void getActiveXZPointsInRange(
	    double x,double z,double maxDistance,
	    std::list<ActiveVector2D> &results);

    virtual void getTopNormal(const Vector2D &xzPoint,Vector3D &normal);
    double getElevationForXZPoint(const Vector2D &v);
    void addObstaclePointsWithinDistance(
	    std::list<Vector2D> & obstacles,const Vector2D &pos,
	    double distance,const PhysicalObject *notIncluded);

    /**
    Returns a description of the current track in the specified format.
    */
    std::string getTrack(const std::string &format);

    /**
    loads the background from the specified file
    */
    void loadBackground(const char *filename);


    void writeTo(std::ostream &out);

    /**
     Returns true if and only if success.
    */
    static bool processINILine(const std::string &line,const char *filename,int count);

    /**
     Returns true if and only if success.
    */
    static bool setINIProperty(const char *propName,double val);

    /**
    Gets the next car name
    */
    std::string getNextCarID();

    /**
    For debugging purposes
    */
    void saveElevationMapBitmap();

    /**
    Saves the track in a format that can be loaded using
    the loadTrackFrom method.
    */
    void saveTo(const char *filename);

    /**
    Does not remove any existing objects but adds additional objects
    specified in the file.
    */
    bool loadAdditionalObjectsFrom(const char * filename);

    /**
     Replaces everything except for the main car, and possibly its drone
    */
    bool loadTrackFrom(const std::string &filename);

    /**
     Loads a backdrop with the specified id.
    */
    bool loadBackdropFrom(const std::string & id);

    /**
     Removes the specified object.
    If the specified object is a RoboticCar, its drone is also removed.
    */
    void remove(PhysicalObject * o);

    /**
    Gets information on all cars in the environment
    */
    std::string getCars(const std::string &format);

    /**
    Gets a car at the specified index.
    */
    RoboticCar * getCarAtIndex(int index);

    RoboticCar * getRoboticCarByID(const std::string & id);

    /**
    Returns a new instance of the specified object type.
    @param type can be "drone" or "roboticcar".
    */
    ComplexObject * createComplexObject(const char * type) const;

    /**
    Calculates geographical position by converting from
    the environment's cartesian coordinate point pos
    into geographical coordinates.
    */
    void getGeoPosition(const Vector3D &pos,double &latitudeDegrees,
	    double &longitudeDegrees,
	    double &elevation);

    /**
    Basically the inverse function of getGeoPosition.
    converts from geographical coordinates(elevation,longitude,latitude)
    into Cartesian coordinates.
    */
    void getGeoPositionFromGPSPoint(double latitudeDegrees,
	    double longitudeDegrees,
	    double elevation, Vector3D &pos);

/**
The boundary for motion of cars and the drone.
*/
    double getBoundaryMinX() const;
    double getBoundaryMinY() const;
    double getBoundaryMinZ() const;

    double getBoundaryMaxX() const;
    double getBoundaryMaxY() const;
    double getBoundaryMaxZ() const;

    /**
    Returns GPS results for the specified point
    */
    std::string getGeoPositionAsString(const Vector3D &pos,const std::string &format);

    /**
    Returns a listing of file names in the specified format.
    Each file name is to be a valid parameter for the loadTrackFrom method.

    If format is "txt", a line separated list of file names is given.
    */
    static std::string getLoadableTracks(const std::string &format);
    static void loadTextures();
    static void loadIniFile();
    static void loadIniFile(const char *filename);


    static std::string getEnvironmentProperty(const std::string &propName);
    static double PLANET_RADIUS;
};

#endif
