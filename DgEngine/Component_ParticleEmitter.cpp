#include "Component_ParticleEmitter.h"
#include "BoxParticleEmitter.h"
#include "DiscParticleEmitter.h"
#include "pugixml.hpp"
#include <string>


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_PARTICLEEMITTER& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "pointEmitter")
		{
			ParticleEmitter temp;
			*it >> temp;
			dest.particleEmitter.Set(temp);
		}
		else if (tag == "boxEmitter")
		{
			BoxParticleEmitter temp;
			*it >> temp;
			dest.particleEmitter.Set(temp);
		}
		else if (tag == "discEmitter")
		{
			DiscParticleEmitter temp;
			*it >> temp;
			dest.particleEmitter.Set(temp);
		}
	}

	//Return node
	return node;
}	//End: Materials::Materials()
