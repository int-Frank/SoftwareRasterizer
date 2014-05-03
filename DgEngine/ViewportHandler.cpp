//================================================================================
// @ ViewportHandler.cpp
// 
// Description: This file defines ViewportHandler methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "ViewportHandler.h"
#include "ViewportManager.h"
#include "WindowManager.h"
#include "Dg_io.h"


//--------------------------------------------------------------------------------
//		Statics
//--------------------------------------------------------------------------------
bool ViewportHandler::isLoaded = false;
ViewportManager ViewportHandler::viewports;


//--------------------------------------------------------------------------------
//	@	ViewportHandler::init()
//--------------------------------------------------------------------------------
//		General copying, must set current viewport to null to avoid 
//		duplicates.
//--------------------------------------------------------------------------------
void ViewportHandler::init(const ViewportHandler& other)
{
	currentViewport = NULL_VIEWPORT_ID;

}	//End: ViewportHandler::init()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::ViewportHandler()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
ViewportHandler::ViewportHandler(const ViewportHandler& other)
{
	init(other);

}	//End: ViewportHandler::ViewportHandler()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
ViewportHandler& ViewportHandler::operator=(const ViewportHandler& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;

}	//End: ViewportHandler::operator=()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::~ViewportHandler()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
ViewportHandler::~ViewportHandler()
{
	Detach();

}	//End: ViewportHandler::~ViewportHandler()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::GetState()
//--------------------------------------------------------------------------------
//		Get the state of a viewport
//--------------------------------------------------------------------------------
ViewportHandler::ViewportState ViewportHandler::GetState(viewportID id)
{
	ViewportState vps;

	//Check if attached
	if (id == NULL_VIEWPORT_ID)
	{
		vps.exists = false;
		return vps;
	}

	//Check if id is valid
	if (!viewports.Exists(id))
	{
		vps.exists = false;
		return vps;
	}

	vps.exists = true;
	vps.isLocked = viewports.IsLocked(id);
	vps.isActive = viewports.IsActive(id);

	return vps;

}	//End: ViewportHandler::GetState()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::State()
//--------------------------------------------------------------------------------
//		Get the state of the viewport
//--------------------------------------------------------------------------------
ViewportHandler::ViewportState ViewportHandler::State() const
{
	return GetState(currentViewport);

}	//End: ViewportHandler::State()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Detach()
//--------------------------------------------------------------------------------
//		Detach handler from current viewport
//--------------------------------------------------------------------------------
bool ViewportHandler::Detach()
{
	ViewportState vps = GetState(currentViewport);

	//Check if id exists
	if (!vps.exists)
	{
		currentViewport = NULL_VIEWPORT_ID;
		return true;
	}

	//Ensure viewport is deactivated
	if(!Deactivate())
		return false;

	//Unlock viewport
	viewports.Unlock(currentViewport);

	//Update current id
	currentViewport = NULL_VIEWPORT_ID;

	return true;

}	//End: ViewportHandler::Detach()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Attach()
//--------------------------------------------------------------------------------
//		Attach a the handler to a viewport
//--------------------------------------------------------------------------------
bool ViewportHandler::Attach(viewportID id)
{
	//Detach first
	Detach();

	ViewportState vps = GetState(id);

	//Check if id exists
	if (!vps.exists)
		return false;

	//Check if viewport is locked
	if (vps.isLocked)
		return false;

	//Lock the viewport
	if (!viewports.Lock(id))
		return false;

	//Assign id
	currentViewport = id;

	return true;

}	//End: ViewportHandler::Attach()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Activate()
//--------------------------------------------------------------------------------
//		Activate the viewport
//--------------------------------------------------------------------------------
bool ViewportHandler::Activate()
{
	ViewportState vps = GetState(currentViewport);

	//Check if id exists
	if (!vps.exists)
	{
		currentViewport = NULL_VIEWPORT_ID;
		return false;
	}
	
	//Check if viewport is already active
	if (vps.isActive)
		return true;

	//Finally activate viewport.
	viewports.Activate(currentViewport);

	return true;

}	//End: ViewportHandler::Activate()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Deactivate()
//--------------------------------------------------------------------------------
//		Deactivate the viewport the handler currently controls
//--------------------------------------------------------------------------------
bool ViewportHandler::Deactivate()
{
	ViewportState vps = GetState(currentViewport);

	//Check if id exists
	if (!vps.exists)
	{
		currentViewport = NULL_VIEWPORT_ID;
		return true;
	}

	//Check if viewport is active
	if (!vps.isActive)
		return true;

	//Finally activate viewport.
	viewports.Deactivate(currentViewport);

	return true;

}	//End: ViewportHandler::Deactivate()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::IsAttached()
//--------------------------------------------------------------------------------
//		Is the handler attached to a viewport?
//--------------------------------------------------------------------------------
bool ViewportHandler::IsAttached() const
{
	return GetState(currentViewport).exists;

}	//End: ViewportHandler::IsAttached()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::IsActive()
//--------------------------------------------------------------------------------
//		Is the handler attached to a viewport and active?
//--------------------------------------------------------------------------------
bool ViewportHandler::IsActive() const
{
	ViewportHandler::ViewportState vps = State();

	//Check if id exists
	if (!vps.exists)
		return false;

	//Check if viewport is active
	return vps.isActive;

}	//End: ViewportHandler::IsAttached()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::GetViewport()
//--------------------------------------------------------------------------------
//		Get the viewport this handler is currently controlling
//--------------------------------------------------------------------------------
Viewport* ViewportHandler::GetViewport()
{
	ViewportState vps = GetState(currentViewport);

	//Check if id exists
	if (!vps.exists)
		return NULL;

	//Check if viewport is locked. If the viewport is not lock, it means
	//it is not attached to any handler.
	if (!vps.isLocked)
		return NULL;

	return viewports[currentViewport];

}	//End: ViewportHandler::GetViewport()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::GetViewport()
//--------------------------------------------------------------------------------
//		Get a viewport from the viewport list
//--------------------------------------------------------------------------------
Viewport* ViewportHandler::GetViewport(viewportID id)
{
	ViewportState vps = GetState(id);

	//Check if id exists
	if (!vps.exists)
		return NULL;

	return viewports[id];

}	//End: ViewportHandler::GetViewport()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Load()
//--------------------------------------------------------------------------------
//		Load viewports into the manager. Can only be loaded once.
//--------------------------------------------------------------------------------
bool ViewportHandler::LoadResources(uint32 w, uint32 y)
{
	if (isLoaded)
		return false;

	//Load data
	LoadXMLFile(viewport_file, viewports);

	//Set dimensions
	viewports.SetParentDimensions(w, y);

	isLoaded = true;
	return true;

}	//End: ViewportHandler::Load()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Resize()
//--------------------------------------------------------------------------------
//		Resizes all viewports
//--------------------------------------------------------------------------------
void ViewportHandler::SetParentDimensions(uint32 w, uint32 h)
{
	if (w == 0 || h == 0)
		return;

	viewports.SetParentDimensions(w, h);

}	//End: ViewportHandler::Resize()


//--------------------------------------------------------------------------------
//	@	ViewportHandler::Compile()
//--------------------------------------------------------------------------------
//		Compile viewports onto a window
//--------------------------------------------------------------------------------
void ViewportHandler::Compile(WindowManager* window)
{
	viewports.Compile(window);
}	//End: ViewportHandler::Compile()