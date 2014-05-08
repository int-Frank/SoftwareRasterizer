#ifndef COMPONENT_MOVEMENT_H
#define COMPONENT_MOVEMENT_H

#include "Vector4.h"

namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//		Movement, relative to the parent.
//--------------------------------------------------------------------------------
struct Component_MOVEMENT
{
	//DATA
	Vector4 direction;			//vector per second
	float yaw, pitch, roll;		//radians per second

	//Constructor
	Component_MOVEMENT(): direction(Vector4::origin), 
		yaw(0.0f), pitch(0.0f), roll(0.0f) {}

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_MOVEMENT&);

#endif