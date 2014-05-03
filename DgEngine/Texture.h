//================================================================================
// @ Texture.h
// 
// Class: Texture
//
// Base Texture class. Textures are essentially an array of one or more mipmaps 
// (more for animated textures). Typically they are used to texture a mesh. 
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include <string>
#include "DgTypes.h"
#include "BaseClass.h"

class Mipmap;

//--------------------------------------------------------------------------------
//	@	Texture
//--------------------------------------------------------------------------------
class Texture : public BaseClass<Texture>
{
public:

	//Constructor/Destructor
	Texture(){}
	virtual ~Texture() {}

	//Copy constructor
	Texture(const Texture& t) {}

	//Return mipmap for input time 't' (for animations)
	virtual const Mipmap* GetMipmap(uint32 time = 0) const =0;

};

#endif