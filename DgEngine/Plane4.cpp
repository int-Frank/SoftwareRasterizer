//================================================================================
// @ Plane4.cpp
// 
// Description: This file contains Plane4's class definitions as well as 
// primitive testing functions.
//
// -------------------------------------------------------------------------------
//
// Original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Plane4.h"
#include "Line4.h"
#include "LineSegment4.h"
#include "CommonMath.h"
#include "Vector4.h"
#include "Ray4.h"

//--------------------------------------------------------------------------------
//	@	Plane4::Plane4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Plane4::Plane4():
normal(1.0f, 0.0f, 0.0f),
offset(0.0f)
{
}	//End: Plane4::Plane4


//--------------------------------------------------------------------------------
//	@	Plane4::Plane4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Plane4::Plane4(float a, float b, float c, float d)
{
	Set(a, b, c, d);

}	//End: Plane4::Plane4()


//--------------------------------------------------------------------------------
//	@	Plane4::Plane4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Plane4::Plane4(const Vector4& n, float d)
{
	Set(n.x, n.y, n.z, d);

}	//End: Plane4::Plane4()


//--------------------------------------------------------------------------------
//	@	Plane4::Plane4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Plane4::Plane4(const Point4& p0, const Point4& p1, const Point4& p2)
{
	Set(p0, p1, p2);

}	//End: Plane4::Plane4


//--------------------------------------------------------------------------------
//	@	Plane4::Plane4()
//--------------------------------------------------------------------------------
//		Copy Constructor
//--------------------------------------------------------------------------------
Plane4::Plane4(const Plane4& other):
normal(other.normal),
offset(other.offset)
{
}	//End: Plane4::Plane4


//--------------------------------------------------------------------------------
//	@	Plane4::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Plane4& Plane4::operator= (const Plane4& other)
{
	normal = other.normal;
	offset = other.offset;

	return *this;

}	//End: Plane4::operator=()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Plane4& source)
{
	out << "[" << source.Normal() << ", "
		<< source.Offset() << "]";

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	Plane4::Get()
//--------------------------------------------------------------------------------
//		Returns normal and offset
//--------------------------------------------------------------------------------
void Plane4::Get(Vector4& normal, float& offset) const
{
	normal = normal;
	offset = offset;

}	//End: Plane4::Get()


//--------------------------------------------------------------------------------
//	@	Plane4::operator==()
//--------------------------------------------------------------------------------
//		Are two Plane4's equal?
//--------------------------------------------------------------------------------
bool Plane4::operator== (const Plane4& plane) const
{
	return (plane.normal == normal && plane.offset == offset);

}	//End: Plane4::operator==()


//--------------------------------------------------------------------------------
//	@	Plane4::operator!=()
//--------------------------------------------------------------------------------
//		Are two Plane4's not equal?
//--------------------------------------------------------------------------------
bool Plane4::operator!= (const Plane4& plane) const
{
	return !(plane.normal == normal && plane.offset == offset);

}	//End: Plane4::operator!=()


//--------------------------------------------------------------------------------
//	@	Plane4::Set()
//--------------------------------------------------------------------------------
//		Sets the parameters
//--------------------------------------------------------------------------------
void Plane4::Set(float a, float b, float c, float d)
{
	//normalise for ckeap distance checks
	float lensq = a*a + b*b + c*c;

	//recover gracefully
	if (::IsZero(lensq))
	{
        std::cerr << "@Plane4::Set(f, f, f, f) -> normal vector has length 0." << std::endl;
		normal = Vector4::xAxis;
		offset = 0.0f;
	}
	else
	{
		float recip = DgInvSqrt(lensq);
		normal.Set(a*recip, b*recip, c*recip);
		offset = d*recip;
	}

}	//End: Plane4::Set()


//--------------------------------------------------------------------------------
//	@	Plane4::Set()
//--------------------------------------------------------------------------------
//		Sets the parameters
//--------------------------------------------------------------------------------
void Plane4::Set(const Vector4& n, const Point4& p)
{
	//normalise for cheap distance checks
	float lensq = n.x*n.x + n.y*n.y + n.z*n.z;

	//recover gracefully
	if (::IsZero(lensq))
	{
        std::cerr << "@Plane4::Set(v, p) -> normal vector has length 0." << std::endl;
		normal = Vector4::xAxis;
		offset = 0.0f;
	}
	else
	{
		float recip = DgInvSqrt(lensq);
		normal.Set(n.x*recip, n.y*recip, n.z*recip);
		offset = -(n.x*p.x + n.y*p.y + n.z*p.z)*recip;
	}

}	//End: Plane4::Set()


//--------------------------------------------------------------------------------
//	@	Plane4::Set()
//--------------------------------------------------------------------------------
//		Sets the parameters
//--------------------------------------------------------------------------------
void Plane4::Set(const Point4& p0, const Point4& p1, const Point4& p2)
{
	//get plane vector
	Vector4 u = p1 - p0;
	Vector4 v = p2 - p0;
	Vector4 w = Cross(u, v);

	//normalise for cheap distance checks
	float lensq = w.x*w.x + w.y*w.y + w.z*w.z;

	//recover gracefully
	//if (::IsZero(lensq))
	if (lensq == 0.0f)
	{
        std::cerr << "@Plane4::Set(p, p, p) -> normal vector has length 0." << std::endl;
		normal = Vector4::xAxis;
		offset = 0.0f;
	}
	else
	{
		float recip = DgInvSqrt(lensq);
		normal.Set(w.x*recip, w.y*recip, w.z*recip);
		offset = -p0.Dot(normal);
	}

}	//End: Plane4::Set()


//--------------------------------------------------------------------------------
//	@	Plane4::Closest()
//--------------------------------------------------------------------------------
//		Returns the closest point on plane to point
//--------------------------------------------------------------------------------
Point4 Plane4::ClosestPoint(const Point4& point) const
{
	return point - Test(point)*normal;

}	//End: Plane4::ClosestPoint()


//--------------------------------------------------------------------------------
//	@	TestLinePlane()
//--------------------------------------------------------------------------------
/*		Summary: Does the line intersect the plane?
			--------------------------------------
		Post:	
		Case - No intersection: returns 0. u set to 0.
		Case - Intersection: returns 1. u set.
			--------------------------------------
		Param<line>:	input line
		Param<plane>:	input plane
		Param<u>:		distance along line to the intersection
*/
//--------------------------------------------------------------------------------
uint8 TestLinePlane(const Line4& line, const Plane4& plane, float& u)
{
	//Retieve data
	Vector4 plane_n = plane.Normal();
	float plane_o = plane.Offset();
	Point4 line_o = line.Origin();
	Vector4 line_d = line.Direction();

	float denom = Dot(plane_n,line_d);

	//check if line is parallel to plane
	if (::IsZero(denom))
	{
		u = 0.0f;
		return 0;
	}

	u = (line_o.Dot(plane_n) + plane_o)/denom;

	return 1;

}	//End: ::TestLinePlane()


//--------------------------------------------------------------------------------
//	@	TestPlaneRay()
//--------------------------------------------------------------------------------
/*		Summary: Does the ray intersect the plane?
			--------------------------------------
		Post:	
		Case - No intersection: returns 0. u set to 0.
		Case - Intersection: returns 1. u set.
			--------------------------------------
		Param<plane>:	input plane
		Param<ray>:		input ray
		Param<u>:	 distance along ray to the intersection
*/
//--------------------------------------------------------------------------------
uint8 TestPlaneRay(const Plane4& plane, const Ray4& ray, float& u)
{
	//Retieve data
	Vector4 plane_n = plane.Normal();
	float plane_o = plane.Offset();
	Point4 ray_o = ray.Origin();
	Vector4 ray_d = ray.Direction();

	float denom = Dot(plane_n,ray_d);

	//check if ray is parallel to plane
	if (::IsZero(denom))
	{
		u = 0.0f;
		return 0;
	}

	//compute u
	u = (ray_o.Dot(plane_n) + plane_o)/denom;

	//if plane is behind ray
	if (u < 0.0f )
	{
		u = 0.0f;
		return 0;
	}

	return 1;

}	//End: ::TestPlaneRay()


//--------------------------------------------------------------------------------
//	@	TestLineSegmentPlane()
//--------------------------------------------------------------------------------
/*		Summary: Does the linesegment intersect the plane?
			--------------------------------------
		Post:	
		Case - No intersection: returns 0. u set to 0.
		Case - Intersection: returns 1. u set.
			--------------------------------------
		Param<segment>:	input linesegment
		Param<plane>:	input plane
		Param<u>:		distance along line to the intersection
*/
//--------------------------------------------------------------------------------
uint8 TestLineSegmentPlane(const LineSegment4& segment, const Plane4& plane, float& u)
{
	//Retrieve data
	Vector4 plane_n = plane.Normal();
	float plane_o = plane.Offset();
	Point4 segment_o = segment.Origin();
	Vector4 segment_d = segment.Direction();
	float segment_l = segment.Length();

	float denom = Dot(plane_n,segment_d/segment_l);

	//check if linesegment is parallel to plane
	if (::IsZero(denom))
	{
		u = 0.0f;
		return 0;
	}

	//compute u
	u = (segment_o.Dot(plane_n) + plane_o)/denom;
	
	//if segment does not intersect Plane
	if (u < 0.0f || u > segment_l)
	{
		u = 0.0f;
		return 0;
	}

	return 1;

}	//End: ::TestLineSegmentPlane()