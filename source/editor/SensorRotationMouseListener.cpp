#include "SensorRotationMouseListener.hpp"
#include "../io/HTTPParser.hpp"
#include "../ui/SensorEditorUI.hpp"

SensorRotationMouseListener::SensorRotationMouseListener(
 Sensor *s,int view): sensor(s),view(view)
{

}

void SensorRotationMouseListener::mouseMoved(double x,double y)
{
  // calculate the rotation angles and set them.
  // Use the Sensor class's setProperty method.
  Vector3D dir(asDouble(sensor->getProperty("direction-x")),
	  asDouble(sensor->getProperty("direction-y")),
	  asDouble(sensor->getProperty("direction-z"))
	  );

   double px,py;

   SensorEditorUI::convertToViewPoint(sensor,view,px,py);

   int dx=(int)(x-px),dy=(int)(y-py);

	SensorEditorUI::setDirection(dx,dy,view, dir);

	sensor->setProperty("direction-x",dir.getX());
     sensor->setProperty("direction-y",dir.getY());
     sensor->setProperty("direction-z",dir.getZ());
}
