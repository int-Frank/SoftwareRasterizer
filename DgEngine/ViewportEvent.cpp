//================================================================================
// @ ViewportEvent.cpp
// 
// Description:
//
// Definitions for the ViewportEvent struct.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "ViewportEvent.h"

//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Builds the ViewportEvents from an xml file, add to array
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, DgArray<ViewportEvent>& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "event")
		{
			ViewportEvent temp;
			*it >> temp;
			dest.push_back(temp);
		}
	}

	return node;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Builds a single Viewport from an xml node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, ViewportEvent& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "entity_id")
		{
			uint32 temp;
			if (StringToNumber(temp, it->child_value(), std::hex))
				dest.entity_id = entityID(temp);
		}
		else if (tag == "viewport_id")
		{
			uint32 temp;
			if (StringToNumber(temp, it->child_value(), std::hex))
				dest.viewport_id = viewportID(temp);
		}
		else if (tag == "active")
		{
			dest.active = ToBool(it->child_value());
		}
	}

	return node;

}	//End: operator>>()