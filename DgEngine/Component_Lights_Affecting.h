#ifndef COMPONENT_LIGHTS_AFFECTING_H
#define COMPONENT_LIGHTS_AFFECTING_H

#include "Component.h"
#include "DgOrderedArray.h"
#include "DgTypes.h"

namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//		Keep this separate for light emitting objects that don't react to lights.
//--------------------------------------------------------------------------------
class Component_LIGHTS_AFFECTING : public Component
{
public:
    void Clear() { pointlights.reset(); spotlights.reset(); }

public:
	//DATA
	DgOrderedArray<entityID> pointlights;
	DgOrderedArray<entityID> spotlights;

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_LIGHTS_AFFECTING&);

#endif