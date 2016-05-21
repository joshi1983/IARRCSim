#include "Sensors.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;


Sensors::~Sensors()
{
  m.enterCriticalSection();

  // loop through the sensors.
  for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
  {
     delete (*it);
  }

  m.leaveCriticalSection();
}

   /**
    Tries to add the specified sensor to this container.
    Returns true if and only if the specified sensor was added to this container.
    Nothing is added if the sensor name is already used by another sensor.
   */
bool Sensors::add(Sensor * sensor)
{
  if (getByName(sensor->getName().c_str())!=NULL)
  {
     cout << "A sensor named "<<sensor->getName()<<" already exists."<<endl;
     return false;
  }
  else
  {
     m.enterCriticalSection();

     sensors.push_back(sensor);

     m.leaveCriticalSection();

     return true;
  }
}

   /**
    Removes the sensor with the specified name.
   */
void Sensors::remove(const std::string &name)
{
     m.enterCriticalSection();

  for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
  {
     if ((*it)->getName()==name)
     {
        sensors.remove(*it);
        break;
     }
  }

     m.leaveCriticalSection();

}

Sensor * Sensors::getByName(const char * name)
{

 Sensor * result=NULL;

     m.enterCriticalSection();

  for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
  {
     if ((*it)->getName()==name)
     {
        result=(*it);
        break;
     }
  }

     m.leaveCriticalSection();

  return result;
}

   /**
    Returns a unique name of a sensor that uses the specified prefix.
   */
std::string Sensors::getNewSensorName(const std::string &prefix)
{

	for (int i=1;i<10000;i++)
	{
        ostringstream oss;
          oss<<prefix;
		oss<<i;

		Sensor *s=getByName(oss.str().c_str());
		if (s==NULL)
		{
 			return oss.str();
		}
	}

	return prefix;
}

#ifndef _USE_VISUAL_STUDIO_

VideoDevice * Sensors::getVideoDeviceByWindowID(int windowid)
{
 VideoDevice * result=NULL;

  m.enterCriticalSection();

  // loop through the sensors.
  for (list<Sensor*>::iterator it = sensors.begin();it!=sensors.end();it++)
  {
      VideoDevice * videoDevice = dynamic_cast<VideoDevice*>(*it);
      if (videoDevice!=NULL)
      {
          if (videoDevice->getWindowID()==windowid)
          {
                          result=videoDevice;
                          break;
          }

      }
  }

  m.leaveCriticalSection();

  return result;
}

#endif

  /**
   Returns a document describing all sensors.
  */
std::string Sensors::getDocument(const std::string &format)
{
 ostringstream result;

  if (format=="xml")
     result << "<sensors>";
  else if (format=="JSON")
	  result << "["<<endl;

  m.enterCriticalSection();

  list<Sensor*>::const_iterator it = sensors.begin();
  bool first=true;
  for (;it!=sensors.end();it++)
  {
	  if (!first && format=="JSON")
		  result<<",";

	  first=false;
	  result<<((*it)->getDeviceInfo(format));
	  result<<endl;
  }


  m.leaveCriticalSection();

  if (format=="xml")
	result << "</sensors>";
  else if (format=="JSON")
	  result << "]";

  return result.str();
}

  /**
   Removes all sensors except ones occupying windows.
  */
void Sensors::clearNonEssentialSensors()
{
 list<VideoDevice*> videoDevices;

  m.enterCriticalSection();

  for (list<Sensor*>::iterator it = sensors.begin();it!=sensors.end();it++)
  {
      VideoDevice * video=dynamic_cast<VideoDevice*>(*it);
      if (video!=NULL)
         videoDevices.push_back(video);
      else
          delete (*it);
  }

  sensors.clear();

  for (list<VideoDevice *>::iterator it=videoDevices.begin();it!=videoDevices.end();it++)
  {
     sensors.push_back(*it);
  }

  m.leaveCriticalSection();


}

  /**
   Writes all the sensor data to a file using an ini-like format
  */
void Sensors::saveToINI(const char *filename)
{
  string sensorInfo=getDocument(string("inifile"));
  ofstream of(filename);

  if (!of)
  {
     cout << "Unable to save sensors INI info to "<<filename<<endl;
	return;
  }
  of << sensorInfo;

  of.close();
}

vector<int> Sensors::getWindowIDs()
{
 vector<int> result;

  m.enterCriticalSection();

   // loop through all sensors.
    for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
    {
      Sensor *s=(*it);
       VideoDevice * video = dynamic_cast<VideoDevice*>(s);
       if (video!=NULL)
           result.push_back(video->getWindowID());
    }

  m.leaveCriticalSection();

    return result;
}

void Sensors::switchMountingObject(PhysicalObject *fromObj,PhysicalObject *toObj)
{
      m.enterCriticalSection();

    for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
    {
      Sensor *s=(*it);
       if (s->getMountingObject()==fromObj)
          s->setMountingObject(toObj);
    }

      m.leaveCriticalSection();
}

list<Sensor*> Sensors::getListCopy()
{
 list<Sensor*> result;

      m.enterCriticalSection();

    for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
    {
        result.push_back(*it);
    }

      m.leaveCriticalSection();

  return result;
}

void Sensors::repaintAllWindows()
{
          m.enterCriticalSection();

  // loop through all sensors.
  for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
  {
      VideoDevice* cam = dynamic_cast<VideoDevice*>(*it);
      if (cam!=NULL)
         cam->repaint();
  }

  m.leaveCriticalSection();
}

void Sensors::deactivateSensorsMountedOn(PhysicalObject * po)
{
  m.enterCriticalSection();

  // loop through all sensors.
  for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
  {
      if ((*it)->getMountingObject()==po)
         (*it)->setActive(false);
  }

  m.leaveCriticalSection();
}

void Sensors::activateSensorsMountedOn(PhysicalObject * po)
{
  m.enterCriticalSection();

  // loop through all sensors.
  for (list<Sensor*>::iterator it=sensors.begin();it!=sensors.end();it++)
  {
      if ((*it)->getMountingObject()==po)
         (*it)->setActive(true);
  }

  m.leaveCriticalSection();
}
