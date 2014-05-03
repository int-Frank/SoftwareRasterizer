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
#include "DgError.h"
#include "ImageManager.h"
#include "pugixml.hpp"


//--------------------------------------------------------------------------------
//	@	Texture_S::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Texture_S& Texture_S::operator=(const Texture_S& other)
{
	if (this == &other)
		return *this;

	mipmap = other.mipmap;

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
	dest.mipmap = resource.GetMipmap(in.child_value());

}	//End: Read()