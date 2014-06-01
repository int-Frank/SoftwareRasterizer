#ifndef COMPONENT_META_H
#define COMPONENT_META_H

#include <string>

#include "Component.h"
#include "DgMap.h"

namespace pugi{ class xml_node; }

//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
class Component_META : public Component
{
public:
    void Clean() { data.reset(); }

public:

    DgMap<std::string, std::string> data;

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_META&);



#endif