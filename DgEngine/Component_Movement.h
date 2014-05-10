#ifndef COMPONENT_MOVEMENT_H
#define COMPONENT_MOVEMENT_H

#include "Component.h"
#include "Vector4.h"

namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//		Movement, relative to the parent.
//--------------------------------------------------------------------------------
class Component_MOVEMENT : public Component
{
public:
    //Constructor
    Component_MOVEMENT() : direction(Vector4::origin),
        yaw(0.0f), pitch(0.0f), roll(0.0f) {}

public:
	//DATA
	Vector4 direction;			//vector per second
	float yaw, pitch, roll;		//radians per second

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_MOVEMENT&);

#endif