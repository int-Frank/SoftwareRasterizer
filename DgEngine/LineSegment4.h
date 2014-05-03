//================================================================================
// @ LineSegment4.h
// 
// Class: LineSegment4
//
// A Linesgment is a line that connects two points. It is represented by a Point4 
// and a Vector4, the length of the vector being the distance between the points.
// This file also declares methods to test LineSegments against other 
// geometric entities.
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

#ifndef LINESEGMENT4_H
#define LINESEGMENT4_H

#include "Vector4.h"
#include "Point4.h"
#include "Dg_io.h"
#include "CommonMath.h"
#include "Primitive1D.h"

class Line4;
class Ray4;

//--------------------------------------------------------------------------------
//	@	LineSegment4
//--------------------------------------------------------------------------------
class LineSegment4 : public Primitive1D
{
public:
	//constructor/destructor
	LineSegment4();
	LineSegment4(const Point4& p1, const Point4& p2);
	inline ~LineSegment4() {}

	//Output
	friend DgWriter& operator<<(DgWriter& out, const LineSegment4& source);

	//copy operations
	LineSegment4(const LineSegment4& other);
	LineSegment4& operator=(const LineSegment4& other);

	//accessors
	inline const Point4& Origin() const {return origin;}
	inline const Vector4& Direction() const {return direction;}
	inline Point4 GetP1() const {return origin;}
	inline Point4 GetP2() const {return origin + direction;}
	inline Point4 GetCenter() const {return origin + 0.5f*direction;}
	void Get(Point4& end1, Point4& end2) const;

	float Length() const;
	float LengthSquared() const;

	//comparison
	bool operator== (const LineSegment4&) const;
	bool operator!= (const LineSegment4&) const;

	//manipulators
	inline void Set(const Point4& O, const Point4& D);
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
	LineSegment4* clone() const {return new LineSegment4(*this);}

protected:
	Point4 origin;
	Vector4 direction;
};


//--------------------------------------------------------------------------------
//		Function Declarations
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	SqDistLineSegmentLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between two LineSegments
			--------------------------------------
		Post:	returns 1. sqDist set, u1 and u2 are set.
			--------------------------------------
		Param<LS1>:	 input linesegment
		Param<LS2>:	 input linesegment
		Param<sqDist>: squared distance between the linesegments
		Param<u1>:	 distance along LS1 to the intersection or 
		             closest point to LS2
		Param<u2>:	 distance along LS2 to the intersection or 
		             closest point to LS1
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineSegmentLineSegment(const LineSegment4& LS1, const LineSegment4& LS2, 
								   float& sqDist, float& u1, float& u2);


//--------------------------------------------------------------------------------
//	@	SqDistLineLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a Line and a LineSegment.
			--------------------------------------
		Post:	returns 1. sqDist set, ul and uls are set.
			--------------------------------------
		Param<L>:	 input line
		Param<LS>:	 input linesegment
		Param<sqDist>: squared distance between the linesegments
		Param<ul>:	 distance along L to the intersection or 
		             closest point to LS
		Param<uls>:	 distance along LS to the intersection or 
		             closest point to L
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineLineSegment(const Line4& L, const LineSegment4& LS,  
							float& sqDist, float& ul, float& uls );


//--------------------------------------------------------------------------------
//	@	SqDistLineSegmentRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a LineSegment and a Ray.
			--------------------------------------
		Post:	returns 1. sqDist set, uls and ur are set.
			--------------------------------------
		Param<LS>:	 input line
		Param<R>:	 input linesegment
		Param<sqDist>: squared distance between the linesegments
		Param<uls>:	 distance along LS to the intersection or 
		             closest point to R
		Param<ur>:	 distance along R to the intersection or 
		             closest point to LS
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineSegmentRay( const LineSegment4& LS, const Ray4& R, 
						   float& sqDist, float& uls, float& ur );


//--------------------------------------------------------------------------------
//	@	SqDistLineSegmentPoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a LineSegment and a Point.
			--------------------------------------
		Post:	Case endpoint 0 is closest: returns 1. P, u set.
		 		Case endpoint 1 is closest: returns 2. P, u set.
				Case somewhere in the middle: returns 3. P, u set.
			--------------------------------------
		Param<LS>:		input linesegment
		Param<P>:		closest point is stored here
		Param<sqDist>:	distance to P
		Param<u>:		distance along LS to the closest point to P
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineSegmentPoint( const LineSegment4& LS, const Point4& P,
							  float& sqDist, float& u ) ;


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineSegmentLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Finds the closest points between two linesegments
			--------------------------------------
		Post:	returns 1. p1, p2 set.
			--------------------------------------
		Param<segment1>:	input linesegment
		Param<segment2>:	input linesegment
		Param<p1>:			point along segment1 closest to segment2
		Param<p2>:			point along segment2 closest to segment1
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineSegmentLineSegment(	const LineSegment4& segment1, 
					const LineSegment4& segment2,
					Point4& p1, Point4& p2   );


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Finds the closest points between a linesegment and a line
			--------------------------------------
		Post:	returns 1. p1, p2 set.
			--------------------------------------
		Param<segment>:	input linesegment
		Param<line>:	input line
		Param<p1>:		point along segment closest to line
		Param<p2>:		point along line closest to segment
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineLineSegment( const Line4& line, const LineSegment4& segment, 
									Point4& p1, Point4& p2);


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineSegmentRay()
//--------------------------------------------------------------------------------
/*		Summary: Finds the closest points between a linesegment and a ray
			--------------------------------------
		Post:	returns 1. p1, p2 set.
			--------------------------------------
		Param<segment>:	input linesegment
		Param<ray>:		input ray
		Param<p1>:		point along segment closest to ray
		Param<p2>:		point along ray closest to segment
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineSegmentRay( const LineSegment4& segment, const Ray4& ray, 
								   Point4& p1, Point4& p2 );




//--------------------------------------------------------------------------------
//	@	DistLineSegmentLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between two LineSegments
			--------------------------------------
		Post:	returns 1. sqDist set, u1 and u2 are set.
			--------------------------------------
		Param<LS1>:	 input linesegment
		Param<LS2>:	 input linesegment
		Param<dist>: distance between the linesegments
		Param<u1>:	 distance along LS1 to the intersection or 
		             closest point to LS2
		Param<u2>:	 distance along LS2 to the intersection or 
		             closest point to LS1
*/
//--------------------------------------------------------------------------------
inline uint8 DistLineSegmentLineSegment(const LineSegment4& LS1, const LineSegment4& LS2, 
								   float& dist, float& u1, float& u2)
{
	uint8 result = SqDistLineSegmentLineSegment(LS1, LS2, dist, u1, u2);
	dist = DgSqrt(dist);
	return result;

}//	End: ::DistLineSegmentLineSegment()


//--------------------------------------------------------------------------------
//	@	DistLineLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a Line and a LineSegment.
			--------------------------------------
		Post:	returns 1. sqDist set, ul and uls are set.
			--------------------------------------
		Param<L>:	 input line
		Param<LS>:	 input linesegment
		Param<dist>: distance between the linesegments
		Param<ul>:	 distance along L to the intersection or 
		             closest point to LS
		Param<uls>:	 distance along LS to the intersection or 
		             closest point to L
*/
//--------------------------------------------------------------------------------
inline uint8 DistLineLineSegment(const Line4& L, const LineSegment4& LS,  
							float& dist, float& ul, float& uls )
{
	uint8 result = SqDistLineLineSegment(L, LS, dist, ul, uls);
	dist = DgSqrt(dist);
	return result;

}	//End: ::DistanceLineLineSegment()


//--------------------------------------------------------------------------------
//	@	DistLineSegmentRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a LineSegment and a Ray.
			--------------------------------------
		Post:	returns 1. sqDist set, uls and ur are set.
			--------------------------------------
		Param<LS>:	 input line
		Param<R>:	 input linesegment
		Param<dist>: squared distance between the linesegments
		Param<uls>:	 distance along LS to the intersection or 
		             closest point to R
		Param<ur>:	 distance along R to the intersection or 
		             closest point to LS
*/
//--------------------------------------------------------------------------------
inline uint8 DistLineSegmentRay( const LineSegment4& LS, const Ray4& R, 
						   float& dist, float& uls, float& ur )
{
	uint8 result = SqDistLineSegmentRay(LS, R, dist, uls, ur);
	dist = DgSqrt(dist);
	return result;

}//	End: ::DistLineSegmentRay()


//--------------------------------------------------------------------------------
//	@	DistLineSegmentPoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a LineSegment and a Point.
			--------------------------------------
		Post:	Case endpoint 0 is closest: returns 1. P, u set.
		 		Case endpoint 1 is closest: returns 2. P, u set.
				Case somewhere in the middle: returns 3. P, u set.
			--------------------------------------
		Param<LS>:		input linesegment
		Param<P>:		closest point is stored here
		Param<dist>:	distance to P
		Param<u>:		distance along LS to the closest point to P
*/
//--------------------------------------------------------------------------------
inline uint8 DistLineSegmentPoint( const LineSegment4& LS, const Point4& P,
							  float& dist, float& u )
{
	uint8 result = SqDistLineSegmentPoint(LS, P, dist, u);
	dist = DgSqrt(dist);
	return result;

}//	End: ::DistLineSegmentPoint()


#endif