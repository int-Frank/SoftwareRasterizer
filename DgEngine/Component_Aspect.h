#ifndef COMPONENT_ASPECT_H
#define COMPONENT_ASPECT_H

#include "Materials.h"
#include "ObjectPair.h"
#include "Sphere.h"
#include "OBB.h"

class Mesh;
class Tetxure;
namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
struct Component_ASPECT
{
	//Mesh
	Mesh* mesh;

	//Object bounds
	ObjectPair<Sphere> sphere;
	ObjectPair<OBB>    box;
	
	//Frustum intersections
	uint8 intersects;

	//Texture/materials
	const Texture* texture;
	Materials materials;

	//Constructor
	Component_ASPECT(): texture(NULL), mesh(NULL), intersects(0) {}

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_ASPECT&);



#endif