#include "Sensor.hpp"
#include <sstream>
#include "../io/HTTPParser.hpp"
#include "../app/Application.hpp"

using namespace std;

double Sensor::POSITION_MIN_Y=3;


Sensor::Sensor(const string &name):
 name(name), mountingObject(NULL), active(true)
{
}

const Vector3D & Sensor::getPosition() const
{
  return position;
}

const string & Sensor::getName() const
{
  return name;
}

const std::string Sensor::formatPropertyAssignment(
	const char*name,const char*val,const std::string &format) const
{
  ostringstream result;

	if (format=="xml")
	{
	   result<<"<property name=\""<<name<<"\" value=\""<<val<<"\"/>"<<endl;
	}
	else if (format=="JSON")
	{
		result << "  \""<<name<<"\": \""<<val<<"\","<<endl;
	}
	else if (format=="ini" || format=="inifile")
        result<<"device-"<<(this->name)<<"-"<<name<<" = "<<val<<endl;
	else
	{
	   result<<name<<" = "<<val<<endl;
	}

	return result.str();
}

const std::string Sensor::formatPropertyAssignment(
	const char*name,const double val,const std::string &format) const
{
  ostringstream valS;
  valS<<val;

  return formatPropertyAssignment(name, valS.str().c_str(),format);
}

const std::string Sensor::getDeviceInfo(const std::string &format) const
{
  ostringstream result;

    if (format=="xml")
    {
       result<<"<device name=\""<<name<<"\" type=\""<<getType()<<"\">";

       result<<getPositionPropertyList(format);

       result<<getDeviceInfoPropertyList(format);

	  result<<"</device>";
    }
    else if (format=="JSON")
    {
	    result<<" {"<<endl;
	    result<<" \"name\": \""<<name<<"\","<<endl;
	    result<<" \"type\": \""<<getType()<<"\","<<endl;

       result<<getPositionPropertyList(format);

       result<<getDeviceInfoPropertyList(format);

	  result<<" }"<<endl;
    }
    else
    {
       result<<"sensor "<<name<<" "<<getType()<<endl;

	  result<<getPositionPropertyList(format);

       result<<getDeviceInfoPropertyList(format);

    }

	return result.str();
}

const std::string Sensor::getProperty(const std::string &name) const
{
  if (name=="name")
	  return this->name;
  else if (name=="position-x")
	  return asString(position.getX());
  else if (name=="position-y")
	  return asString(position.getY());
  else if (name=="position-z")
	  return asString(position.getZ());
  else
	  return string();
}

const std::string Sensor::getPositionPropertyList(const std::string &format) const
{
	string result;

	  result=formatPropertyAssignment("position-x",position.getX(),format);
	  result+=formatPropertyAssignment("position-y",position.getY(),format);
	  result+=formatPropertyAssignment("position-z",position.getZ(),format);

	  return result;
}

bool Sensor::setProperty(const std::string &propertyName,double value)
{
  if (propertyName=="position-x")
	  position.setX(value);
  else if (propertyName=="position-y")
  {
    if (value<POSITION_MIN_Y)
        value = POSITION_MIN_Y;

    position.setY(value);
  }
  else if (propertyName=="position-z")
	  position.setZ(value);
  else
	  return false;

  return true; // indicate property set.
}

void Sensor::setMountingObject(PhysicalObject *po)
{
  mountingObject=po;
}

PhysicalObject * Sensor::getMountingObject()
{
  return mountingObject;
}

bool Sensor::isMovable() const
{
  if (mountingObject==NULL)
     return true;
  else
  {
     if (mountingObject==Application::getMainCar())
        return true;
     else
        return false;
  }
}

bool Sensor::isActive() const
{
    return active;
}

void Sensor::setActive(bool active)
{
    this->active=active;
}
