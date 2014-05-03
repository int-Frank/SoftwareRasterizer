//================================================================================
// @ Texture_S.h
// 
// Class: Texture_S
//
// Texture class for static images. 
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
	Texture_S(): mipmap(NULL) {}
	Texture_S(const Mipmap* img): mipmap(img) {}
	~Texture_S() {}

	//Copy operations
	Texture_S(const Texture_S& t): mipmap(t.mipmap) {}
	Texture_S& operator=(const Texture_S&);

	//Input. Images are retrieved from the ImageManager input.
	friend void Read(const pugi::xml_node& in, Texture_S& dest, ImageManager& resource);

	//Return functions
	const Mipmap* GetMipmap(uint32 time = 0) const 
		{return mipmap;}

	//Create copy
	Texture_S* clone() const {return new Texture_S(*this);}

private:
	//Data members
	const Mipmap* mipmap;
};


#endif