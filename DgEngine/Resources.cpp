//================================================================================
// @ Resources.cpp
// 
// Description:
//
// This cpp file defines the pathnames for all external resources. These are
// gathered in one file for simplicity.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include <string>
#include "FontManager.h"
#include "ImageManager.h"
#include "TextureManager.h"
#include "Mesh_List.h"
#include "Skybox.h"
#include "ViewportHandler.h"
#include "MessageBox.h"

//--------------------------------------------------------------------------------
//		Static members
//--------------------------------------------------------------------------------
const std::string ERRORFILE = "errorlog.txt";
const std::string impl::ttf::folder = "fonts/";
const std::string ImageManager::folder = "images/";
const std::string Mesh_List::folder = "objects/base_files/";
const std::string Mesh_List::file_extension = "obj";
const std::string Skybox::obj_file = "skybox";
const std::string ViewportHandler::viewport_file = "Viewports.xml";
const std::string MessageBox::messageBoxFile = "MessageBoxes.xml";