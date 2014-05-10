#ifndef COMPONENT_PHYSICS_H
#define COMPONENT_PHYSICS_H

#include "Component.h"
#include "OBB.h"
#include "Sphere.h"
#include "ObjectPair.h"

class VQS;
class Entity;
namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//		Geometry for physics
//--------------------------------------------------------------------------------
class Component_PHYSICS : public Component
{
public:
	//DATA
	ObjectPair<OBB>    box;
	ObjectPair<Sphere> sphere;

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_PHYSICS&); 


#endif