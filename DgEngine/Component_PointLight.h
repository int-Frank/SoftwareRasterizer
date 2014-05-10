#ifndef COMPONENT_POINTLIGHT_H
#define COMPONENT_POINTLIGHT_H

#include "Component.h"
#include "PointLight.h"
#include "ObjectPair.h"
#include "DgTypes.h"

namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
class Component_POINTLIGHT : public Component
{
public:
	//DATA
	ObjectPair<PointLight> light;

};

//Input
pugi::xml_node& operator>>(pugi::xml_node&, Component_POINTLIGHT&);

#endif