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
#include <memory>
#include "Mipmap.h"
#include "Image.h"
#include "DgMap_d.h"


//--------------------------------------------------------------------------------
//	@	ImageManager
//--------------------------------------------------------------------------------
class ImageManager
{
public:

  enum Values
  {
    HEADER        = 0xFF000000,
    IMAGE_HEAD    = 0x11000000,
    MIPMAP_HEAD   = 0x12000000,
    RESERVE_MASK  = 0xFF,
    NULL_ID       = 0
  };

	//Constructor/Destructor
	ImageManager();
	~ImageManager() {}

	//Accessors
	const Mipmap& GetMipmap(uint32_t id);
	const Image& GetImage(uint32_t id);

	//Manipulators
	void clearAll() {mipmaps.clear(); images.clear();}
	void clearMipmaps() {mipmaps.clear();}
	void clearImages() {images.clear();}

  //Set the path to the (XML) file which maps image file names to IDs.
  void SetDataFile(const std::string& path) const;

private:
  //Data members
	Dg::map<uint32_t, std::shared_ptr<Mipmap>> mipmaps;	//Container for all mipmaps
  Dg::map<uint32_t, std::shared_ptr<Image>> images;	//Container for all Images

  std::string xmlFile;              //The xml file that maps image files to ids.

	//Defaults
	const Mipmap DEFAULT_MIPMAP;
	const Image DEFAULT_IMAGE;

private:
  //Functions
  std::string GetFilenameFromXML(uint32_t id);

	//Disallow Copy operations
	ImageManager(const ImageManager&);
	ImageManager& operator=(const ImageManager&);
	
};

//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
extern ImageManager IMAGE_MANAGER;

#endif