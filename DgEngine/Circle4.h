//================================================================================
// @ Circle4.h
// 
// Class: Circle4
//
// A circle can be defined by the set of all points p at distance r away from 
// a central point c, which satisfy the condition (p_i - c) dot v = 0, where v is 
// an orthogonal vector to the circle. Not to be confused with a disk which is
// all points <= r.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef CIRCLE4_H
#define CIRCLE4_H

#include "DgTypes.h"
#include "CommonMath.h"
#include "BasisR3.h"
#include "Point4.h"
#include "Dg_io.h"
#include "Primitive1D.h"
#include <assert.h>

class VQS;
class Sphere;

//--------------------------------------------------------------------------------
//	@	Circle4
//--------------------------------------------------------------------------------
class Circle4 : public Primitive1D
{
public:
	//Constructor / destructor
	Circle4(): center(Point4::origin), radius(1.0f) {}
	Circle4(const Point4&, const Vector4&, float);
	~Circle4() {}

	//Copy operations
	Circle4(const Circle4&);
	Circle4& operator= (const Circle4&);
	
	//Mutators
	void SetCenter(const Point4& p)		{center = p;}
	void SetBasis(const BasisR3& b)		{basis = b;}
	void SetRadius(float r)				{assert(r >= 0.0f); radius = r;}
	void Set(const Point4&, const Vector4&, float);

	//Accessors
	const Point4& Center() const {return center;}
	const BasisR3& Basis() const {return basis;}
	float Radius() const {return radius;}
	
	//--------------------------------------------------------------------------------
	//		From Primitive1D
	//--------------------------------------------------------------------------------
public:
	
	//Entry Point
	uint8 ClosestPoints(const Primitive1D&, Point4&, Point4&) const				{return 0;}

	//Closest points
	uint8 ClosestPointsLine(const Line4&, Point4&, Point4&) const				{return 0;}
	uint8 ClosestPointsRay(const Ray4&, Point4&, Point4&) const					{return 0;}
	uint8 ClosestPointsLineSegment(const LineSegment4&, Point4&, Point4&) const {return 0;}

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
	void Transform(const VQS&);
	void TransformQuick(const VQS&);

	//--------------------------------------------------------------------------------
	//		From BaseClass
	//--------------------------------------------------------------------------------
public:

	//Create a deep copy of the object
	Circle4* clone() const {return new Circle4(*this);}

private:
	//Data members
	Point4 center;
	BasisR3 basis;
	float radius;

};


//--------------------------------------------------------------------------------
//	@	TestCircleSphere()
//--------------------------------------------------------------------------------
/*		Summary: Tests if a circle intersects a sphere
			--------------------------------------
		Post:	
		Case - Intersection, returns 1, sqDist set.
		Case - No Intersection, returns 0, sqDist set.
			--------------------------------------
		Param<c>:	input circle
		Param<s>:	input Sphere
		Param<sqDist>: Distance squared between circle and sphere
*/
//--------------------------------------------------------------------------------
uint8 TestCircleSphere(const Circle4&, const Sphere&, float& sqDist);


//--------------------------------------------------------------------------------
//	@	SqDistCirclePoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a circle and a point
			--------------------------------------
		Post:	
		Case - No special case, returns 0. sqDist set.
		Case - All circle points are equidistant from inPoint: returns 1. sqDist set.
			--------------------------------------
		Param<inCircle>:	input circle
		Param<inPoint>:		input Point
		Param<sqDist>: squared distance between tcircle and line
*/
//--------------------------------------------------------------------------------
uint8 SqDistCirclePoint(const Circle4&, const Point4&, float& SqDist);


//--------------------------------------------------------------------------------
//	@	DistCirclePoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a circle and a point
			--------------------------------------
		Post:	
		Case - No special case, returns 0. sqDist set.
		Case - All circle points are equidistant from inPoint: returns 1. dist set.
			--------------------------------------
		Param<inCircle>:	input circle
		Param<inPoint>:		input Point
		Param<dist>:		distance between tcircle and line
*/
//--------------------------------------------------------------------------------
inline uint8 DistCirclePoint(const Circle4& inCircle, const Point4& inPoint, float& dist)
{
	uint8 result = SqDistCirclePoint(inCircle, inPoint, dist);
	dist = DgSqrt(dist);
	return result;
}

#endif