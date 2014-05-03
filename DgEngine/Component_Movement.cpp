#include "Component_Movement.h"
#include "pugixml.hpp"
#include "Dg_io.h"
#include <string>


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_MOVEMENT& dest)
{
	//iterate through all attributes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "direction")
		{
			DgString(it->child_value()) >> dest.direction;
		}
		else if (tag == "yaw")
		{
			DgString(it->child_value()) >> dest.yaw;
		}
		else if (tag == "pitch")
		{
			DgString(it->child_value()) >> dest.pitch;
		}
		else if (tag == "roll")
		{
			DgString(it->child_value()) >> dest.roll;
		}

    }

	//Return node
	return node;
}	//End: Materials::Materials()
