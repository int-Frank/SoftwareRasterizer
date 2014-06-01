//================================================================================
// @ Global_Objects.cpp
// 
// Description:
//
// This file defines all global objects
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "ImageManager.h"
#include "TextureManager.h"
#include "Mesh_List.h"
#include "WindowManager.h"
#include "SettingsParser.h"


//--------------------------------------------------------------------------------
//		Data managers
//--------------------------------------------------------------------------------
namespace global
{
  ImageManager    *IMAGE_MANAGER   = NULL;		
  TextureManager  *TEXTURE_MANAGER = NULL;
  Mesh_List       *MESH_MANAGER    = NULL;
  SettingsParser  *SETTINGS        = NULL;
  WindowManager   *WINDOW          = NULL;
}

