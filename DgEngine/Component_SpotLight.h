#ifndef COMPONENT_SPOTLIGHT_H
#define COMPONENT_SPOTLIGHT_H

#include "Component.h"
#include "SpotLight.h"
#include "ObjectPair.h"
#include "DgTypes.h"

namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
class Component_SPOTLIGHT : public Component
{
public:
	//DATA
	ObjectPair<SpotLight> light;

};

//Input
pugi::xml_node& operator>>(pugi::xml_node&, Component_SPOTLIGHT&);

#endif