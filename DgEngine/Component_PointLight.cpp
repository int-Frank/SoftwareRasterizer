#include "Component_PointLight.h"
#include "pugixml.hpp"


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_POINTLIGHT& dest)
{
	//Read in the pointlight data
	node >> dest.light.base;
	dest.light.Update();

	//Return node
	return node;

}	//End: Materials::Materials()

