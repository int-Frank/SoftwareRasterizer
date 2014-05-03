//================================================================================
// @ Clipper.cpp
// 
// Description: This file defines Clipper's methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Clipper.h"
#include "Polygon.h"


//--------------------------------------------------------------------------------
//	@	Clipper::Clipper()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Clipper::Clipper(): new_point(NULL), begin_ptr(NULL)
{
}


//--------------------------------------------------------------------------------
//	@	Clipper::~Clipper()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Clipper::~Clipper()
{
}	//End: Clipper::~Clipper()


//--------------------------------------------------------------------------------
//	@	Clipper::Clipper()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Clipper::Clipper(const Clipper& other) : new_point(NULL), begin_ptr(NULL),
	size(0), clipFrustum(other.clipFrustum)
{
}	//End: Clipper::Clipper()


//--------------------------------------------------------------------------------
//	@	Clipper::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Clipper& Clipper::operator=(const Clipper& other)
{
	if (this == &other)
		return *this;

	new_point = NULL;
	begin_ptr = NULL;

	//temp data
	size = 0;

	//Copy data
	clipFrustum = other.clipFrustum;

	return *this;

}	//End: Clipper::operator=()


//--------------------------------------------------------------------------------
//	@	Clipper::ClipPolygon()
//--------------------------------------------------------------------------------
//		Clip Polygon and add to output list.
//		Pre:  SetFrustum has been called.
//		Post: clipped polygon is stored as a linked list of points.
//--------------------------------------------------------------------------------
bool Clipper::ClipPolygon(const Polygon& poly, uint8 planes, Point*& start, uint8& return_size)
{
	//Reset size and indices
	size = 3;
	new_point = &points[3];
	begin_ptr = &points[0];

	//Reset pointers
	points[0].next = &points[1];
	points[1].next = &points[2];
	points[2].next = &points[0];

	//Build polygon into array
	points[0].vertex.pos = poly.p0->position_temp;
	points[0].vertex.clr = poly.p0->clr;
	points[0].vertex.uv = poly.uv0;

	points[1].vertex.pos = poly.p1->position_temp;
	points[1].vertex.clr = poly.p1->clr;
	points[1].vertex.uv = poly.uv1;

	points[2].vertex.pos = poly.p2->position_temp;
	points[2].vertex.clr = poly.p2->clr;
	points[2].vertex.uv = poly.uv2;

	//--------------------------------------------------------------------------------
	//		Clip to frustrum
	//--------------------------------------------------------------------------------

	for (uint8 i = 0; i < Frustum::NUMFACES; ++i)
	{
		if ((planes & (1 << i)) && !ClipTo(clipFrustum.GetPlane(i)))
			return false;
	}

	//Assign polygon data
	start = begin_ptr;
	return_size = size;
	return true;

}	//End: Clipper::ClipPolygon()


//--------------------------------------------------------------------------------
//	@	Clipper::ClipNear()
//--------------------------------------------------------------------------------
//		Clip to near plane
//--------------------------------------------------------------------------------
bool Clipper::ClipTo(const Plane4& plane) 
{
	//Initiate points
	Point* pt_prev(begin_ptr);				//Needed as a reference for insertion/deletion
	Point* pt_current(begin_ptr->next);		//Needed for modifying
	Vertex_RASTER v_prev(pt_prev->vertex);
	Vertex_RASTER v_current(pt_current->vertex);
	uint8 sze = size;

	for (uint8 i = 0; i < sze; ++i)
	{
		//Find where each point lies in relation to the plane
		float dc = plane.Test(v_current.pos);
		float dp = plane.Test(v_prev.pos);

		//previous:	inside or on boundary
		//current:	inside or on boundary
		if (dp >= 0.0f && dc >= 0.0f)
		{
			//Advance to next point
			pt_prev = pt_current;
			pt_current = pt_current->next;
		}

		//previous:	strictly inside
		//current:	outside or on boundary
		else if (dp > 0.0f && dc <= 0.0f)
		{
			// intersection factor (between 0 and 1)
			float frac = dp / (dp - dc);   

			//intersect fraction
			pt_current->vertex.pos = v_prev.pos + frac * (v_current.pos - v_prev.pos);
			pt_current->vertex.clr = v_prev.clr + (v_current.clr - v_prev.clr) * frac;
			pt_current->vertex.uv = v_prev.uv + frac * (v_current.uv - v_prev.uv);

			//Advance to next point
			pt_prev = pt_current;
			pt_current = pt_current->next;
		}

		//previous:	outside or on boundary
		//current:	strictly outside
		else if (dp <= 0.0f && dc < 0.0f)
		{
			//Remove current.position;
			RemovePoint(pt_prev);

			//Get new current.position point
			pt_current = pt_prev->next;
		}

		//previous:	strictly outside
		//current:	strictly inside
		else if (dp < 0.0f && dc > 0.0f)
		{
			// intersection factor (between 0 and 1)
			float frac = dp / (dp - dc);

			//intersect fraction
			new_point->vertex.pos = v_prev.pos + frac * (v_current.pos - v_prev.pos);
			new_point->vertex.clr = v_prev.clr + (v_current.clr - v_prev.clr) * frac;
			new_point->vertex.uv = v_prev.uv + frac * (v_current.uv - v_prev.uv);

			//Add to list
			AddPoint(pt_prev);

			//Advance to next point
			pt_current = pt_current->next;
		}

		//Move to next line
		v_prev = v_current;

		//Assign current
		v_current = pt_current->vertex;

	}

	//If polygon not inside, or on an edge, cull
	return (size > 2);

}	//End: Clipper::ClipTo()