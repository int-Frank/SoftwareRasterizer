//================================================================================
// @ ImageManager.h
// 
// Class: ImageManager
//
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
#include "Mipmap.h"
#include "Image.h"
#include "dg_shared_ptr.h"
#include "dg_map.h"
#include "XMLValidator.h"


//--------------------------------------------------------------------------------
//	@	ImageManager
//--------------------------------------------------------------------------------
class ImageManager
{
public:

  enum
  {
    PATH_MASK     = 0xFFF00000,
    FILE_MASK     = 0x000FFFFF,
    PATH_MASK_RES = 0x0FF00000, //Values lower than this in the path mask are reserved
    FILE_MASK_RES = 0x0000FFFF, //Values lower than this in the file mask are reserved
    NULL_ID       = 0,
    DEFAULT_IMG   = 1,
    DEFAULT_MM    = 2,
  };

	//Constructor/Destructor
	ImageManager();
	~ImageManager() {}

  //Disallow Copy operations
  ImageManager(const ImageManager&);
  ImageManager& operator=(const ImageManager&);

	//Accessors
	const Mipmap& GetMipmap(uint32_t id);
	const Image& GetImage(uint32_t id);

  //Loads the resource into memory if exists.
  bool MipmapExists(uint32_t a_id) { return (&GetMipmap(a_id) != &defaultMipmap); }
  bool ImageExists(uint32_t a_id) { return (&GetImage(a_id) != &defaultImage); }

	//Manipulators
	void clearAll() {mipmaps.clear(); images.clear();}
	void clearMipmaps() {mipmaps.clear();}
	void clearImages() {images.clear();}

  //Set the path to the (XML) file which maps image file names to IDs.
  bool SetDataFile(const std::string& path);

private:
  //Data members
	Dg::map<uint32_t, Dg::shared_ptr<Mipmap>> mipmaps;	//Container for all mipmaps
  Dg::map<uint32_t, Dg::shared_ptr<Image>> images;	//Container for all Images

  std::string xmlFile;              //The xml file that maps image files to ids.

	//Defaults
	const Mipmap defaultMipmap;
	const Image defaultImage;

private:
  //Functions
  std::string GetFilePathFromXML(uint32_t id);

private:
  const static std::string s_schemaPath;
};

//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
namespace global
{
  extern ImageManager* IMAGE_MANAGER;
}

#endif