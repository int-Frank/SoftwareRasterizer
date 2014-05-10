#ifndef COMPONENT_ASPECT_H
#define COMPONENT_ASPECT_H

#include "Component.h"
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
class Component_ASPECT : public Component
{
public:
    //Constructor
    Component_ASPECT() : texture(NULL), mesh(NULL), intersects(0) {}

    void Clear() { mesh = NULL; texture = NULL; }

public:
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

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_ASPECT&);



#endif