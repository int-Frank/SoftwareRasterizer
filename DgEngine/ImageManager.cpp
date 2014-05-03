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
#include "DgError.h"
#include "Dg_io.h"


ImageManager::ImageManager()
{
}	//End: ImageManage::Imagamanager()


//--------------------------------------------------------------------------------
//	@	ImageManager::GetMipmap()
//--------------------------------------------------------------------------------
//		Retrieve an pointer to base Mipmap. Returns NULL if failed to load mipmap.
//--------------------------------------------------------------------------------
const Mipmap* ImageManager::GetMipmap(const std::string& tag)
{
	std::list<std::pair<Mipmap, std::string>>::const_iterator it 
		= mipmap_list.begin();
	
	//Try to find mipmap in the list of already loaded mipmaps
	for (it; it != mipmap_list.end(); it++)
	{
		if (it->second == tag)
		{
			//If the image has been found
			return &it->first;
		}
	}

	//Full path
	std::string complete = folder+tag;

	//Try to load mipmap
	Mipmap temp;
	if (!temp.Load(complete))
	{
		return &DEFAULT_MIPMAP;
	}

	//Add to list
	mipmap_list.push_back( M_pair(temp, tag) );

	//Return pointer to last element
	return &(mipmap_list.back().first);

}	//End: ImageManager::operator[]()


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