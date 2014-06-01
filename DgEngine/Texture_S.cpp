//================================================================================
// @ Texture_S.cpp
// 
// Description:
//
// Definitions for the Texture_S class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Texture_S.h"
#include "Texture.h"
#include "ImageManager.h"
#include "pugixml.hpp"


//--------------------------------------------------------------------------------
//	@	Texture_S::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Texture_S& Texture_S::operator=(const Texture_S& other)
{
	mipmapID = other.mipmapID;
	return *this;

}	//End: Texture_S::operator=()


//--------------------------------------------------------------------------------
//	@	Read()
//--------------------------------------------------------------------------------
//		Input
//--------------------------------------------------------------------------------
void Read(const pugi::xml_node& in, Texture_S& dest, ImageManager& resource)
{
	//Assign new texture
  std::string tempMMID(in.child_value());

  uint32_t id;
  if (String)

  pugi::xml_node single = in.child("single");
  dest.mipmap = resource.GetMipmap(single.child_value());

}	//End: Read()