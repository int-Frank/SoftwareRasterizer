#ifndef COMPONENT_POSITION_H
#define COMPONENT_POSITION_H

#include "VQS.h"
#include "DgOrderedArray.h"

namespace pugi{class xml_node;}

const entityID ROOT_ID = 0;

//--------------------------------------------------------------------------------
//		Positional data, relative to the parent. The parent will be the 
//		world VQS if the entity is not a child of another.
//--------------------------------------------------------------------------------
struct Component_POSITION
{
	//DATA
	VQS T_WLD_OBJ;	//Set in Systems
	VQS T_PAR_OBJ;

	//VQS heirachy data
	DgOrderedArray<entityID>	children;	//This is set in the PostProcess system.
	entityID					parent;

	Component_POSITION(): parent(ROOT_ID) {}

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& Read(pugi::xml_node&, Component_POSITION&, entityID _parent);

#endif