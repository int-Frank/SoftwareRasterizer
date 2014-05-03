//================================================================================
// @ Frustum.h
// 
// Class: Frustum
//
// The frustum is a rectangular-based pyramid with a truncated top. It is defined 
// by six planes and a point of origin. All transformations are with respect
// to this point.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "DgTypes.h"
#include "Plane4.h"
#include "Point4.h"
#include "Primitive3D.h"

class BasisR3;
class OBB;
class Sphere;

//--------------------------------------------------------------------------------
//	@	Frustum
//--------------------------------------------------------------------------------
class Frustum : public Primitive3D
{
public:
	
	static const uint8 NUMFACES = 6;
	static const uint8 INSIDE = 128;
	static const uint8 OUTSIDE = 0;
	static const uint8 ALL_PLANES = 255;
	static const uint8 ALL_BUT_FAR = 253;

	//Constructor / destructor
	Frustum() {}
	~Frustum() {}

	////Copy operations
	Frustum(const Frustum&);
	Frustum& operator= (const Frustum&);

	//VQS manipulators
	void Build(const Point4& p0, const BasisR3& basis, 
					float near_z, float far_z,
					float w, float h);

	const Plane4& GetPlane(uint8 i) const { return planes[i]; }

	bool TestParticle(const Point4&, float radius) const;
	
	//--------------------------------------------------------------------------------
	//		From Primitive3D
	//--------------------------------------------------------------------------------
public:
	
	bool IsInside(const Point4& p) const;

	//--------------------------------------------------------------------------------
	//		From Primitive
	//--------------------------------------------------------------------------------
public:

	Point4 ClosestPoint (const Point4&) const {return origin;}
	
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
	void Transform(const VQS&) {}
	void TransformQuick(const VQS&) {}

	//--------------------------------------------------------------------------------
	//		From BaseClass
	//--------------------------------------------------------------------------------
public:

	//Create a deep copy of the object
	Frustum* clone() const {return new Frustum(*this);}

private:

	//Data
	Plane4 planes[NUMFACES];			//near, far, top, bottom, left, right.
	Point4 origin;

	void init(const Frustum&);
public:
	
	//--------------------------------------------------------------------------------
	//		Collision functions
	//--------------------------------------------------------------------------------
	friend uint8 TestFrustumOBB(const Frustum&, const OBB&);
	friend uint8 TestFrustumSphere(const Frustum&, const Sphere&);
	friend uint8 TestFrustumOBBQuick(const Frustum&, const OBB&);
	friend uint8 TestFrustumSphereQuick(const Frustum&, const Sphere&);
};


#endif