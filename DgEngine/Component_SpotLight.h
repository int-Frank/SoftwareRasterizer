#ifndef COMPONENT_SPOTLIGHT_H
#define COMPONENT_SPOTLIGHT_H

#include "SpotLight.h"
#include "ObjectPair.h"
#include "DgTypes.h"

namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
struct Component_SPOTLIGHT
{
	//DATA
	ObjectPair<SpotLight> light;

};

//Input
pugi::xml_node& operator>>(pugi::xml_node&, Component_SPOTLIGHT&);

#endif