#include "Component_Lights_Affecting.h"
#include "pugixml.hpp"
#include <string>


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_LIGHTS_AFFECTING& dest)
{
	//Return node
	return node;
}	//End: Materials::Materials()
