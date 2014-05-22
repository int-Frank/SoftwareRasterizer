#include "GameDatabase.h"


//--------------------------------------------------------------------------------
//		Checks if a child class has not already been included in the hierarcy.
//-------------------------------------------------------------------------------
bool IsChildValid(entityID childID, const pugi::xml_node& parent)
{
    //Is the parent the root node?
    if (!parent)
    {
        return true;
    }

    //Get the parent class id
    pugi::xml_attribute parIDAtt = parent.attribute("id");

    if (!parIDAtt)
    {
        return IsChildValid(childID, parent.parent());
    }

    //Read the class id.
    uint32 parentID;
    if (!StringToNumber(parentID, parIDAtt.value(), std::hex))
    {
        return IsChildValid(childID, parent.parent());
    }

    if (parentID == childID)
    {
        return false;
    }

    return IsChildValid(childID, parent.parent());
}


void GameDatabase::ValidateNode(const pugi::xml_node& node)
{
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        // Get the name of the node
        std::string tag = it->name();


    }
}


