#include "../stdafx.h"
#include "../robotcontrollers/TCPIPRobotController.hpp"
#include "Application.hpp"
#include "Environment.hpp"
#include "../physicalobjects/Pylon.hpp"
#include "../physicalobjects/TrafficLight.hpp"
#include "../physicalobjects/StopSign.hpp"
#include "../physicalobjects/Ramp.hpp"
#include "../physicalobjects/Tunnel.hpp"
#include "../physicalobjects/RoboticHelicopter.hpp"
#include "../physicalobjects/VerticalWall.hpp"
#include "../editor/CircuitDesign.hpp"
#include "../sensors/VideoDevice.hpp"
#include "../sensors/Lidar.hpp"
#include "../sensors/UltrasonicSensor.hpp"
#include "../docserver/DocServer.hpp"
#include "../io/HTTPParser.hpp"
#include "../io/FileDirectory.hpp"
#include "../modelio/RRSMComplexObjectLoader.hpp"
#include "../shapes/ZeroPlane.hpp"
#include "../ui/SensorEditorUI.hpp"


#include <ctime>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>


using namespace std;

static ComplexObject *background=NULL;

double Environment::PLANET_RADIUS= 637100000;
 // Earth's radius in cm

void Environment::loadBackground(const char *filename)
{
 RRSMComplexObjectLoader loader;

   if (background!=NULL)
   {
     ComplexObject *b=background;

	   background=NULL;
	   delete b;
   }

   background = loader.loadFrom(filename);

   if (background==NULL)
   {
      cout << "Unable to load environment background model from "
		 << string(filename) << endl;
   }
   updateGeoVectors();
}

std::string Environment::getLoadableTracks(const std::string &format)
{
 ostringstream result;
 list<string> filenames = FileDirectory::getFileNamesInDirectory(
	 string("tracks"));

 list<string>::iterator it = filenames.begin();

 if (format=="xml")
	result<<"<tracks>"<<endl;
 else if (format=="JSON")
	 result<<"["<<endl;

bool first=true;
 // loop through filenames.
 for (;it!=filenames.end();it++)
 {
    if (format=="xml")
       result<<"<track filename=\""<<(*it)<<"\" />"<<endl;
    else if (format=="JSON")
    {
	    if (!first)
		    result<<",";

	    result<<"{\"filename\": \""<<(*it)<<"\"}"<<endl;
    }
    else
	    result<<(*it)<<endl;

    first=false;
 }
 if (format=="xml")
	 result<<"</tracks>";
 else if (format=="JSON")
	 result<<"]"<<endl;

 return result.str();
}

void Environment::drawShadows()
{
    objectListMutex.enterCriticalSection();

  // loop through the objects.
  list<PhysicalObject*>::iterator it=objects.begin();
  const Vector3D &lightDirection = Application::getLightDirection();

    for (;it!=objects.end();)
    {
         (*it)->drawShadows(lightDirection);
    }

    objectListMutex.leaveCriticalSection();
}

bool Environment::loadTrackFrom(const std::string &filename)
{
  objectListMutex.enterCriticalSection();

  clearWithExceptions();

  bool result=loadAdditionalObjectsFrom(filename.c_str());

  objectListMutex.leaveCriticalSection();

  return result;
}


/**
 Returns a string representation of the specified property.
 Empty string indicates not found.
*/
string Environment::getEnvironmentProperty(const string &propName)
{
	  if (propName=="suspension-softness")
		  return asString(RoboticCar::getGlobalSuspensionSoftness());
	  else if (propName=="device-video-width")
		  return asString(VideoDevice::getGlobalWidth());
	  else if (propName=="device-video-height")
		  return asString(VideoDevice::getGlobalHeight());
       else if (propName=="device-video-fov")
	       return asString(VideoDevice::getGlobalFieldOfViewDegrees());

      else if (propName=="application-paused")
         return asString(Application::isPaused());

	  else if (propName=="device-lidar-range-max")
		  return asString(Lidar::getGlobalRangeMax());
	  else if (propName=="device-lidar-range-min")
		  return asString(Lidar::getGlobalRangeMin());
	  else if (propName=="device-lidar-resolution-angle")
		  return asString(Lidar::getGlobalResolutionAngleDegrees());
	  else if (propName=="device-lidar-range-scan")
		  return asString(Lidar::getGlobalRangeScan());
       else if (propName=="device-lidar-scan-frequency")
	       return asString(Lidar::getGlobalLidarScanFrequency());

	  else if (propName=="device-ultrasonic-range-min")
	       return asString(UltrasonicSensor::getGlobalRangeMin());
	  else if (propName=="device-ultrasonic-range-max")
	       return asString(UltrasonicSensor::getGlobalRangeMax());

	  else if (propName=="stopsign-width")
		  return asString(StopSign::getGlobalWidth());
	  else if (propName=="stopsign-bottom")
		  return asString(StopSign::getGlobalBottom());
	  else if (propName=="stopsign-line-distance")
		  return asString(StopSign::getGlobalLineDistance());
	  else if (propName=="stopsign-line-width")
		  return asString(StopSign::getGlobalLineWidth());

	  else if (propName=="trafficlight-bottom")
		  return asString(TrafficLight::getGlobalBottom());
       else if (propName=="trafficlight-height")
		  return asString(TrafficLight::getGlobalHeight());
       else if (propName=="trafficlight-width")
		  return asString(TrafficLight::getGlobalWidth());
       else if (propName=="trafficlight-radius")
		  return asString(TrafficLight::getGlobalLightRadius());
       else if (propName=="trafficlight-line-distance")
		  return asString(TrafficLight::getGlobalLineDistance());
	  else if (propName=="trafficlight-line-width")
		  return asString(TrafficLight::getGlobalLineWidth());
	  else if (propName=="trafficlight-timing-red")
		  return asString(TrafficLight::getGlobalRedLightTiming());
	  else if (propName=="trafficlight-timing-green")
		  return asString(TrafficLight::getGlobalGreenLightTiming());

	  else if (propName=="tunnel-width")
		  return asString(Tunnel::getGlobalWidth());
	  else if (propName=="tunnel-height")
		  return asString(Tunnel::getGlobalHeight());
	  else if (propName=="tunnel-length")
		  return asString(Tunnel::getGlobalLength());
	  else if (propName=="tunnel-length-min")
		  return asString(Tunnel::getGlobalMinLength());
	  else if (propName=="tunnel-length-max")
		  return asString(Tunnel::getGlobalMaxLength());

       else if (propName=="ramp-width")
		  return asString(Ramp::getGlobalWidth());
	  else if (propName=="ramp-height")
		  return asString(Ramp::getGlobalHeight());
	  else if (propName=="ramp-length")
		  return asString(Ramp::getGlobalLength());

	  else if (propName=="pylon-height")
		  return asString(Pylon::getGlobalHeight());
	  else if (propName=="pylon-radius")
		  return asString(Pylon::getGlobalRadius());

	  else if (propName=="device-motor-steering-tcpipport")
		  return asString(TCPIPRobotController::getGlobalSteeringPort());
	  else if (propName=="device-motor-drive-tcpipport")
		  return asString(TCPIPRobotController::getGlobalDrivePort());

	  else if (propName=="environment-lighting-direction-x")
		  return asString(Application::getLightDirectionX());
	  else if (propName=="environment-lighting-direction-y")
		  return asString(Application::getLightDirectionY());
	  else if (propName=="environment-lighting-direction-z")
		  return asString(Application::getLightDirectionZ());

	  else if (propName=="drone-speed-max")
		  return asString(RoboticHelicopter::getMaxSpeed());

	  else if (propName=="docserver-port")
	       return asString(DocServer::getDocServerPort());
	  else
	  {
	     if (stringStartsWith(propName,"device-"))
		{
		  // get the device name, if it is a name.
			string::size_type pos=propName.find('-');

			if (pos!=string::npos)
			{
			  string name=propName.substr(pos+1);
			  pos=name.find('-');
			  if (pos!=string::npos)
			  {
			    string subproperty=name.substr(pos+1);
			     name=name.substr(0,pos);

                    // Does the device exist? Let's find out.
                    Sensor *s=Application::getAllSensors().getByName(name.c_str());
				if (s!=NULL)
				{
				  return s->getProperty(subproperty);
				}
			  }
			}
		}
	     return string("");
	  }
}

void Environment::loadIniFile()
{
	loadIniFile(Application::CONFIG_INI_FILE_NAME);

  // if the file exists, load sensors.ini.
  if (FileDirectory::fileExists(Application::SENSORS_INI_FILE_NAME))
     loadIniFile(Application::SENSORS_INI_FILE_NAME);
}

bool Environment::setINIProperty(const char *propName,double val)
{
  ostringstream line;
    line<< propName<<" = "<<val;
    return processINILine(line.str(),"N/A",-1);
}

bool Environment::processINILine(const string &line,const char *filename,int count)
{
	 istringstream lineStream(line,ios_base::in);
	 string propName;
	 lineStream >> propName;
	 string tok;
	  lineStream >> tok;

	  if (tok=="=")
		  lineStream >> tok;

	 double val;
	 istringstream paramStream(tok,ios_base::in);

	  paramStream >> val;

	  if (propName=="sensor")
	  {
	    string name=tok;
	    string type;

	    lineStream >> type;

	   // cout << "Trying to make a sensor with name "<<name
	    //<<" and type: "<<type<<endl;
	    if (type=="ultrasonic")
	    {
	       if (!Application::getAllSensors().add(new UltrasonicSensor(name)))
			  return false;
	    }
	    else if (type=="lidar")
	    {
	    	  if (!Application::getAllSensors().add(new Lidar(name)))
			  return false;
	    }
	    else if (type=="video")
	    {
             // do nothing.
	    }
	  }
	  else if (propName=="suspension-softness")
		  RoboticCar::setGlobalSuspensionSoftness(val);

	  else if (propName=="environment-lighting-enabled")
		  Application::setLightingEnabled(val);

      else if (propName=="application-paused")
         Application::setPaused(val);

	  else if (propName=="device-video-width")
		  VideoDevice::setGlobalWidth((int)val);
	  else if (propName=="device-video-height")
		  VideoDevice::setGlobalHeight((int)val);
       else if (propName=="device-video-fov")
	       VideoDevice::setGlobalFieldOfViewDegrees(val);
	  else if (propName=="device-video-position-x")
		  VideoDevice::setGlobalPositionX(val);
	  else if (propName=="device-video-position-y")
		  VideoDevice::setGlobalPositionY(val);
	  else if (propName=="device-video-position-z")
		  VideoDevice::setGlobalPositionZ(val);

	  else if (propName=="device-lidar-range-max")
		  Lidar::setGlobalRangeMax(val);
	  else if (propName=="device-lidar-range-min")
		  Lidar::setGlobalRangeMin(val);
	  else if (propName=="device-lidar-resolution-angle")
		  Lidar::setGlobalResolutionAngleDegrees(val);
	  else if (propName=="device-lidar-range-scan")
		  Lidar::setGlobalRangeScan(val);
       else if (propName=="device-lidar-scan-frequency")
	       Lidar::setGlobalLidarScanFrequency(val);

	  else if (propName=="device-ultrasonic-range-min")
	       UltrasonicSensor::setGlobalRangeMin(val);
	  else if (propName=="device-ultrasonic-range-max")
	       UltrasonicSensor::setGlobalRangeMax(val);

	  else if (propName=="stopsign-width")
		  StopSign::setGlobalWidth(val);
	  else if (propName=="stopsign-bottom")
		  StopSign::setGlobalBottom(val);
	  else if (propName=="stopsign-line-distance")
		  StopSign::setGlobalLineDistance(val);
	  else if (propName=="stopsign-line-width")
		  StopSign::setGlobalLineWidth(val);

	  else if (propName=="trafficlight-bottom")
		  TrafficLight::setGlobalBottom(val);
       else if (propName=="trafficlight-height")
		  TrafficLight::setGlobalHeight(val);
       else if (propName=="trafficlight-width")
		  TrafficLight::setGlobalWidth(val);
       else if (propName=="trafficlight-radius")
		  TrafficLight::setGlobalLightRadius(val);
       else if (propName=="trafficlight-line-distance")
		  TrafficLight::setGlobalLineDistance(val);
	  else if (propName=="trafficlight-line-width")
		  TrafficLight::setGlobalLineWidth(val);
	  else if (propName=="trafficlight-timing-red")
		  TrafficLight::setGlobalRedLightTiming((int)val);
	  else if (propName=="trafficlight-timing-green")
		  TrafficLight::setGlobalGreenLightTiming((int)val);

	  else if (propName=="tunnel-width")
		  Tunnel::setGlobalWidth(val);
	  else if (propName=="tunnel-height")
		  Tunnel::setGlobalHeight(val);
	  else if (propName=="tunnel-length")
		  Tunnel::setGlobalLength(val);
	  else if (propName=="tunnel-length-min")
		  Tunnel::setGlobalMinLength(val);
	  else if (propName=="tunnel-length-max")
		  Tunnel::setGlobalMaxLength(val);

       else if (propName=="ramp-width")
		  Ramp::setGlobalWidth(val);
	  else if (propName=="ramp-height")
		  Ramp::setGlobalHeight(val);
	  else if (propName=="ramp-length")
		  Ramp::setGlobalLength(val);

	  else if (propName=="pylon-height")
		  Pylon::setGlobalHeight(val);
	  else if (propName=="pylon-radius")
		  Pylon::setGlobalRadius(val);

	  else if (propName=="device-motor-steering-tcpipport")
		  TCPIPRobotController::setGlobalSteeringPort((int)val);
	  else if (propName=="device-motor-drive-tcpipport")
		  TCPIPRobotController::setGlobalDrivePort((int)val);

	  else if (propName=="roboticcar-guide-height")
		  SensorEditorUI::setGuideHeight(val);
	  else if (propName=="roboticcar-guide-width")
		  SensorEditorUI::setGuideWidth(val);
	  else if (propName=="roboticcar-guide-length")
		  SensorEditorUI::setGuideLength(val);

	  else if (propName=="drone-speed-max")
		  RoboticHelicopter::setMaxSpeed(val);

	  else if (propName=="environment-lighting-direction-x")
		  Application::setLightDirectionX(val);
	  else if (propName=="environment-lighting-direction-y")
		  Application::setLightDirectionY(val);
	  else if (propName=="environment-lighting-direction-z")
		  Application::setLightDirectionZ(val);

	  else if (propName=="docserver-port")
		  DocServer::setDocServerPort((int)val);
	  else if (propName.find("penalty-")==0)
	  {

	  }
	  else if (stringStartsWith(propName,"device-"))
	  {
		  string s=propName.substr(7);
		  string::size_type pos = s.find("-");

		  if (pos==string::npos)
		  {
		  	cout << "Unrecognized property name in "
			  <<filename << "("<<count<<"): " << propName << endl;
			return false;
		  }
		  else
		  {
		     string deviceName=s.substr(0,pos);
			Sensor *sensor=Application::getAllSensors().getByName(deviceName.c_str());

			if (sensor!=NULL)
			{
				string subproperty=s.substr(pos+1);
			   if (!sensor->setProperty(subproperty,val))
			   {
			   	cout << "Device name '"<<deviceName
					<<"' found but subproperty "<<subproperty
				<<" is a problem in "
			      <<filename << "("<<count<<"):"
				 << propName<<", subproperty: "<<subproperty << endl;
				return false;
			   }
			}
			else
			{
			    cout << "Unrecognized device name in "
				    <<filename << "("<<count<<"):"
				    << propName<<", device name: "<<deviceName << endl;
			    return false;
			}
		  }
	  }
	  else
	  {
		  cout << "Unrecognized property name in "
			  <<filename << "("<<count<<"):" << propName << endl;
		  return false;
	  }

  return true;
}

void Environment::loadIniFile(const char *filename)
{
 ifstream in(filename);

   if (!in)
   {
	   cout << "Unable to load from "<<filename<<endl;
      return;
   }

   string line;
   int count=0;

   // loop through lines.
   while (getline(in,line))
   {
	   count++;
	   trimString(line);
      if (line=="")
		 continue;


	 if (line[0]=='#')
		 continue;

      processINILine(line,filename,count);

   }
}

void Environment::loadTextures()
{
  StopSign::init();
  CircuitDesign::init();
  VerticalWall::loadTextures();
}

Environment::Environment()
{

}

Environment::~Environment()
{
	clear();
}

// for the automatic car driving in the simulator
void Environment::addObstaclePointsWithinDistance
(std::list<Vector2D> & obstacles,const Vector2D &pos,
 double distance,const PhysicalObject * notIncluded)
{
    objectListMutex.enterCriticalSection();

	list<PhysicalObject*>::const_iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
		try
		{
			{
				Pylon *p = dynamic_cast<Pylon*>(*it);
				if (p!=NULL && p!=notIncluded)
				{
					double dis=p->getXZDistanceFrom(pos);

					if (dis<=distance)
						obstacles.push_back(Vector2D(p->getX(),p->getZ()));
					continue;
				}
			}
			{
				RoboticCar *car=dynamic_cast<RoboticCar*>(*it);
				if (car!=NULL && car!=notIncluded)
				{
					double dis=car->getXZDistanceFrom(pos);
					if (dis<=distance)
					{
						obstacles.push_back(Vector2D(car->getX(),car->getZ()));
					}
					continue;
				}
			}
			{
				StopSign *sign=dynamic_cast<StopSign*>(*it);
				if (sign!=NULL && sign!=notIncluded)
				{
					double dis=sign->getXZDistanceFrom(pos);
					if (dis<=distance)
					{
						obstacles.push_back(Vector2D(sign->getX(),sign->getZ()));
					}
					continue;
				}
			}
			{
				Tunnel *tunnel=dynamic_cast<Tunnel*>(*it);
				if (tunnel!=NULL && tunnel!=notIncluded)
				{
                      list<Vector2D> points;
				  tunnel->addObstaclePoints(points);

				  list<Vector2D>::const_iterator point_it=points.begin();

				  // loop through the points.
				  for (;point_it!=points.end();point_it++)
				  {
					double dis=pos.getDistanceFrom(*point_it);

					if (dis<=distance)
					{
						obstacles.push_back(*point_it);
					}
				  }
				  continue;
				}
			}
			{
				Ramp *ramp=dynamic_cast<Ramp*>(*it);
				if (ramp!=NULL && ramp!=notIncluded)
				{
                      list<Vector2D> points;
				  ramp->addObstaclePoints(points);

				  list<Vector2D>::const_iterator point_it=points.begin();

				  // loop through the points.
				  for (;point_it!=points.end();point_it++)
				  {
					double dis=pos.getDistanceFrom(*point_it);

					if (dis<=distance)
					{
						obstacles.push_back(*point_it);
					}
				  }
				  continue;
				}
			}
			{
			    VerticalWall *wall = dynamic_cast<VerticalWall*>(*it);
			    if (wall!=NULL && wall!=notIncluded)
				{
                      list<Vector2D> points;
				  wall->addObstaclePoints(points);

				  list<Vector2D>::const_iterator point_it=points.begin();

				  // loop through the points.
				  for (;point_it!=points.end();point_it++)
				  {
					double dis=pos.getDistanceFrom(*point_it);

					if (dis<=distance)
					{
						obstacles.push_back(*point_it);
					}
				  }
				  continue;
				}
			}
		}
		catch (exception& e)
		{
		   cout << "problem with Environment::addObstaclePointsWithinDistance."<<endl;
		}
	}

  objectListMutex.leaveCriticalSection();
}

void Environment::getActiveXZPointsInRange(
  double x,double z,double maxDistance,list<ActiveVector2D> &results)
{
    objectListMutex.enterCriticalSection();

  list<PhysicalObject*>::const_iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
	   (*it)->getActiveXZPointsInRange(x,z,maxDistance,results);
	}

	objectListMutex.leaveCriticalSection();
}

void Environment::clear()
{
    objectListMutex.enterCriticalSection();

  list<PhysicalObject*>::iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
	   delete *it;
	}

   objects.clear();

   objectListMutex.leaveCriticalSection();
}

void Environment::clearWithExceptions()
{
  objectListMutex.enterCriticalSection();

  RoboticCar *mainCar = Application::getMainCar();

  list<PhysicalObject*>::iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
	    if ((*it)!=mainCar && (*it)!=mainCar->getReleasedDrone())
	    {
		   delete *it;
	    }
	}

   objects.clear();

   objects.push_back(mainCar);

   if (mainCar->getReleasedDrone()!=NULL)
      objects.push_back(mainCar->getReleasedDrone());

   objectListMutex.leaveCriticalSection();
}

void Environment::drawForDesign(int windowid)
{
	glColor3d(1,1,1);
	glDisable(GL_DEPTH_TEST);

	objectListMutex.enterCriticalSection();

    list<PhysicalObject*>::const_iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
		(*it)->drawForDesign(windowid);
	}

    objectListMutex.leaveCriticalSection();
}

void Environment::makeCircuitTrack()
{
  objectListMutex.enterCriticalSection();

  makeRandomizedDragRaceTrack();

  // add a traffic light.
  objects.push_back(new TrafficLight(70,100,-1000,PI));
  objects.push_back(new StopSign(-70,100,-1600,PI));

  objectListMutex.leaveCriticalSection();
}

void Environment::deleteObjectsWithinXZdistance(
	double x,double z,double distance,
	const list<PhysicalObject *> exceptions)
{
     objectListMutex.enterCriticalSection();

    list<PhysicalObject*>::const_iterator it = objects.begin();
    list<PhysicalObject*> toremove;

    // loop through all objects.
	for (;it!=objects.end();it++)
	{
	    double dist2 = (*it)->getXZDistanceFrom(x, z);

         if (dist2<distance)
	    { // remove from list and destroy, if it is not one of the exceptions.
		    bool found=false;

		    // loop through the list of exceptions(objects that can't be removed)
		    for ( list<PhysicalObject*>::const_iterator excIt=exceptions.begin();
			    excIt!=exceptions.end();
			    excIt++)
		    {
		      if ((*excIt)==(*it))
		      {
			    found=true;
		         break;
		      }
		    }
		    if (!found)
		    {
			  PhysicalObject* po = *it;
			  /**
			  If a car is being removed, its drone has to be removed also.
			  */
                 RoboticCar * car = dynamic_cast<RoboticCar*>(po);
                 if (car!=NULL)
                 {
                   // remove its helicopter.
       	         RoboticHelicopter * drone = car->getReleasedDrone();
	              if (drone!=NULL)
		         {
				  toremove.push_back(drone);
                   }
			  }
	            toremove.push_back(po);
		    }
	    }
	}

	list<PhysicalObject*>::iterator it2=toremove.begin();
	for (;it2!=toremove.end();)
	{
		PhysicalObject* po = *it2;
		it2++;
        objects.remove(po);
        delete (po);
	}

    objectListMutex.leaveCriticalSection();
}

void Environment::remove(PhysicalObject * o)
{
 RoboticCar * car = dynamic_cast<RoboticCar*>(o);

  objectListMutex.enterCriticalSection();

  if (car!=NULL)
  {

    // remove its helicopter.
	  RoboticHelicopter * drone = car->getReleasedDrone();

	  if (drone!=NULL)
	  {
		  objects.remove(drone);

		  // can it be safely deleted too?
		  // tests show it crashes when drone is deleted.
		  // possible memory leak here.
	  }
  }
  objects.remove(o);

  objectListMutex.leaveCriticalSection();
}

// usually for loading tracks.
bool Environment::loadAdditionalObjectsFrom(const char * filename)
{
  // read the file.
  ifstream in;
   in.open(filename);

   if (!in)
   {
       cout << "File could not be openned: "<<filename<<endl;
      return false;
   }
  string line;
  RRSMComplexObjectLoader loader;

  // cout <<"Environment::loadAdditionalObjectsFrom called."<<endl;

   // loop through the lines.
   while (getline(in,line))
   {
       // process the line.
	  // turn it into an object.
       string name;
	  istringstream iss(line,istringstream::in);

	  iss >> name;

	  PhysicalObject *o=NULL;
	  if (name=="Pylon")
	  {
		  o=new Pylon();
	  }
	  else if (name=="Ramp")
		  o=new Ramp();
	  else if (name=="StopSign")
            o=new StopSign();
	  else if (name=="Tunnel")
		  o=new Tunnel();
      else if (name=="VerticalWall")
          o=new VerticalWall();
	  else if (name=="TrafficLight")
		  o=new TrafficLight();
	  else if (name=="RoboticCar")
		  o=new RoboticCar(this->getNextCarID());
	  else if (name=="RoboticHelicopter")
		  o=new RoboticHelicopter(string("helicopter"));
	  else if (name=="load")
	  {
		  string filename;
		  iss>>filename;
		  o=loader.loadFrom(filename.c_str());
	  }

	  if (o!=NULL)
	  {
	     o->readFrom(iss);
	     if (!isSpecialObstacle(o))
		     add(o);
         else
            delete o; // avoid memory leak.
	  }
   }
   if (getFirstCarNotControlledExternally()==NULL)
	   cout << "Environment main car Not found immediately after loading"<<endl;

   in.close();

  // cout <<"Environment::loadAdditionalObjectsFrom complete."<<endl;

   return true;
}

bool Environment::isSpecialObstacle(PhysicalObject *o)
{
  RoboticCar * car = dynamic_cast<RoboticCar*>(o);
  if (car!=NULL)
  {
     if (car->isMainCar())
     {
        Application::getMainCar()->copyAttributesFrom(car);
        return true;
     }
     return false;
  }
  // is it a released helicopter for the main car?
  RoboticHelicopter * helicopter = dynamic_cast<RoboticHelicopter*>(o);

  if (helicopter!=NULL)
  {

  }

  return false;
}

void Environment::makeDragRaceTrack()
{
    objectListMutex.enterCriticalSection();

	for (int i=0;i<50;i++)
	{
	  objects.push_back(new Pylon(-150,0,-i*60,0.9));
	  objects.push_back(new Pylon(150,0,-i*60,0.9));
	}

    objectListMutex.leaveCriticalSection();
}

void Environment::makeRandomizedDragRaceTrack()
{
	double width=300;
	clock_t t = clock()/CLOCKS_PER_SEC;
	srand(t);

    objectListMutex.enterCriticalSection();

	for (int i=0;i<50;i++)
	{
	 double rVal = 0.001*(rand()%1000);
	 double rVal2 = 0.001*(rand()%1000);
      double rVal3 = 0.001*(rand()%1000);

	  objects.push_back(new Pylon(-width/2-10+rVal*20,0,-i*60+rVal2*15,
		  0.7+rVal3*0.3));
       rVal = 0.001*(rand()%1000);
       rVal2 = 0.001*(rand()%1000);
	  rVal3 = 0.001*(rand()%1000);

	  objects.push_back(new Pylon(width/2-10+rVal*20,0,-i*60+rVal2*15,
		  0.7+rVal3*0.3));
	}

	objectListMutex.leaveCriticalSection();
}

RoboticCar* Environment::getFirstCarNotControlledExternally()
{
   objectListMutex.enterCriticalSection();

   list<PhysicalObject*>::const_iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
	  RoboticCar *rc=dynamic_cast<RoboticCar*>(*it);

	   if (rc!=NULL)
	   {
		   if (!rc->isControlledExternally())
		   {
		       objectListMutex.leaveCriticalSection();
		       return rc;
		   }

	   }
	}

    objectListMutex.leaveCriticalSection();

	return NULL;
}

/**
 Returns the first instance of ComplexObject in
 this environment or NULL if none is found.
*/
ComplexObject* Environment::getBackgroundObject()
{
   return background;
}

void Environment::draw(int windowid,const Vector3D &viewpoint)
{
    objectListMutex.enterCriticalSection();

   // loop through the objects and draw each one.
   list<PhysicalObject*>::const_iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{

	   (*it)->draw(windowid,viewpoint);
	}

	if (Application::isLightingEnabled())
	{
		glColor3d(0,0,0);
		glDisable(GL_LIGHTING);

	  for (it=objects.begin();it!=objects.end();it++)
	  {
		  (*it)->drawShadows(Application::getLightDirection());
	  }


		glEnable(GL_LIGHTING);
	}
	if (background!=NULL)
	{
		background->draw(windowid,viewpoint);
	}

  objectListMutex.leaveCriticalSection();
}

void Environment::ticksPerformed(int milis)
{

    objectListMutex.enterCriticalSection();

    list<PhysicalObject*>::iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
	  PhysicalObject* po = *it;
	  InteractiveObject *io=	dynamic_cast<InteractiveObject*>(po);

	  if (io!=NULL)
	  {
	     io->ticksPerformed(this,milis);
	  }
	}

	objectListMutex.leaveCriticalSection();
	time+=milis;
}

void Environment::writeTo(std::ostream &out)
{
    objectListMutex.enterCriticalSection();

  list<PhysicalObject*>::const_iterator it = objects.begin();

	for (;it!=objects.end();it++)
	{
	  PhysicalObject* po = *it;

	  string name = po->getName();

	  if (name!="PhysicalObject")
	  {// do not write things like ComplexObject

	     out << name<< " ";
	      (*it)->writeTo(out);
	     out << endl;
	  }
	}

	objectListMutex.leaveCriticalSection();
}

void Environment::saveTo(const char *filename)
{
	std::ofstream of;
	of.open(filename);
	if (!of)
	{
		cout << "Unable to write file: "<<filename<<endl;
		return;
	}

	writeTo(of);
	of.close();
}

void Environment::add(PhysicalObject*object)
{
    objectListMutex.enterCriticalSection();

  objects.push_back(object);

  objectListMutex.leaveCriticalSection();
}

double Environment::getElevationForXZPoint(const Vector2D &v)
{
    objectListMutex.enterCriticalSection();

  // loop through all objects.
  list<PhysicalObject*>::const_iterator it = objects.begin();
  double elevation=-999999;

	for (;it!=objects.end();it++)
	{
	  PhysicalObject* po = *it;

       double curEl=po->getElevationForXZPoint(v);
	  if (curEl>elevation)
		  elevation=curEl;
	}

    objectListMutex.leaveCriticalSection();

	return elevation;
}

void Environment::getTopNormal(const Vector2D &xzPoint,Vector3D &normal)
{
    objectListMutex.enterCriticalSection();

  // loop through all objects.
  list<PhysicalObject*>::const_iterator it = objects.begin();
  double elevation=-999999;

	for (;it!=objects.end();it++)
	{
	  PhysicalObject* po = *it;

       double curEl=po->getElevationForXZPoint(xzPoint);
	  if (curEl>elevation)
	  {
		  elevation=curEl;
		  po->getTopNormal(xzPoint,normal);
	  }
	}

	objectListMutex.leaveCriticalSection();
}

void Environment::saveElevationMapBitmap()
{
 int width=60;
 int height=100;
 BitmapImage img(width,height);
 double scale=50;
 double z_offset=-height*scale;

  for (int y=0;y<height;y++)
  {
    if (y%5==0)
       cout << "y="<<y<<endl;

    for (int x=0;x<width;x++)
    {
	   Vector2D p(x*scale,z_offset+y*scale);
       double elevation=getElevationForXZPoint(p);

	   img.setPixel(x,y,img.getColourForRGB(
		   (int)(elevation*100),
		  (int)( elevation*30),(int)(elevation*10)));
    }
  }

  img.saveToBitmapFile("elevations.bmp");
}

void Environment::getAsShapeCollection(ShapeCollection & shapes)
{
 shapes.clear();

     objectListMutex.enterCriticalSection();

 list<PhysicalObject*>::const_iterator it = objects.begin();


 // loop through all the objects.
 for (;it!=objects.end();it++)
 {
	 (*it)->addShapeRepresentation(shapes);
 }

 objectListMutex.leaveCriticalSection();
}

double Environment::getNearestPositiveIntersection(const Ray3D &ray,
				double minDepth,double maxDepth)
{
 ShapeCollection shapes;

objectListMutex.enterCriticalSection();

 list<PhysicalObject*>::const_iterator it = objects.begin();

 // loop through all the objects.
 for (;it!=objects.end();it++)
 {
	 (*it)->addShapeRepresentation(shapes);
 }
double result = shapes.getNearestPositiveIntersection(ray,minDepth,maxDepth);

 objectListMutex.leaveCriticalSection();


 return result;
}

void Environment::getDepthsOnPlane(const Vector3D &pos,const Vector3D & axis1,
	const Vector3D & axis2
	,double minDepth,double maxDepth,double degreesToScan
	,int numRays,double *results)
{
 ShapeCollection shapes;

 shapes.add(new ZeroPlane());

objectListMutex.enterCriticalSection();
 list<PhysicalObject*>::const_iterator it = objects.begin();

 // loop through all the objects.
 for (;it!=objects.end();it++)
 {
	 (*it)->addShapeRepresentation(shapes);
 }

 shapes.removeShapesNotConsideredWithin(pos,maxDepth);

 //cout << "Environment, degrees to scan: "<<degreesToScan<<endl;
 shapes.getDepthsOnPlane(pos,axis1,axis2,minDepth,maxDepth,degreesToScan,
	 numRays,results);

  objectListMutex.leaveCriticalSection();
}

std::string Environment::getCars(const std::string &format)
{
 ostringstream result;

 if (format=="xml")
	result<<"<vehicles>"<<endl;
 else if (format=="JSON")
	 result<<"["<<endl;

objectListMutex.enterCriticalSection();
 list<PhysicalObject*>::const_iterator it = objects.begin();
 int i=0;

 // loop through all the objects.
 for (;it!=objects.end();it++)
 {
    RoboticCar * car = dynamic_cast<RoboticCar*>(*it);
    if (car!=NULL)
    {
      if (format=="xml")
         result<<"<vehicle id=\""<<car->getID()<<"\" maincar=\""<<car->isMainCar()<<"\" />"<<endl;
	 else if (format=="JSON")
	 {
		 if (i!=0)
			 result<<",";

		 result<<"{\"id\": \""<<car->getID()<<"\","<<endl;
		 result<<" \"maincar\": \""<<car->isMainCar()<<"\""<<endl;
		 result<<"}";

		 result<<endl;
	 }
	 else
	     result<<car->getID()<<endl;

	 i++;
    }
 }
 objectListMutex.leaveCriticalSection();

 if (format=="xml")
	 result<<"</vehicles>";
 else if (format=="JSON")
	 result<<"]";

 return result.str();
}


RoboticCar* Environment::getRoboticCarByID(const std::string &id)
{
    objectListMutex.enterCriticalSection();

 list<PhysicalObject*>::const_iterator it = objects.begin();

 // loop through all the objects.
 for (;it!=objects.end();it++)
 {
    RoboticCar * car = dynamic_cast<RoboticCar*>(*it);
    if (car!=NULL)
    {
      if (car->getID()==id)
      {
         objectListMutex.leaveCriticalSection();
         return car;
      }
    }
 }

 objectListMutex.leaveCriticalSection();

 return NULL; // not found.
}

std::string Environment::getNextCarID()
{
 char temp[100];

  for (int i=1;;i++)
  {
	  sprintf(temp,"car%d",i);
	  string id=temp;
	  if (getRoboticCarByID(id)==NULL)
	    return id;
  }

}

void Environment::getGeoPosition(const Vector3D &pos,
	double &latitudeDegrees,double &longitudeDegrees,
	    double &elevation)
{
    elevation = pos.getY();

    // calculate the point relative to centre of the Earth.
    Vector3D earthPoint=originEarth + pos.getZ()*north + pos.getX()*east;

 double longitudeRadians = atan2(earthPoint.getZ(),earthPoint.getX());

  longitudeDegrees= 180*longitudeRadians/PI;

 double latitudeRadians = asin(earthPoint.getY()/Environment::PLANET_RADIUS);

  latitudeDegrees = latitudeRadians*180/PI;
}

void Environment::getGeoPositionFromGPSPoint(double latitudeDegrees,
	    double longitudeDegrees,
	    double elevation, Vector3D &pos)
{
  double latRadians = latitudeDegrees*PI/180;
  double latRadius = PLANET_RADIUS*sin(latRadians);
  double longRadians = longitudeDegrees*PI/180;
  double originRotationDegrees = background->getGeoRotationDegrees();
  double originRotationRadians = originRotationDegrees*PI/180.0;

  pos.set(latRadius*sin(longRadians),
	  PLANET_RADIUS*cos(latRadians),latRadius*sin(longRadians));

   pos.sub(originEarth);

  // pos.set(,elevation,);

   pos.rotateAroundYBy(-originRotationRadians);

   pos.setY(elevation);
}

std::string Environment::getGeoPositionAsString(const Vector3D &pos,const std::string &format)
{
 double latitude;
 double longitude;
 double elevation;

   getGeoPosition(pos,latitude,longitude,elevation);
 ostringstream result;

 // cout.precision(std::numeric_limits< double >::digits10);

 result.precision(15);

  if (format=="xml")
  {
     result<<"<gps longitude=\""<<fixed<<longitude<<"\" latitude=\""
		<<latitude<<"\" elevation=\""<<elevation<<"\">";
  }
  else if (format=="JSON")
  {
	  result<<"{\"longitude\":"<<fixed<<longitude<<", \"latitude\": "
		  <<latitude<<", \"elevation\":"<<elevation<<"}";
  }
  else
  {
      result<<fixed<<longitude<<endl;
	 result<<fixed<<latitude<<endl;
	 result<<elevation<<endl;
  }

  if (format=="xml")
	  result<<"</gps>";

  return result.str();
}

/**
 Updates vectors used for GPS calculations
*/
void Environment::updateGeoVectors()
{
  // calculate the rest based partly on the origin data.
  double originLatitudeDegrees = background->getLatitudeDegrees();
  double originLongitudeDegrees = background->getLongitudeDegrees();
  double latRadians = originLatitudeDegrees*PI/180;
  double latRadius = abs(PLANET_RADIUS*cos(latRadians));
  double longRadians = originLongitudeDegrees*PI/180;

    // calculate position of the backdrop on the earth.
	originEarth.set(latRadius*cos(longRadians),
		PLANET_RADIUS*sin(latRadians),latRadius*sin(longRadians));

    north.set(0,1,0);
    north.rotateBy(background->getGeoRotationDegrees(),1,0,0);
    north.rotateBy(originLatitudeDegrees,0,0,1);
    north.rotateBy(-originLongitudeDegrees,0,1,0);

	east.set(0,0,1);
    east.rotateBy(background->getGeoRotationDegrees(),1,0,0);
    east.rotateBy(originLongitudeDegrees,0,1,0);

}

ComplexObject * Environment::createComplexObject(const char * type) const
{
  if (strcmp(type,"drone")==0)
  {
	  return RoboticHelicopter::createNewComplexObject();
  }
  else if (strcmp(type,"roboticcar")==0)
  {
     return RoboticCar::createNewComplexObject();
  }

  return NULL;
}

bool Environment::loadBackdropFrom(const std::string & id)
{
 vector<Backdrop> &backdrops = Application::getBackdropVector();

  // make all backdrops inactive so that only 1 ends up marked as active eventually.
  for (unsigned int i=0;i<backdrops.size();i++)
  {
  	 backdrops[i].setActive(false);
  }

  // find the backdrop with the specified id.
  for (unsigned int i=0;i<backdrops.size();i++)
  {
    if (backdrops[i].getID()==id)
    {
      loadBackground(backdrops[i].getFilename().c_str());
	  backdrops[i].setActive(true);
      return true;
    }
  }

  return false; // indicate unable to find the backdrop.
}

double Environment::getBoundaryMinX() const
{
      if (background==NULL)
          return -99999;

      return -background->getBoundaryXRange();
}

double Environment::getBoundaryMinY() const
{
    return 0;
}

double Environment::getBoundaryMinZ() const
{
      if (background==NULL)
          return -99999;

      return -background->getBoundaryZRange();
}

double Environment::getBoundaryMaxX() const
{
      if (background==NULL)
          return 99999;

      return background->getBoundaryXRange();
}
double Environment::getBoundaryMaxY() const
{
  return 2000;
}

double Environment::getBoundaryMaxZ() const
{
      if (background==NULL)
          return 99999;

      return background->getBoundaryZRange();
}

string  Environment::getTrack(const string &format)
{
 ostringstream result;

  if (format=="xml")
     result<<"<track>"<<endl;

objectListMutex.enterCriticalSection();
  // loop through obstacles.
  for (list<PhysicalObject*>::const_iterator it=objects.begin();it!=objects.end();it++)
  {
    PhysicalObject *po = *it;

     result<<po->getDescription(format)<<endl;
  }
  objectListMutex.leaveCriticalSection();

  if (format=="xml")
     result<<"</track>";

  return result.str();
}

void Environment::resetClock()
{
    time=0;
}

void Environment::setClock(double val)
{
    time=val;
}

double Environment::getCurrentTime()
{
    return time;
}

string Environment::getCurrentTime(const std::string &format)
{
 ostringstream result;

    if (format=="xml")
       result<<"<time ms=\""<<time<<"\" seconds=\""<<(time/1000)<<"\" />"<<endl;
    else
       result<<time;

    return result.str();
}
