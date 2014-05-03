//================================================================================
// @ ViewportEvent.h
// 
// struct: ViewportEvent
//
// Not sure if this will be a permenant feature. The ViewportEvent is a way to 
// communicate a request to attach a Component_CAMERA to some viewport. This 
// perhaps could be part of some more general event handling system, but will
// do for now.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================


#ifndef VIEWPORTEVENT_H
#define VIEWPORTEVENT_H

#include "DgTypes.h"
#include "ViewportHandler.h"
#include "DgArray.h"

namespace pugi{ class xml_node; }


//--------------------------------------------------------------------------------
//	@	ViewportEvent
//--------------------------------------------------------------------------------
struct ViewportEvent
{
	//Constructor / destructor
	ViewportEvent() : entity_id(0), viewport_id(ViewportHandler::NULL_VIEWPORT_ID),
					  active(false) {}
	ViewportEvent(entityID eid, viewportID vid, bool new_vp, bool _fit, bool _active) :
					  entity_id(eid), viewport_id(vid), active(_active) {}

	//Data
	entityID entity_id;
	viewportID viewport_id;
	bool active;
};

//--------------------------------------------------------------------------------
//		Read events from xml node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, ViewportEvent& dest);
pugi::xml_node& operator>>(pugi::xml_node&, DgArray<ViewportEvent>& dest);

#endif