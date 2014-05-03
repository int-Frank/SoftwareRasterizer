//================================================================================
// @ ViewportManager.h
// 
// Class: ViewportManager
//
// Loads and stores viewports. Typically all viewports for a program are stored 
// here. Z-Masks can also be applied to z-buffers that lie underneath other 
// viewports. This is usefull to prevent overdraw. Also, the manager handles
// compiling active viewports to a Window..
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================


#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include "Viewport.h"
#include "DgMap.h"
#include "DgTypes.h"
#include "DgArray.h"
#include "DgRect.h"
#include <string>

class WindowManager;
class ViewportHandler;
namespace pugi{class xml_node;}


//--------------------------------------------------------------------------------
//	@	ViewportManager
//--------------------------------------------------------------------------------
class ViewportManager
{
private:
	
	//--------------------------------------------------------------------------------
	//		Viewport and any current z-masks
	//--------------------------------------------------------------------------------
	struct ViewWrap
	{
		Viewport viewport;
		DgArray<DgRect> masks;
	};

public:
	//Constructor / destructor
	ViewportManager();
	~ViewportManager() {}

	//Copy operations
	ViewportManager(const ViewportManager& other);
	ViewportManager& operator= (const ViewportManager&);

	//Build from XML file
	friend pugi::xml_node& operator>>(pugi::xml_node& tool, ViewportManager& dest);
	
	//IDs
	void SetID(std::string str) {id = str;}
	std::string GetID() const {return id;}

	//Set parent dimensions
	void SetParentDimensions(uint32 w, uint32 h);

	//Find out is a particular viewport exists
	bool Exists(viewportID);
	
	//Access viewport by id, NULL if not found
	Viewport* operator[](viewportID);
	const Viewport* operator[](viewportID) const;

	//Lock/Unlock a viewport - lock true only if unlocked to begin with
	bool Lock(viewportID);
	bool Unlock(viewportID);
	bool IsLocked(viewportID);

	//Activate/Deactivate a viewport
	bool Activate(viewportID);
	bool Deactivate(viewportID);
	bool IsActive(viewportID);

	//Clear viewport list
	void Clear();

	//Reset all viewports (typically after each frame)
	void Reset(bool flush, bool zMasks);

	void ApplyZMasks();

	//Compile viewports onto a window
	void Compile(WindowManager*) const;

private:

	std::string id;

	//Data members
	DgMap<viewportID, ViewWrap> viewportList;

	//All viewports are relative to these dimensions
	uint32 parent_w;
	uint32 parent_h;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void init(const ViewportManager&);
	void SetMasks();
};

#endif