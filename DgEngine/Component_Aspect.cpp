#include "Component_Aspect.h"
#include "pugixml.hpp"
#include "Mesh_List.h"
#include "Mesh.h"
#include "TextureManager.h"
#include <string>


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Component_ASPECT& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

        if (tag == "materials")
		{
			*it >> dest.materials;
		}
		else if (tag == "mesh")
		{
			dest.mesh = MESH_MANAGER[std::string(it->child_value())];
		}
		else if (tag == "texture")
		{
			dest.texture = TEXTURE_MANAGER[std::string(it->child_value())];
			char ds = 5;
		}
    }

	if (dest.mesh == NULL)
		return node;

	dest.mesh->SetBV(dest.sphere.base, dest.box.base);

	dest.box.Update();
	dest.sphere.Update();

	//Return node
	return node;
}	//End: Materials::Materials()
