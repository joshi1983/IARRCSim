#include "SensorRepositionMouseListener.hpp"
#include "../ui/SensorEditorUI.hpp"

using namespace std;

SensorRepositionMouseListener::SensorRepositionMouseListener(Sensor *sensor,int view):
  sensor(sensor), view(view)
{

} 
 
void SensorRepositionMouseListener::mouseMoved(double x,double y)
{
  Vector3D newPos=sensor->getPosition();

  SensorEditorUI::viewPointTo3D(x,y,view,newPos);

     sensor->setProperty("position-x",newPos.getX());
     sensor->setProperty("position-y",newPos.getY());
	sensor->setProperty("position-z",newPos.getZ());
   
}

int SensorRepositionMouseListener::getView() const
{
  return view;
}

