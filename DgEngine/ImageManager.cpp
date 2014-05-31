//================================================================================
// @ ImageManager.cpp
// 
// Description:
//
// Definitions for the ImageManager class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "ImageManager.h"
#include "Dg_io.h"

#include "pugixml.hpp"

ImageManager::ImageManager()
{
}	//End: ImageManage::Imagamanager()


//--------------------------------------------------------------------------------
//	@	ImageManager::GetMipmap()
//--------------------------------------------------------------------------------
//		Retrieve an pointer to base Mipmap. Returns NULL if failed to load mipmap.
//--------------------------------------------------------------------------------
const Mipmap& ImageManager::GetMipmap(uint32_t id)
{
  uint32 index;
  if (mipmaps.find(id, index))
  {
    return *mipmaps[index];
  }

	//Try to load the mapping file.

  //Get the file-id mapping

  //Try to load the mipmap into the list

  //Return newly added mipmap

}	//End: ImageManager::operator[]()


//--------------------------------------------------------------------------------
//	@	ImageManager::GetFilenameFromXML()
//--------------------------------------------------------------------------------
//		Retrieve an pointer to base Mipmap. Returns NULL if failed to load mipmap.
//--------------------------------------------------------------------------------
std::string ImageManager::GetFilenameFromXML(uint32_t id)
{
  //Load file
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(xmlFile.c_str());

  //Try to load the mapping file.

  //Get the file-id mapping

  //Try to load the mipmap into the list

  //Return newly added mipmap

}	//End: ImageManager::GetFilenameFromXML()


//--------------------------------------------------------------------------------
//	@	ImageManager::GetImage()
//--------------------------------------------------------------------------------
//		Retrieve an pointer to Image. Returns NULL if failed to load.
//--------------------------------------------------------------------------------
const Image* ImageManager::GetImage(const std::string& tag)
{
	std::list<I_pair>::const_iterator it 
		= image_list.begin();
	
	//find element
	for (it; it != image_list.end(); it++)
	{
		if (it->second == tag)
		{
			//If the image has been found
			return &it->first;
		}
	}

	//If image not found
	
	//Full path
	std::string complete = folder+tag;

	//Try to load image
	Image temp;
	if (!temp.Load(complete))
	{
		return &DEFAULT_IMAGE;
	}

	//Add to list
	image_list.push_back( I_pair(temp, tag) );

	//Return pointer to last element
	return &(image_list.back().first);

}	//End: ImageManager::operator[]()