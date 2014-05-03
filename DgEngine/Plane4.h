//================================================================================
// @ Plane4.h
// 
// Class: Plane4
//
// A Plane is infinite flat surface. It is implemented by using the generalized
// plane equation: Ax + By + Cz + D = 0, or a vector normal to the plane
// and an offset.
//
// -------------------------------------------------------------------------------
//
// Original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef PLANE4_H
#define PLANE4_H

#include "Vector4.h"
#include "Dg_io.h"
#include "Point4.h"
#include "CommonMath.h"
#include "DgTypes.h"
#include "Primitive2D.h"

class Line4;
class Ray4;
class LineSegment4;
class OBB;

//--------------------------------------------------------------------------------
//	@	Plane4
//--------------------------------------------------------------------------------
class Plane4 : public Primitive2D
{
public:
	//constructor/destructor
	Plane4();
	Plane4(float a, float b, float c, float d);
	Plane4(const Point4& p0, const Point4& p1, const Point4& p2);
	Plane4(const Vector4& n, float d);
	inline ~Plane4() {}

	//Output
	friend DgWriter& operator<<(DgWriter& out, const Plane4& source);

	//copy operations
	Plane4(const Plane4& other);
	Plane4& operator= (const Plane4& other);

	//accessors
	inline const Vector4& Normal() const {return normal;}
	inline float Offset() const {return offset;}
	void Get(Vector4& normal, float& offset) const;

	//comparison
	bool operator== (const Plane4&) const;
	bool operator!= (const Plane4&) const;

	//manipulators
	void Set(float a, float b, float c, float d);
	void Set(const Vector4& n, float d);
	void Set(const Vector4& n, const Point4& p); 
	void Set(const Point4& p0, const Point4& p1, const Point4& p2);

	//ests which side of the plane a point lies
	inline float Test(const Point4&) const;

	//! Test a vector against the plane normal
	float NormalDot(const Vector4& v) const { return Dot(normal, v); }

	//Distance to a point
	inline float Distance(const Point4&) const;
	
	//--------------------------------------------------------------------------------
	//		From Primitive2D
	//--------------------------------------------------------------------------------
public:
	
	//--------------------------------------------------------------------------------
	//		From Primitive
	//--------------------------------------------------------------------------------
public:

	Point4 ClosestPoint (const Point4&) const;
	
	//General Interaction functions
	uint8 Test(const Primitive*) const					{return 0;}

	//Intersections
	uint8 TestSphere(const Sphere&) const				{return 0;}
	uint8 TestCone(const Cone&) const					{return 0;}
	uint8 TestOBB(const OBB&) const						{return 0;}
	uint8 TestFrustum(const Frustum&) const				{return 0;}
	uint8 TestPoint(const Point4&) const				{return 0;}
	uint8 TestPlane(const Plane4&) const				{return 0;}
	uint8 TestLine(const Line4&) const					{return 0;}
	uint8 TestRay(const Ray4&) const					{return 0;}
	uint8 TestLineSegment(const LineSegment4&) const	{return 0;}
	uint8 TestCircle(const Circle4&) const				{return 0;}

	//--------------------------------------------------------------------------------
	//		From Object
	//--------------------------------------------------------------------------------
public:
	
	//Transformations do nothing by default
	void Transform(const VQS&)		{}
	void TransformQuick(const VQS&) {}

	//--------------------------------------------------------------------------------
	//		From BaseClass
	//--------------------------------------------------------------------------------
public:

	//Create a deep copy of the object
	Plane4* clone() const {return new Plane4(*this);}

private:
	Vector4 normal;
	float offset;

	
public:
	//OBB Plane intersecion
	friend uint8 TestOBBPlane(const OBB& bb, const Plane4& p);
	friend uint8 TestFrustumOBB(const Frustum&, const OBB&);
};


//--------------------------------------------------------------------------------
//		INLINES	
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//		Returns the distance from a point to the plane.
//		Negative is behind the normal vector
//		Positive is in front of normal vector
//--------------------------------------------------------------------------------
inline float Plane4::Test(const Point4& point) const
{
	return point.Dot(normal) + offset;
}	//End: Plane4::Test()


//--------------------------------------------------------------------------------
//		Returns distance to a point
//--------------------------------------------------------------------------------
inline float Plane4::Distance(const Point4& point) const
{
	return DgAbs(point.Dot(normal) + offset);

}	//End: Plane4::Distance()


//--------------------------------------------------------------------------------
//		Function Declarations
//--------------------------------------------------------------------------------

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
uint8 TestLinePlane(const Line4& line, const Plane4& plane, float& u);


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
uint8 TestPlaneRay(const Plane4& plane, const Ray4& ray, float& u);


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
uint8 TestLineSegmentPlane(const LineSegment4& segment, const Plane4& plane, float& u);

#endif