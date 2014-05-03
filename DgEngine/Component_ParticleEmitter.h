#ifndef COMPONENT_PARTICLEEMITTER_H
#define COMPONENT_PARTICLEEMITTER_H

#include "ParticleEmitter.h"
#include "BaseWrapper.h"

class Mesh;
class Tetxure;
namespace pugi{ class xml_node; }

//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
struct Component_PARTICLEEMITTER
{
	BaseWrapper<ParticleEmitter> particleEmitter;

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_PARTICLEEMITTER&);



#endif