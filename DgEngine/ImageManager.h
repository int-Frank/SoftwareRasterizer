//================================================================================
// @ ImageManager.h
// 
// Class: ImageManager
//
// Class for loading, storing and handing out Images and Mipmaps. Since images 
// and mipmaps are large objects, a constant pointer is given 
// instead of the object itself itself. The static member ImageManager::folder 
// defines which folder the font files are located. This is defined
// in Resources.cpp.
//
// To retreive an image or mipmap, the manager checks the lists to see if it
// already exists. If not, it attempts to load from the designated folder.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================


#ifndef ImageManager_H
#define ImageManager_H

#include <string>
#include <list>
#include "Mipmap.h"
#include "Image.h"

typedef std::pair<Mipmap, std::string>	M_pair;
typedef std::pair<Image, std::string>	I_pair;

//--------------------------------------------------------------------------------
//	@	ImageManager
//--------------------------------------------------------------------------------
class ImageManager
{
public:
	//Constructor/Destructor
	ImageManager();
	~ImageManager() {}

	//Accessors
	const Mipmap* GetMipmap(const std::string& filename);
	const Image* GetImage(const std::string& filename);
	const Image* GetDefaultImage() const { return &DEFAULT_IMAGE; }
	const Mipmap* GetDefaultMipmap() const { return &DEFAULT_MIPMAP; }

	//Manipulators
	void clearAll() {mipmap_list.clear(); image_list.clear();}
	void clearMipmaps() {mipmap_list.clear();}
	void clearImages() {image_list.clear();}

private:
	//Data members
	std::list<M_pair> mipmap_list;	//Container for all mipmaps
	std::list<I_pair> image_list;	//Container for all Images

	//Defaults
	const Mipmap DEFAULT_MIPMAP;
	const Image DEFAULT_IMAGE;

	//Disallow Copy operations
	ImageManager(const ImageManager&);
	ImageManager& operator=(const ImageManager&);
	
	static const std::string folder; //Folder containing images
};

//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
extern ImageManager IMAGE_MANAGER;

#endif