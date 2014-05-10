#ifndef COMPONENT_PARTICLEEMITTER_H
#define COMPONENT_PARTICLEEMITTER_H

#include "Component.h"
#include "ParticleEmitter.h"
#include "BaseWrapper.h"

class Mesh;
class Tetxure;
namespace pugi{ class xml_node; }

//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
class Component_PARTICLEEMITTER : public Component
{
public:
    void Clear() { particleEmitter.Clear(); }

public:
	BaseWrapper<ParticleEmitter> particleEmitter;

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_PARTICLEEMITTER&);



#endif