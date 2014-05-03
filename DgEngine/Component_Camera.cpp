#include "Component_Camera.h"
#include "pugixml.hpp"
#include <string>
#include "Dg_io.h"


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_CAMERA& dest)
{

	//iterate through all nodes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "camera_system")
		{
			*it >> dest.cameraSystem;
		}
    }


	//Return node
	return node;
}	//End: Materials::Materials()

