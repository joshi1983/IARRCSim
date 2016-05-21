#ifndef CIRCUIT_DESIGN_HEADER
#define CIRCUIT_DESIGN_HEADER

#include <iostream>
#include <list>


#include "../app/Environment.hpp"
#include "ActiveVector2D.hpp"
#include "../physicalobjects/RoboticCar.hpp"

class CircuitDesign
{
  private:
     int menuState;

	void drawMenu(int windowid) const;

  public:
     CircuitDesign();
	void draw(int windowid,Environment&e,RoboticCar*mainCar,
		int scaledwidth,int scaledheight,double x,double z);

	void writeTo(std::ostream & out) const;
	int getMenuHeight() const;
	void setMenuState(int stateIndex);
	int getMenuState() const;
	void setMenuHoverState(int stateIndex);
	static void init();
};


#endif
