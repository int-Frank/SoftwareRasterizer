//================================================================================
// @ Primitive.h
// 
// Class: Primitive
//
// All 1D, 2D and 3D primitives derive from this class. A primitive is essentially
// any geometric entity. This class contains generic Test functions for
// testing two primitives as well as specific Test functions (TestSphere() for 
// example). This allows for testing two primitives from the base class. For
// example:
//
//		Primitive *s = new Sphere;
//		Primitive *b = new OBB;
//
//		//To test s against b
//		s->Test(b);
//
//		//s will now call the correct function in b:
//		b->TestSphere(this);
//
// -------------------------------------------------------------------------------
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "DgTypes.h"
#include "Point4.h"
#include "Object.h"

class VQS;
class Sphere;
class OBB;
class Frustum;
class Cone;
class Plane4;
class Line4;
class Ray4;
class LineSegment4;
class Circle4;



//--------------------------------------------------------------------------------
//	@	Primitive
//--------------------------------------------------------------------------------
class Primitive : public Object
{
public:
	//Constructor / destructor
	Primitive() {}
	virtual ~Primitive() {}

	//Closest point on the Primitive to a point
	virtual Point4 ClosestPoint (const Point4& input) const =0;
	
	//--------------------------------------------------------------------------------
	//		Intersections and testing
	//--------------------------------------------------------------------------------

	//Entry point
	virtual uint8 Test(const Primitive*) const =0;

	//Intersections
	virtual uint8 TestSphere(const Sphere&) const =0;
	virtual uint8 TestCone(const Cone&) const =0;
	virtual uint8 TestOBB(const OBB&) const =0;
	virtual uint8 TestFrustum(const Frustum&) const =0;
	virtual uint8 TestPoint(const Point4&) const =0;
	virtual uint8 TestPlane(const Plane4&) const =0;
	virtual uint8 TestLine(const Line4&) const =0;
	virtual uint8 TestRay(const Ray4&) const =0;
	virtual uint8 TestLineSegment(const LineSegment4&) const =0;
	virtual uint8 TestCircle(const Circle4&) const =0;


	//--------------------------------------------------------------------------------
	//		Squared Distances
	//--------------------------------------------------------------------------------

	////Entry point
	//virtual uint8 SqDist(const Primitive&, float&) const =0;

	//virtual uint8 SqDistSphere(const Sphere&, float&) const =0;
	//virtual uint8 SqDistOBB(const OBB&, float&) const =0;
	//virtual uint8 SqDistFrustum(const Frustum&, float&) const =0;
	//virtual uint8 SqDistPlane(const Plane4&, float&) const =0;
	//virtual uint8 SqDistPoint(const Point4&, float&) const =0;
	//virtual uint8 SqDistLine(const Line4&, float&) const =0;
	//virtual uint8 SqDistRay(const Ray4&, float&) const =0;
	//virtual uint8 SqDistLineSegment(const LineSegment4&, float&) const =0;

	//--------------------------------------------------------------------------------
	//		Distances
	//--------------------------------------------------------------------------------

	////Entry point
	//virtual uint8 Dist(const Primitive&, float&) const =0;

	//virtual uint8 DistSphere(const Sphere&, float&) const =0;
	//virtual uint8 DistOBB(const OBB&, float&) const =0;
	//virtual uint8 DistFrustum(const Frustum&, float&) const =0;
	//virtual uint8 DistPlane(const Plane4&, float&) const =0;
	//virtual uint8 DistPoint(const Point4&, float&) const =0;
	//virtual uint8 DistLine(const Line4&, float&) const =0;
	//virtual uint8 DistRay(const Ray4&, float&) const =0;
	//virtual uint8 DistLineSegment(const LineSegment4&, float&) const =0;


};

#endif