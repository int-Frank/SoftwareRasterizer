//================================================================================
// @ Texture_A.cpp
// 
// Description:
//
// Definitions for the Texture_A class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Texture_A.h"
#include "ImageManager.h"
#include "pugixml.hpp"
#include "Dg_io.h"
#include <string>


//--------------------------------------------------------------------------------
//	@	Read
//--------------------------------------------------------------------------------
//		Build frame from an xml node
//--------------------------------------------------------------------------------
namespace impl
{
	void Read(const pugi::xml_node& node, impl::Frame& dest, ImageManager& resource)
	{
    std::string fileID(node.child_value());
    uint32_t id;
    StringToNumber(id, fileID, std::hex);
    dest.tme = uint32(node.attribute("delay").as_int());

    if (resource.MipmapExists(id))
    {
      dest.mipmapID = id;
    }

	}	//End: Frame::Read()
}


//--------------------------------------------------------------------------------
//	@	Texture_A::Texture_A()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Texture_A::Texture_A(const Texture_A &other)
{
	framelist = other.framelist;
	looptime = other.looptime;
}


//--------------------------------------------------------------------------------
//	@	Texture_A::operator
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Texture_A& Texture_A::operator=(const Texture_A& other)
{
	if (this == &other)
		return *this;

	framelist = other.framelist;
	looptime = other.looptime;

	return *this;

}	//End: Texture_A::operator=()


//--------------------------------------------------------------------------------
//	@	Read()
//--------------------------------------------------------------------------------
//		Build animated texture from xml node
//--------------------------------------------------------------------------------
void Read(const pugi::xml_node& node, Texture_A& dest, ImageManager& resource)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "frame")
		{
			impl::Frame temp;
      impl::Read(*it, temp, resource);
			dest.Add(temp);
		}
	}

}	//End: Texture_A::Read()


//--------------------------------------------------------------------------------
//	@	Texture_A::Add()
//--------------------------------------------------------------------------------
//		Add a frame to the frame list
//--------------------------------------------------------------------------------
void Texture_A::Add(const impl::Frame& f)
{
	//add frame to framelist
	framelist.push_back(f);

	//calculate new total display time
	looptime += f.tme;

}	//End: Texture_A::Add()


//--------------------------------------------------------------------------------
//	@	Texture_A::GetMipmap()
//--------------------------------------------------------------------------------
//		Get current frame (mipmap). Returns NULL of frame list empty.
//--------------------------------------------------------------------------------
uint32_t Texture_A::GetMipmap(uint32 time) const
{
	//wrap input time to looptime
	time %= looptime;

	//determine current frame by finding where mod(t) falls in the animation timeline
	uint32 step = 0;
	for (uint32 i = 0; i < framelist.size(); i++)
	{
		step += framelist[i].tme;

		if (time < step)
		{
			return framelist[i].mipmapID;
		}
	}

	return ImageManager::DEFAULT_MM;

}	//End: Texture_A::Add()
