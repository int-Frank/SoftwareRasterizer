#ifndef COMPONENT_CAMERA_H
#define COMPONENT_CAMERA_H

#include "CameraSystem.h"
#include "VQS.h"
#include "DgTypes.h"

namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
struct Component_CAMERA
{
	CameraSystem cameraSystem;
	VQS T_OBJ_WLD;			//For transforming objects in world space to camera space

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Component_CAMERA&);


#endif