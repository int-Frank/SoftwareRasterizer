#include "Component_Meta.h"
#include "pugixml.hpp"
#include <string>
#include "Dg_io.h"
#include "CommonMath.h"


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_META& dest)
{

    //iterate through all nodes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the attribute
        std::string tag(it->name());
        std::string value(it->child_value());

        //Overwrite entries.
        if (!dest.data.insert(value, tag))
            dest.data.set(tag, value);

    }
    
    //Return node
    return node;
}	//End: Materials::Materials()