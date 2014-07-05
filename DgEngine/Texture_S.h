//================================================================================
// @ Texture_S.h
// 
// Class: Texture_S
//
// Texture class for static images. 
// 
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef TEXTURE_S_H
#define TEXTURE_S_H

#include "Texture.h"
#include "ImageManager.h"

class Mipmap;
class ImageManager;
namespace pugi{ class xml_node; }

//--------------------------------------------------------------------------------
//	@	Testure_S
//--------------------------------------------------------------------------------
class Texture_S : public Texture
{
public:
	//Constructor/destructor
	Texture_S(): mipmapID(ImageManager::DEFAULT_MM) {}
	Texture_S(uint32_t a_id): mipmapID(a_id) {}
	~Texture_S() {}

	//Copy operations
	Texture_S(const Texture_S& a_mm): mipmapID(a_mm.mipmapID) {}
  Texture_S& operator=(const Texture_S&);

	//Input. Images are retrieved from the ImageManager input.
	friend void Read(const pugi::xml_node& in, Texture_S& dest, ImageManager& resource);

	//Return functions
	uint32_t GetMipmap(uint32 time = 0) const 
		{return mipmapID;}

	//Create copy
	Texture_S* clone() const {return new Texture_S(*this);}

private:
	//Data members
	uint32_t mipmapID;
};


#endif