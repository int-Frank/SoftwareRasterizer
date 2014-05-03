//================================================================================
// @ Line4.h
// 
// Class: Line4
//
// A Line is defined by a point and direction extending infinitely in both
// directions. It is represented by a Point4 and a normalized Vector4. This file
// also declares methods to test Lines against other geometric entities.
//
// -------------------------------------------------------------------------------
//
// original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef LINE4_H
#define LINE4_H

#include "Vector4.h"
#include "Point4.h"
#include "Dg_io.h"
#include "CommonMath.h"
#include "Primitive1D.h"

class Line4 : public Primitive1D
{
public:
	//constructor/destructor
	Line4();
	Line4(const Point4& origin, const Vector4& direction);
	inline ~Line4() {}

	//Output
	friend DgWriter& operator<<(DgWriter& out, const Line4& source);
	
	//copy operations
	Line4(const Line4& other);
	Line4& operator=(const Line4& other);

	//accessors
	inline const Point4& Origin() const {return origin;}
	inline const Vector4& Direction() const {return direction;}
	void Get(Point4& O, Vector4& D) const;

	//comparison
	bool operator== (const Line4&) const;
	bool operator!= (const Line4&) const;

	//manipulators
	inline void Set(const Point4& O, const Vector4& D);
	inline void Clean() {origin.Clean(); direction.Clean();}
	
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
	void Transform(const VQS&)		{}
	void TransformQuick(const VQS&) {}

	//--------------------------------------------------------------------------------
	//		From BaseClass
	//--------------------------------------------------------------------------------
public:

	//Create a deep copy of the object
	Line4* clone() const {return new Line4(*this);}


private:

	//Data members
	Point4 origin;
	Vector4 direction;
};


//--------------------------------------------------------------------------------
//		INLINES
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Line4::Set()
//--------------------------------------------------------------------------------
//		Set origin and Destination
//--------------------------------------------------------------------------------
inline void Line4::Set(const Point4& O, const Vector4& D)
{
	//Assign
	origin = O;
	direction = D;

	direction.Normalize();

}	//End: Line4::Set()


//--------------------------------------------------------------------------------
//		Function Declarations
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	SqDistLineLine()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between two lines
			--------------------------------------
		Post:	
		Case - Lines not parallel: returns 0. sqDist set, u1 and u2 are set.
		Case - Lines parallel: returns 1. sqDist set, u1 and u2 are set with u1 = 0.
			--------------------------------------
		Param<L1>:	 input line
		Param<L2>:	 input line
		Param<sqDist>: squared distance between the lines
		Param<u1>:	 distance along L1 to the intersection or 
		             closest point to L2
		Param<u2>:	 distance along L2 to the intersection or 
		             closest point to L1
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineLine(const Line4&, const Line4&, 
					 float& sqDist, float& u1, float& u2);


//--------------------------------------------------------------------------------
//	@	SqDistLinePoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a point and a line
			--------------------------------------
		Post: returns 0. sqDist set, u set.
			--------------------------------------
		Param<L>:	 input line
		Param<P>:	 input point
		Param<sqDist>: squared distance from the point to the line
		Param<u>:	 distance along L to the closest point to P
*/
//--------------------------------------------------------------------------------
uint8 SqDistLinePoint(const Line4&, const Point4&, float& sqDist, float& u);


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineLine()
//--------------------------------------------------------------------------------
/*		Summary: Finds closest points between two Lines
			--------------------------------------
		Post:	
		Case Lines not parallel: returns 0. P1, P2 set.
		Case Lines parallel: returns 1. P1, P2 set with P1 = L1.origin.
			--------------------------------------
		Param<L1>: input line
		Param<L2>: input line
		Param<P1>: the point on L1 that is closest to L2
		Param<P2>: the point on L2 that is closest to L1
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineLine(const Line4&, const Line4&, 
							Point4& point1, Point4& point2);


//--------------------------------------------------------------------------------
//	@	DistLineLine()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between two lines
			--------------------------------------
		Post:	
		Case - Lines not parallel: returns 0. dist set, u1 and u2 are set.
		Case - Lines parallel: returns 1. dist set, u1 and u2 are set with u1 = 0.
			--------------------------------------
		Param<L1>:	 input line
		Param<L2>:	 input line
		Param<dist>: distance between the lines
		Param<u1>:	 distance along L1 to the intersection or 
		             closest point to L2
		Param<u2>:	 distance along L2 to the intersection or 
		             closest point to L1
*/
//--------------------------------------------------------------------------------
inline uint8 DistLineLine(	const Line4& line1, const Line4& line2,
						  float& dist, float& u1, float& u2)
{
	//Perform test
	uint8 result = SqDistLineLine(line1, line2, dist, u1, u2);

	//Root the distance
	DgSqrt(dist);

	return result;


}	//End: DistLineLine()


//--------------------------------------------------------------------------------
//	@	DistLinePoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a point and a line
			--------------------------------------
		Post: returns 0. sqDist set, u set.
			--------------------------------------
		Param<L>:	 input line
		Param<P>:	 input point
		Param<dist>: squared distance from the point to the line
		Param<u>:	 distance along L to the closest point to P
*/
//--------------------------------------------------------------------------------
inline uint8 DistLinePoint(	const Line4& line, const Point4& point, 
						   float& dist, float& u)
{
	//Perform test
	uint8 result = SqDistLinePoint(line, point, dist, u);

	//Root the distance
	DgSqrt(dist);

	return result;

}	//End: DistLinePoint()

#endif