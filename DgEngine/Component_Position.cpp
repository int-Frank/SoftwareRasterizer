#include "Component_Position.h"
#include "pugixml.hpp"
#include <string>
#include "Dg_io.h"


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& Read(pugi::xml_node& node, Component_POSITION& dest, entityID _parent)
{
	//Set parent ID
	dest.parent = _parent;

	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

        if (tag == "vqs")
		{
			*it >> dest.T_PAR_OBJ;
		}
    }

	//Return node
	return node;

}	//End: Materials::Materials()
