#ifndef COMPONENT_POINTLIGHT_H
#define COMPONENT_POINTLIGHT_H

#include "PointLight.h"
#include "ObjectPair.h"
#include "DgTypes.h"

namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
struct Component_POINTLIGHT
{
	//DATA
	ObjectPair<PointLight> light;

};

//Input
pugi::xml_node& operator>>(pugi::xml_node&, Component_POINTLIGHT&);

#endif