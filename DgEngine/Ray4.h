//================================================================================
// @ Ray4.h
// 
// Class: Ray4
//
// A Ray is defined by a point of origin and direction extending infinitely in one
// direction. It is represented by a Point4 and a normalized Vector4. This file
// also declares methods to test Rays against other geometric entities.
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

#ifndef RAY4_H
#define RAY4_H

#include "Vector4.h"
#include "Point4.h"
#include "Dg_io.h"
#include "CommonMath.h"
#include "Primitive1D.h"

class Line4;
class VQS;

//--------------------------------------------------------------------------------
//	@	Ray4
//--------------------------------------------------------------------------------
class Ray4 : public Primitive1D
{
public:
	//constructor/destructor
	Ray4();
	Ray4(const Point4& origin, const Vector4& direction);
	inline ~Ray4() {}

	//Input/Output
	friend DgWriter& operator<<(DgWriter& out, const Ray4& source);
	friend DgReader& operator>>(DgReader& in, Ray4& dest);

	//copy operations
	Ray4(const Ray4& other);
	Ray4& operator=(const Ray4& other);

	//accessors
	inline const Point4& Origin() const {return origin;}
	inline const Vector4& Direction() const {return direction;}
	void Get(Point4& O, Vector4& D) const;

	//comparison
	bool operator== (const Ray4&) const;
	bool operator!= (const Ray4&) const;

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
	void Transform(const VQS&);
	void TransformQuick(const VQS&);

	//--------------------------------------------------------------------------------
	//		From BaseClass
	//--------------------------------------------------------------------------------
public:

	//Create a deep copy of the object
	Ray4* clone() const {return new Ray4(*this);}

protected:
	Point4 origin;
	Vector4 direction;
};


//--------------------------------------------------------------------------------
//		INLINES
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Ray4::Set()
//--------------------------------------------------------------------------------
//		Set origin and Destination
//--------------------------------------------------------------------------------
inline void Ray4::Set(const Point4& O, const Vector4& D)
{
	origin = O;
	direction = D;

	direction.Normalize();

}	//End: Ray4::Set()


//--------------------------------------------------------------------------------
//		Function Definitions
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between two Rays
			--------------------------------------
		Post:	returns 1. sqDist set, u1 and u2 are set.
			--------------------------------------
		Param<R1>:	 input ray
		Param<R2>:	 input ray
		Param<sqDist>: squared distance between the lines
		Param<u1>:	 distance along R1 to the intersection or 
		             closest point to R2
		Param<u2>:	 distance along R2 to the intersection or 
		             closest point to R1
*/
//--------------------------------------------------------------------------------
uint8 SqDistRayRay( const Ray4& R1, const Ray4& R2, 
				   float& sqDist, float& u1, float& u2);


//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a Line4 and a Ray4. Based 
                 on article and code by Dan Sunday at www.geometryalgorithms.com
			--------------------------------------
		Post:	Case demon == 0: returns 2. sqDist set, u1 and u2 are set.
				Case else: returns 1. sqDist set, u1 and u2 are set.
			--------------------------------------
		Param<R>:	 input ray
		Param<L>:	 input ray
		Param<sqDist>: squared distance between the lines
		Param<ur>:	 distance along R to the intersection or 
		             closest point to L
		Param<ul>:	 distance along L to the intersection or 
		             closest point to R
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineRay( const Line4& R, const Ray4& L, 
                   float& sqDist, float& ur, float& ul );


//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a point and a ray
			--------------------------------------
		Post:	Case origin is closest point: returns 2. sqDist set, u set.
				Case else: returns 1. sqDist set, u set.
			--------------------------------------
		Param<R>:	 input ray
		Param<P>:	 input point
		Param<sqDist>: squared distance from the point to the ray
		Param<u>:	 distance along R to the closest point to P
*/
//--------------------------------------------------------------------------------
uint8 SqDistPointRay(const Point4& P, const Ray4& R, float& sqDist, float& u);


//--------------------------------------------------------------------------------
//	@	ClosestPointsRayRay()
//--------------------------------------------------------------------------------
/*		Summary: Finds closest points between two Rays
			--------------------------------------
		Post:	returns 1. P1, P2 set.
			--------------------------------------
		Param<R1>: input ray
		Param<R2>: input ray
		Param<P1>: the point on R1 that is closest to R2
		Param<P2>: the point on R2 that is closest to R1
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsRayRay(const Ray4& R1, const Ray4& R2, Point4& P1, Point4& P2);



//--------------------------------------------------------------------------------
//	@	ClosestPointsLineRay()
//--------------------------------------------------------------------------------
/*		Summary: Finds closest points between a Line and a Ray
			--------------------------------------
		Post:	returns 1. P1, P2 set.
			--------------------------------------
		Param<R>: input ray
		Param<L>: input line
		Param<P1>: the point on R that is closest to L
		Param<P2>: the point on L that is closest to R
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineRay(const Ray4& R, const Line4& L, Point4& P1, Point4& P2);


//--------------------------------------------------------------------------------
//	@	DistPointRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a point and a ray
			--------------------------------------
		Post:	Case origin is closest point: returns 2. dist set, u set.
				Case else: returns 1. dist set, u set.
			--------------------------------------
		Param<R>:	 input ray
		Param<P>:	 input point
		Param<dist>: distance from the point to the ray
		Param<u>:	 distance along R to the closest point to P
*/
//--------------------------------------------------------------------------------
inline uint8 DistPointRay(const Point4& P, const Ray4& R, float& dist, float& u)
{
	uint8 result =  SqDistPointRay(P, R, dist, u);
	dist = DgSqrt(dist);

	return result;

}	//End: SqDistPointRay();


//--------------------------------------------------------------------------------
//	@	DistLineRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between a Line4 and a Ray4.
			--------------------------------------
		Post:	Case demon == 0: returns 2. dist set, ur and ul are set.
				Case else: returns 1. dist set, ur and ul are set.
			--------------------------------------
		Param<R>:	 input ray
		Param<L>:	 input ray
		Param<dist>: distance between the lines
		Param<ur>:	 distance along R to the intersection or 
		             closest point to L
		Param<ul>:	 distance along L to the intersection or 
		             closest point to R
*/
//--------------------------------------------------------------------------------
inline uint8 DistLineRay( const Line4& L, const Ray4& R,
						   float& dist, float& ur, float& ul)
{
	uint8 result =  SqDistLineRay(L, R, dist, ul, ur);
	dist = DgSqrt(dist);

	return result;

}	//End: DistLineRay()


//--------------------------------------------------------------------------------
//	@	DistRayRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance between two Rays
			--------------------------------------
		Post:	returns 1. dist set, u1 and u2 are set.
			--------------------------------------
		Param<R1>:	 input ray
		Param<R2>:	 input ray
		Param<dist>: distance between the lines
		Param<u1>:	 distance along R1 to the intersection or 
		             closest point to R2
		Param<u2>:	 distance along R2 to the intersection or 
		             closest point to R1
*/
//--------------------------------------------------------------------------------
inline uint8 DistRayRay(const Ray4& R1, const Ray4& R2, 
				   float& dist, float& u1, float& u2)
{
	uint8 result = SqDistRayRay(R1, R2, dist, u1, u2);
	dist = DgSqrt(dist);

	return result;

}	//End: DistRayRay()


#endif