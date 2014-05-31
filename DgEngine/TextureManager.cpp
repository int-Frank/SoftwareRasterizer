//================================================================================
// @ TextureManager.cpp
// 
// Description:
//
// Definitions for the TextureManager class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "TextureManager.h"
#include "pugixml.hpp"
#include "Texture_S.h"
#include "Texture_A.h"
#include "ImageManager.h"


//--------------------------------------------------------------------------------
//	@	TextureManager::LoadDocument()
//--------------------------------------------------------------------------------
//		Loads the texture document 
//--------------------------------------------------------------------------------
bool TextureManager::LoadDocument(std::string file)
{
	//Open file
	return document.load_file(file.c_str());

}	//End: TextureManager::LoadDocument()


//--------------------------------------------------------------------------------
//	@	TextureManager::AddTexture()
//--------------------------------------------------------------------------------
//		Add a texture to the list. Returns pointer to the added texture.
//--------------------------------------------------------------------------------
const Texture* TextureManager::AddTexture(const Texture& t, uint32 id)
{
	//Add to list
	texture_list.insert(t, id);

	//Return pointer to last element
	return texture_list.back();

}	//End: TextureManager::AddTexture()


//--------------------------------------------------------------------------------
//	@	TextureManager::operator[]
//--------------------------------------------------------------------------------
//		Searches texturelist for a texture. Returns NULL of texture 'tag'
//		could not be loaded.
//--------------------------------------------------------------------------------
const Texture* TextureManager::operator[](const std::string& tag)
{
	std::list<Texture_string>::iterator it = texture_list.begin();

	//find element
	for (it; it != texture_list.end(); it++)
	{
		if (it->second == tag)
		{
			//If the texture has been found
			return it->first;
		}
	}

	//If image not found,
	//Try to load the image
	return Load(tag);
	
}	//End:TextureManager::operator[]


//--------------------------------------------------------------------------------
//	@	TextureManager::Load
//--------------------------------------------------------------------------------
//		Load a texture into texture_list
//--------------------------------------------------------------------------------
const Texture* TextureManager::Load(const std::string& input)
{
	//Get the root node
	pugi::xml_node rootNode = document.document_element();

	//iterate through all nodes
	for (pugi::xml_node_iterator it = rootNode.begin(); it != rootNode.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "single")
		{
			if (!(it->attribute("id").value() == input))
				continue;

			//Read texture
			Texture_S temp_s;
			Read(*it, temp_s, IMAGE_MANAGER);

			//Add to list and return
			return AddTexture(temp_s, input);
		}
		else if (tag == "animated")
		{
			if (!(it->attribute("id").value() == input))
				continue;

			//Read texture
			Texture_A temp_a;
			Read(*it, temp_a, IMAGE_MANAGER);

			//Add to list and return
			return AddTexture(temp_a, input);
		}
	}
	
	return &DEFAULT_TEXTURE_S;

}	//End: TextureManager::Load()


//--------------------------------------------------------------------------------
//	@	TextureManager::Find()
//--------------------------------------------------------------------------------
//		Find a specific node in the document. Only searched nodes in the root.
//--------------------------------------------------------------------------------
const pugi::xml_node TextureManager::Find(std::string type, std::string id) const
{
	//Get the root node
	pugi::xml_node rootNode = document.document_element();

	//iterate through all nodes
	for (pugi::xml_node_iterator it = rootNode.begin(); it != rootNode.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == type)
		{
			if ((it->attribute("id").value() == id))
				return *it;
		}
	}

	//Return the null node
	pugi::xml_node failed;
	return failed;

}	//End: TextureManager::Find()