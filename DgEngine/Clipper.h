//================================================================================
// @ Clipper.h
// 
// Class: Clipper
//
// Clips polygons to a frustum. The clipped polygon is stored internally as
// a linked list of points. 
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef CLIPPER_H
#define CLIPPER_H

#include "Polygon_RASTER.h"
#include "Polygon_RASTER_SB.h"
#include "DgTypes.h"
#include "Vertex_RASTER.h"
#include "Frustum.h"

struct Polygon;
class Materials;
class Mipmap;
class Image;


//--------------------------------------------------------------------------------
//	@	Clipper
//--------------------------------------------------------------------------------
class Clipper
{
public:

	//--------------------------------------------------------------------------------
	//		Holds Vertex data while clipping
	//--------------------------------------------------------------------------------
	struct Point
	{
		Point::Point(): next(NULL) {}

		Vertex_RASTER vertex;
		Point* next;
	};

public:
	//Constructor/Destructor
	Clipper();
	~Clipper();

	//Copy operations
	Clipper(const Clipper&);
	Clipper& operator=(const Clipper&);

	//Set the clipping frustum
	void SetFrustum(const Frustum& f) { clipFrustum = f; }

	//Process Polygon.
	bool ClipPolygon(const Polygon&, uint8 planes, Point*& start, uint8& return_size);

private:
	//Data members

	//temp data used while clipping
	Point points[15];		//Point array (contatiner used while clipping)
	Point* new_point;		//Pointer of current spare
	Point* begin_ptr;		//pointer to a start position
	uint8 size;				//Size of output polygon

	Frustum clipFrustum;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------

	//Clipping functions
	bool ClipTo(const Plane4&);

	//Functions manipulating points
	inline void AddPoint(Point* ref);		//Adds a point after ref in the list
	inline void RemovePoint(Point* ref);	//Removes a point from the list

};


//--------------------------------------------------------------------------------
//		Add point to polygon list
//--------------------------------------------------------------------------------
inline void Clipper::AddPoint(Point* ref)
{
	//Assign pointer
	new_point->next = ref->next;

	//Add to Point list
	ref->next = new_point;		//Insert new point
	
	//increment spare
	++new_point;

	//increment size
	++size;

}	//End: Clipper::AddPoint


//--------------------------------------------------------------------------------
//		Remove next point from list
//--------------------------------------------------------------------------------
inline void Clipper::RemovePoint(Point* ref)
{
	//Bypass ref->next
	ref->next = ref->next->next;

	//Assign a new begin pointer
	//in case deleted element was the beginning
	begin_ptr = ref;

	//Deincrement size
	--size;

}	//End: Clipper::RemovePoint()


#endif