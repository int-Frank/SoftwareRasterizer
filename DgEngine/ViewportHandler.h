//================================================================================
// @ ViewportHandler.h
// 
// Class: ViewportHandler
//
// Instances of this class are essentially windows to access the internal 
// viewports. Only one instance of a handler can be attached to a particular
// viewport in the internal viewport manager.
//
// Simplifies accessing and locking viewports. Typically, once a viewport is 
// assigned to a camera, it is desirable that all other cameras will be disallowed
// from accessing this viewport. The ViewportHandler class contains a static 
// viewport manager which acts as the internal resource. All viewports are assigned from
// this resource. The manager must be loaded before any instance of this class
// can be used.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef VIEWPORTHANDLER_H
#define VIEWPORTHANDLER_H

#include "DgTypes.h"
#include "ViewportManager.h"
#include <string>

class Viewport;
class WindowManager;

//--------------------------------------------------------------------------------
//	@	ViewportHandler
//--------------------------------------------------------------------------------
class ViewportHandler
{
	friend class CameraSystem;
public:

	//--------------------------------------------------------------------------------
	//		Encapsulates the state of an internal viewport
	//--------------------------------------------------------------------------------
	struct ViewportState
	{
		ViewportState(): exists(false), isLocked(false), isActive(false) {}

		bool exists;
		bool isLocked;
		bool isActive;
	};

public:
	//Constructor / destructor
	ViewportHandler(): currentViewport(NULL_VIEWPORT_ID){}
	~ViewportHandler();

	//Copy operations
	ViewportHandler(const ViewportHandler&);
	ViewportHandler& operator= (const ViewportHandler&);

	//Attach/Detach
	bool Attach(viewportID);	//Attach handler to a viewport
	bool Detach();				//Detach handler from viewport
	bool Activate();			//Activate viewport
	bool Deactivate();			//Deactivate viewport

	bool IsAttached() const;
	bool IsActive() const;
	ViewportState State() const;

	//Accessors
	Viewport* GetViewport();
	
	//Returns the state of a viewport
	static ViewportState GetState(viewportID);

	//Functions to access the internal Viewport Manager.
	static bool LoadResources(uint32 w, uint32 y);
	static void SetParentDimensions(uint32 w, uint32 y);
	static void Compile(WindowManager*);
	static void Reset(bool flush, bool zMasks) {viewports.Reset(flush, zMasks);}
	static Viewport* GetViewport(viewportID);
	static const viewportID	NULL_VIEWPORT_ID = -1;

private:
	//Data members
	viewportID currentViewport;

	void init(const ViewportHandler&);


	//--------------------------------------------------------------------------------
	//		Static resources
	//--------------------------------------------------------------------------------

	//The viewports a handler can be attached to. Loaded once at startup.
	//These shouldn't change throughout the life of the application.
	static ViewportManager viewports;
	static bool isLoaded;
	static const std::string viewport_file;

};

#endif