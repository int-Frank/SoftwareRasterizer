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

//--------------------------------------------------------------------------------
//	@	ImageManager::ImageManager()
//--------------------------------------------------------------------------------
ImageManager::ImageManager() : defaultMipmap(), defaultImage()
{
}	//End: ImageManage::Imagamanager()


//--------------------------------------------------------------------------------
//	@	ImageManager::ImageManager()
//--------------------------------------------------------------------------------
ImageManager::ImageManager(const ImageManager& other) : defaultMipmap(), defaultImage()
{
  mipmaps = other.mipmaps;
  images = other.images;
  xmlFile = other.xmlFile;
}	//End: ImageManage::Imagamanager()


//--------------------------------------------------------------------------------
//	@	ImageManager::ImageManager()
//--------------------------------------------------------------------------------
ImageManager& ImageManager::operator=(const ImageManager& other)
{
  if (this == &other)
    return *this;

  mipmaps = other.mipmaps;
  images = other.images;
  xmlFile = other.xmlFile;

  return *this;
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
  std::string path = GetFilePathFromXML(id);

  if (path == "")
  {
    return defaultMipmap;
  }

  //Try to load the mipmap into the list
  Mipmap *tempMM = new Mipmap;
  if (!tempMM->Load(path))
  {
    delete tempMM;
    return defaultMipmap;
  }

  Dg::shared_ptr<Mipmap> tempMM_sptr(tempMM);
  mipmaps.insert(id, tempMM_sptr);

  //Return newly added mipmap
  return *tempMM;

}	//End: ImageManager::operator[]()


//--------------------------------------------------------------------------------
//	@	ImageManager::GetImage()
//--------------------------------------------------------------------------------
//		Retrieve an pointer to base Mipmap. Returns NULL if failed to load mipmap.
//--------------------------------------------------------------------------------
const Image& ImageManager::GetImage(uint32_t id)
{
  uint32 index;
  if (images.find(id, index))
  {
    return *images[index];
  }

  //Try to load the mapping file.
  std::string path = GetFilePathFromXML(id);

  if (path == "")
  {
    return defaultImage;
  }

  //Try to load the mipmap into the list
  Image *tempImg = new Image;
  if (!tempImg->Load(path))
  {
    delete tempImg;
    return defaultImage;
  }

  Dg::shared_ptr<Image> tempImg_sptr(tempImg);
  images.insert(id, tempImg_sptr);

  //Return newly added mipmap
  return *tempImg;

}	//End: ImageManager::operator[]()


//--------------------------------------------------------------------------------
//	@	ImageManager::GetFilenameFromXML()
//--------------------------------------------------------------------------------
std::string ImageManager::GetFilePathFromXML(uint32_t id)
{
  //Try to load the mapping file.
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(xmlFile.c_str());

  if (!result)
  {
    return "";
  }

  pugi::xml_node root = doc.document_element();

  //Get the path and file components from the id
  std::stringstream ss;
  ss << (id & PATH_MASK);
  std::string pathID(ss.str());
  ss.str(std::string());
  ss << (id & FILE_MASK);
  std::string fileID(ss.str());

  pugi::xml_node pathNode = root.find_child_by_attribute("filePath", "id", pathID.c_str());

  if (!pathNode)
  {
    return "";
  }

  pugi::xml_node fileNode = pathNode.find_child_by_attribute("file", "id", fileID.c_str());


  if (!fileNode)
  {
    return "";
  }

  pugi::xml_attribute pathAtt = pathNode.attribute("path");
  std::string path(pathAtt.as_string());
  path += std::string(fileNode.child_value());
  return path;

}	//End: ImageManager::GetFilenameFromXML()


//--------------------------------------------------------------------------------
//		@GameDtabase::SetDataFile()
//--------------------------------------------------------------------------------
bool ImageManager::SetDataFile(const std::string& path)
{
  if (path == xmlFile)
  {
    return true;
  }

  XMLValidator fileValidator;
  if (!fileValidator.SetSchema(s_schemaPath))
  {
    return false;
  }

  //Does class file conform to the schema?
  if (!fileValidator.ValidateXML(path))
  {
    return false;
  }

  xmlFile = path;

  return true;
}