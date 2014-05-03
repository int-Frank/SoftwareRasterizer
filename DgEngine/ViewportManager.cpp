//================================================================================
// @ ViewportManager.cpp
// 
// Description: This file defines ViewportManager methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "ViewportManager.h"
#include "CommonGraphics.h"
#include "WindowManager.h"

//--------------------------------------------------------------------------------
//	@	ViewportManager::init()
//--------------------------------------------------------------------------------
//		Initialize from another
//--------------------------------------------------------------------------------
void ViewportManager::init(const ViewportManager& other)
{
	viewportList = other.viewportList;
	parent_w = other.parent_w;
	parent_h = other.parent_h;

}	//End: ViewportManager::init()


//--------------------------------------------------------------------------------
//	@	ViewportManager::ViewportManager()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
ViewportManager::ViewportManager(): parent_w(0), parent_h(0)
{
}	//End: ViewportManager::ViewportManager()


//--------------------------------------------------------------------------------
//	@	ViewportManager::ViewportManager()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
ViewportManager::ViewportManager(const ViewportManager& other)
{
	init(other);
}	//End: ViewportManager::ViewportManager()


//--------------------------------------------------------------------------------
//	@	ViewportManager::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
ViewportManager& ViewportManager::operator=(const ViewportManager& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;

}	//End: ViewportManager::operator=()


//--------------------------------------------------------------------------------
//	@	ViewportManager::SetParentDimensions()
//--------------------------------------------------------------------------------
//		Set the parent dimensions. This changes the size of all viewports.
//--------------------------------------------------------------------------------
void ViewportManager::SetParentDimensions(uint32 w, uint32 h)
{
	//Set parent data
	parent_w = w;
	parent_h = h;

	//Adjust all viewports
	for (int32 i = 0; i < viewportList.size(); ++i)
	{
		viewportList[i].viewport.UpdateSize(w,h);
	}

	//Set masks
	SetMasks();

}	//End: ViewportManager::SetParentDimensions()


//--------------------------------------------------------------------------------
//	@	ViewportManager::SetMasks()
//--------------------------------------------------------------------------------
//		Set all z-masks for each viewport. Does not apply masks, just 
//		calculates them.
//--------------------------------------------------------------------------------
void ViewportManager::SetMasks()
{
	//Adjust all viewports
	for (int32 i = 0; i < viewportList.size()-1; ++i)
	{
		//Clear current masks
		viewportList[i].masks.clear();

		//Adjust all viewports
		for (int32 j = i+1; j < viewportList.size(); ++j)
		{
			DgRect temp((viewportList[j].viewport.x() - viewportList[i].viewport.x()),
				(viewportList[j].viewport.y() - viewportList[i].viewport.y()),
				viewportList[j].viewport.w(), viewportList[j].viewport.h());

			if (!ClipRect(temp, viewportList[i].viewport.w(), viewportList[i].viewport.h()))
				viewportList[i].masks.push_back(temp);
		}
	}

}	//End: ViewportManager::SetMasks()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Exists()
//--------------------------------------------------------------------------------
//		Check if a viewport exists
//--------------------------------------------------------------------------------
bool ViewportManager::Exists(viewportID id)
{
	int dummy;
	return viewportList.find(id, dummy);

}	//End: ViewportManager::Exists()


//--------------------------------------------------------------------------------
//	@	RendererManger::operator[]
//--------------------------------------------------------------------------------
//		Accessor, access by id. Returns NULL if not found.
//--------------------------------------------------------------------------------
Viewport* ViewportManager::operator[](viewportID id)
{
	int index;
	if (!viewportList.find(id, index))
		return NULL;

	return &viewportList[index].viewport;

}	//End: RendererManger::operator[]


//--------------------------------------------------------------------------------
//	@	RendererManger::operator[]
//--------------------------------------------------------------------------------
//		Accessor, access by id. Returns NULL if not found.
//--------------------------------------------------------------------------------
const Viewport* ViewportManager::operator[](viewportID id) const
{
	int index;
	if (!viewportList.find(id, index))
		return NULL;

	return &viewportList[index].viewport;

}	//End: RendererManger::operator[]


//--------------------------------------------------------------------------------
//	@	ViewportManager::Lock()
//--------------------------------------------------------------------------------
//		Flag a viewport as locked.
//--------------------------------------------------------------------------------
bool ViewportManager::Lock(viewportID id)
{
	int index;
	if (!viewportList.find(id, index))
		return false;

	viewportList[index].viewport.SwitchLocked(true);

	return true;

}	//End: ViewportManager::Lock()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Unlock()
//--------------------------------------------------------------------------------
//		Flag a viewport as unlocked.
//--------------------------------------------------------------------------------
bool ViewportManager::Unlock(viewportID id)
{
	int index;
	if (!viewportList.find(id, index))
		return false;

	viewportList[index].viewport.SwitchLocked(false);

	return true;

}	//End: ViewportManager::Unlock()


//--------------------------------------------------------------------------------
//	@	ViewportManager::IsLocked()
//--------------------------------------------------------------------------------
//		Is a viewport locked?
//--------------------------------------------------------------------------------
bool ViewportManager::IsLocked(viewportID id)
{
	int index;
	if (!viewportList.find(id, index))
		return false;

	return viewportList[index].viewport.IsLocked();

}	//End: ViewportManager::IsLocked()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Activate()
//--------------------------------------------------------------------------------
//		Activate a viewport. Returns true if viewport found and 
//		successfully activated.
//--------------------------------------------------------------------------------
bool ViewportManager::Activate(viewportID id)
{
	//Check if viewport exists
	int index;
	if (!viewportList.find(id, index))
		return false;

	//Set flag
	viewportList[index].viewport.SwitchActive(true);

	//Reset z-masks
	SetMasks();

	return true;

}	//End: ViewportManager::Activate()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Deactivate()
//--------------------------------------------------------------------------------
//		Deactivate a viewport. Returns true if viewport found and 
//		successfully deactivated.
//--------------------------------------------------------------------------------
bool ViewportManager::Deactivate(viewportID id)
{
	//Check if viewport exists
	int index;
	if (!viewportList.find(id, index))
		return false;

	//Set flag
	viewportList[index].viewport.SwitchActive(false);

	//Reset z-masks
	SetMasks();

	return true;

}	//End: ViewportManager::Deactivate()


//--------------------------------------------------------------------------------
//	@	ViewportManager::IsActive()
//--------------------------------------------------------------------------------
//		Is a viewport Active? Returns true if viewport found and is active.
//--------------------------------------------------------------------------------
bool ViewportManager::IsActive(viewportID id)
{
	int index;
	if (!viewportList.find(id, index))
		return false;

	return viewportList[index].viewport.IsActive();

}	//End: ViewportManager::IsActive()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Clear()
//--------------------------------------------------------------------------------
//		Clears the viewport list.
//--------------------------------------------------------------------------------
void ViewportManager::Clear()
{
	viewportList.reset();

}	//End: ViewportManager::Clear()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Reset()
//--------------------------------------------------------------------------------
//		Reset viewports. Usually done after each frame.
//--------------------------------------------------------------------------------
void ViewportManager::Reset(bool flush, bool zMasks)
{
	for (int32 i = 0; i < viewportList.size(); ++i)
	{
		//Check if active
		if (viewportList[i].viewport.IsActive())
			viewportList[i].viewport.Reset(flush);
		else
			viewportList[i].viewport.Reset(false);	//Reset anyway

	}

	if (zMasks)
	{
		ApplyZMasks();
	}
}	//End: ViewportManager::Reset()


//--------------------------------------------------------------------------------
//	@	ViewportManager::ApplyZMasks()
//--------------------------------------------------------------------------------
//		Applies z-masks to all viewports. This 'masks out' regions on each
//		viewport which is covered by another viewport, preventing overdraw.
//--------------------------------------------------------------------------------
void ViewportManager::ApplyZMasks()
{
	for (int32 i = 0; i < viewportList.size()-1; ++i)
	{
		//Check if active
		if (!viewportList[i].viewport.IsActive())
			continue;

		for (uint32 mask = 0; mask < viewportList[i].masks.size(); ++mask)
		{
			viewportList[i].viewport.MaskOut(viewportList[i].masks[mask]);
		}
	}
}	//End: ViewportManager::ApplyZMasks()


//--------------------------------------------------------------------------------
//	@	ViewportManager::Compile()
//--------------------------------------------------------------------------------
//		Compile all active viewports to a window.
//--------------------------------------------------------------------------------
void ViewportManager::Compile(WindowManager* window) const
{
	for (int32 i = 0; i < viewportList.size(); ++i)
	{
		//Check if active
		if (!viewportList[i].viewport.IsActive())
			continue;

		window->UpdateBuffer(viewportList[i].viewport.viewpane, 
			viewportList[i].viewport.x(), viewportList[i].viewport.y());
	}
}	//End: ViewportManager::Compile()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Builds the database from a xml file
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& tool, ViewportManager& dest)
{
	//iterate through all attributes
    for (	pugi::xml_attribute_iterator ait = tool.attributes_begin(); 
			ait != tool.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "id")
		{
			dest.SetID(ait->as_string());
		}
    }

	//iterate through all nodes
	for (pugi::xml_node_iterator it = tool.begin(); it != tool.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

		if (tag == "viewport")
		{
			//Get the ID
			uint32 id;
			if (!StringToNumber(id, it->attribute("id").value(), std::hex))
				continue;

			//Check for valid id
			if (id < 0)
				continue;

			//Read the viewport
			ViewportManager::ViewWrap temp;
			*it >> temp.viewport;

			//Add to the list
			dest.viewportList.insert(temp, viewportID(id));
		}

    }

	return tool;

}	//End: GameDatabase::Build()