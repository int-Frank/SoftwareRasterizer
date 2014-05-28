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
ImageManager	IMAGE_MANAGER;		//Managers images and mipmaps
TextureManager	TEXTURE_MANAGER;	//All textures are stored/loaded from here.
Mesh_List		MESH_MANAGER;		//All base objects stored/loaded from here
SettingsParser  g_settingsParser;
WindowManager	*WINDOW = NULL;
