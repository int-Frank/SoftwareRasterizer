//================================================================================
// @ TextureManager.h
// 
// Class: TextureManager
//
// Used to load, store and hand out textures. Since a single texture may be
// used for more than one object, a constant pointer is given instead of
// the texture itself.
//
// Before using the texture manager, an xml document containing all textures
// must be loaded. This must be done for each instance of a texture manager.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <list>
#include <string>
#include "BaseWrapper.h"
#include "Texture.h"
#include "pugixml.hpp"
#include "Texture_S.h"
#include "Mipmap.h"


//--------------------------------------------------------------------------------
//	@	TextureManager
//--------------------------------------------------------------------------------
class TextureManager
{
	typedef std::pair<BaseWrapper<Texture>, std::string> Texture_string;

public:

	//Constructor/destructor
	TextureManager() : DEFAULT_MIPMAP(), DEFAULT_TEXTURE_S(&DEFAULT_MIPMAP) {}
	~TextureManager() {}

	//Find and return a texture
	const Texture* operator[](const std::string& tag);

	//Find and return a specific texture xml node
	const pugi::xml_node Find(std::string type, std::string id) const;
	void clear() {texture_list.clear();}

	//Load Resources. Input is the xml texture file.
	//Returns true for successful load.
	bool LoadDocument(std::string);

private:

	//Data members
	std::list<Texture_string> texture_list;	//The list of textures

	/*!
	 * A valid texture must always be returned. If there was an error finding or
	 * loading a texture, the default static texture is returned.
	 */
	const Texture_S DEFAULT_TEXTURE_S;
	const Mipmap DEFAULT_MIPMAP;
	
	//Resources
	pugi::xml_document document;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------

	//Load a texture
	const Texture* TextureManager::Load(const std::string&);

	//Manipulators
	const Texture* AddTexture(const Texture& t, const std::string& tag);

	//DISALLOW COPY AND ASSIGNMENT
	TextureManager(const TextureManager&);
	TextureManager& operator= (const TextureManager&);

};

//--------------------------------------------------------------------------------
//		GLOBALS
//--------------------------------------------------------------------------------
extern TextureManager TEXTURE_MANAGER;

#endif