//================================================================================
// @ CameraSystem.cpp
// 
// Description: This file defines CameraSystem's methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "CameraSystem.h"
#include "VQS.h"
#include "pugixml.hpp"
#include "Mesh.h"
#include "Skybox.h"


//--------------------------------------------------------------------------------
//	@	CameraSystem::CameraSystem()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
CameraSystem::CameraSystem(const CameraSystem& other): 
camera(other.camera), frustum(other.frustum), view(other.view)
{
}	//End: CameraSystem::CameraSystem()


//--------------------------------------------------------------------------------
//	@	CameraSystem::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
CameraSystem& CameraSystem::operator=(const CameraSystem& other)
{
	if (this == &other)
		return *this;

	camera = other.camera;
	frustum = other.frustum;
	view = other.view;

	return *this;

}	//End: CameraSystem::operator=()


//--------------------------------------------------------------------------------
//	@	CameraSystem::SetViewport()
//--------------------------------------------------------------------------------
//		Set the viewport
//--------------------------------------------------------------------------------
bool CameraSystem::SetViewport(viewportID id)
{
	//Get the state of the viewport
	ViewportHandler::ViewportState vps = ViewportHandler::GetState(id);

	//Check if id exists
	if (!vps.exists)
		return false;

	//Check if viewport is locked
	if (vps.isLocked)
		return false;

	//From here, viewport exists and is free to use
	//Save current viewport id
	viewportID id_cur = view.currentViewport;

	//Attach to a new viewport
	if (!view.Attach(id))
	{
		//Something went wrong, 'view' is now invalid.
		//Camera is effectively detached.

		//Try to reattach original viewport
		view.Attach(id_cur);

		return false;
	}

	//Set the camera's fov
	SetFOV(camera.HFOV());

	//Successful attachment of a new viewport. Link new viewport to camera.
	view.GetViewport()->Initialise(camera.BaseFrustum(),
								 camera.Wd2(),
								 camera.Hd2(),
								 camera.NearZ());

	return true;

}	//End: CameraSystem::SetViewport()


//--------------------------------------------------------------------------------
//	@	CameraSystem::Activate()
//--------------------------------------------------------------------------------
//		Activate the viewport
//--------------------------------------------------------------------------------
bool CameraSystem::Activate()
{
	return view.Activate();

}	//End: CameraSystem::Activate()


//--------------------------------------------------------------------------------
//	@	CameraSystem::Dectivate()
//--------------------------------------------------------------------------------
//		Dectivate the viewport
//--------------------------------------------------------------------------------
bool CameraSystem::Deactivate()
{
	return view.Deactivate();

}	//End: CameraSystem::Dectivate()


//--------------------------------------------------------------------------------
//	@	CameraSystem::ToggleActivate()
//--------------------------------------------------------------------------------
//		Toggle the active state of the viewport
//--------------------------------------------------------------------------------
void CameraSystem::ToggleActive()
{
	if (IsActive())
		Deactivate();
	else if (!IsActive() && IsAttached())
		Activate();

}	//End: CameraSystem::Activate()


//--------------------------------------------------------------------------------
//	@	CameraSystem::IsActive()
//--------------------------------------------------------------------------------
//		Is the Camerasystem active?
//--------------------------------------------------------------------------------
bool CameraSystem::IsActive() const
{
	return view.IsActive();

}	//End: CameraSystem::IsActive()


//--------------------------------------------------------------------------------
//	@	CameraSystem::IsAttached()
//--------------------------------------------------------------------------------
//		Is the ViewportHandler valid?
//--------------------------------------------------------------------------------
bool CameraSystem::IsAttached() const
{
	return view.IsAttached();

}	//End: CameraSystem::IsAttached()


//--------------------------------------------------------------------------------
//	@	CameraSystem::SetFOV()
//--------------------------------------------------------------------------------
//		Set the field of views
//--------------------------------------------------------------------------------
void CameraSystem::SetFOV(float hfov)
{
	if (hfov < 0.0f)
		return;

	//Calculate the dimensions of the viewplane from the fov
	float wd2 = DgTan(0.5f * hfov);

	//If not attached to a viewport, just set a default hd2
	if (!IsAttached())
	{
		camera.SetViewplane(wd2, 1.0f);
		return;
	}

	Viewport* vp_ptr = view.GetViewport();
	
	//Find the correct hd2
	float hd2 = wd2 * vp_ptr->h() / vp_ptr->w();

	//Set the camera
	camera.SetViewplane(wd2, hd2);

	//Set the frustum
	camera.SetFrustum(frustum);

	//Set the viewport
	view.GetViewport()->Initialise(camera.BaseFrustum(),
		camera.Wd2(),
		camera.Hd2(),
		camera.NearZ());
	
}	//End: CameraSystem::SetFOV()


//--------------------------------------------------------------------------------
//	@	CameraSystem::SetVQS()
//--------------------------------------------------------------------------------
//		Transform the camera
//--------------------------------------------------------------------------------
void CameraSystem::SetVQS(const VQS& vqs)
{
	//Set the camera
	camera.SetVQS(vqs);

	//Set the frustum
	camera.SetFrustum(frustum);

}	//End: CameraSystem::SetVQS()


//--------------------------------------------------------------------------------
//	@	CameraSystem::UpdateVQS()
//--------------------------------------------------------------------------------
//		Transform the camera
//--------------------------------------------------------------------------------
void CameraSystem::UpdateVQS(const VQS& vqs)
{
	//Set the camera
	camera.UpdateVQS(vqs);

	//Set the frustum
	camera.SetFrustum(frustum);

}	//End: CameraSystem::UpdateVQS()


//--------------------------------------------------------------------------------
//	@	CameraSystem::AddObject() 
//--------------------------------------------------------------------------------
//		Process an object. 
//		Pre:	mesh and mipmap are pointers to valid objects (not NULL pointers).
//				A valid viewport is attached to the camerasystem.
//		Post:	Clips and projects an objects polygons and adds them to the
//				master polygon list.
//--------------------------------------------------------------------------------
void CameraSystem::AddObject(Mesh* mesh, const Materials& materials, 
							 const Mipmap* mipmap, uint8 planes)
{
	//Pass object to viewport
	if (IsAttached())
		view.GetViewport()->AddObject(mesh, materials, mipmap, planes);
	
}	//End: CameraSystem::AddObject()


//--------------------------------------------------------------------------------
//	@	CameraSystem::AddSkybox()
//--------------------------------------------------------------------------------
//		Add a skybox to the viewport
//--------------------------------------------------------------------------------
void CameraSystem::AddSkybox(Skybox& skybox)
{
	if (IsAttached())
		skybox.SendToRenderer(view.GetViewport());

}	//End: CameraSystem::AddSkybox()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Render()
//--------------------------------------------------------------------------------
//		Draw all polygons to the viewpane
//--------------------------------------------------------------------------------
void CameraSystem::Render()
{
	if (IsAttached())
		view.GetViewport()->Render();

}	//End: ViewportHandler::Render()


void ReadCamera(pugi::xml_node& node, CameraSystem& dest)
{
	//Define defaults
	float hfov = 1.1f;
	float near_clip = 0.05f;
	float far_clip = 100.0f;


	//iterate through all attributes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "fov")
		{
			DgString(it->child_value()) >> hfov;
		}
		else if (tag == "near_clip")
		{
			DgString(it->child_value()) >> near_clip;
		}
		else if (tag == "far_clip")
		{
			DgString(it->child_value()) >> far_clip;
		}
	}

	dest.camera.SetNearFar(near_clip, far_clip);
	dest.SetFOV(hfov);
}


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build camera system from XML file
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& tool, CameraSystem& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = tool.begin(); it != tool.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

		if (tag == "camera_obj")
		{
			ReadCamera(*it, dest);
		}

    }

	//Set frustum
	dest.camera.SetFrustum(dest.frustum);

	return tool;

}	//End: operator>>()