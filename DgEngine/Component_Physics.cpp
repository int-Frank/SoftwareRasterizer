#include "Component_PHYSICS.h"
#include "VQS.h"
#include "pugixml.hpp"


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_PHYSICS& dest)
{
	//Return node
	return node;
}	//End: Materials::Materials()
