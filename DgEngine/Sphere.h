//================================================================================
// @ Sphere.h
// 
// Class: Sphere
//
// A circle can be defined by the set of all points p at distance <= r away from 
// a central point c. It is implemented simply by defining a point and radius.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef SPHERE_H
#define SPHERE_H

#include "Point4.h"
#include "DgTypes.h"
#include "Dg_io.h"
#include "Primitive3D.h"
#include <assert.h>

class Vector4;
class Ray4;
class LineSegment4;
class VQS;
class OBB;

//--------------------------------------------------------------------------------
//	@	Sphere
//--------------------------------------------------------------------------------
class Sphere : public Primitive3D
{
public:
	//Constructor/Destructor
	Sphere(): center(Point4::origin), radius(1.0f) {}
	Sphere(const Point4&, float);
	~Sphere() {}

	//Copy operations
	Sphere(const Sphere& s): 
		center(s.center), radius(s.radius) {}
	Sphere& operator= (const Sphere&);

	//Comparison 
	bool operator== (const Sphere&) const;
	bool operator!= (const Sphere&) const;

	//Input/output
	friend DgReader& operator>> (DgReader& in, Sphere& c);
	friend DgWriter& operator<< (DgWriter& out, const Sphere& c);

	//Set functions
	inline void SetCenter(const Point4&);
	void Translate(const Vector4&);
	inline void SetRadius(float val);
	inline void Set(const Point4&, float val);
	void Set(const OBB&);

	//Return functions
	const Point4& Center()	const {return center;}
	float Radius()			const {return radius;}
	float SqRadius()		const {return radius*radius;}

	inline void Clean();
	
	//--------------------------------------------------------------------------------
	//		From Primitive3D
	//--------------------------------------------------------------------------------
public:
	
	bool IsInside(const Point4& p) const;

	//--------------------------------------------------------------------------------
	//		From Primitive
	//--------------------------------------------------------------------------------
public:

	Point4 ClosestPoint (const Point4&) const;
	
	//General Interaction functions
	uint8 Test(const Primitive*) const					{return 0;}

	//Intersections
	uint8 TestSphere(const Sphere&) const;
	uint8 TestCone(const Cone&) const;
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
	Sphere* clone() const {return new Sphere(*this);}


private:

	//Data members
	Point4 center;
	float radius;

};


//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Sphere::SetCenter()
//--------------------------------------------------------------------------------
//		Set center
//--------------------------------------------------------------------------------
inline void Sphere::SetCenter(const Point4& p)
{
	center = p;

}	//End: Sphere::SetCenter()


//--------------------------------------------------------------------------------
//	@	Sphere::SetRadius()
//--------------------------------------------------------------------------------
//		Set radius
//--------------------------------------------------------------------------------
inline void Sphere::SetRadius(float val)
{
	assert(val >= 0.0f);
	radius = val;

}	//End: Sphere::SetRadius()


//--------------------------------------------------------------------------------
//	@	Sphere::Set()
//--------------------------------------------------------------------------------
//		Set all data
//--------------------------------------------------------------------------------
inline void Sphere::Set(const Point4& p, float val)
{
	center = p;
	SetRadius(val);

}	//End: Sphere::Set()


//--------------------------------------------------------------------------------
//	@	Sphere::Clean()
//--------------------------------------------------------------------------------
//		Set near-zero values to zero
//--------------------------------------------------------------------------------
inline void Sphere::Clean()
{
	center.Clean();
	if (radius < 0.0f)
		radius = 0.0f;

}	//End: Sphere::Clean()


//--------------------------------------------------------------------------------
//		Function Declarations
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//	@	TestSphereSphere()
//--------------------------------------------------------------------------------
/*		Summary: Tests if two spheres are intersecting
			--------------------------------------
		Post:	
		Case - Intersection: returns 1, sqDist set.
		Case - No Intersection: returns 0, sqDist set.
			--------------------------------------
		Param<s1>:	input Sphere
		Param<s2>:	input Sphere
		Param<sqDist>: Distance squared between the sphere centers
*/
//--------------------------------------------------------------------------------
uint8 TestSphereSphere(const Sphere&, const Sphere&, float& sqDist);


//--------------------------------------------------------------------------------
//	@	TestOBBSphere()
//--------------------------------------------------------------------------------
/*		Summary: Tests if a Sphere and OBB are colliding
			--------------------------------------
		Post:	
		Case - Intersection: returns 1, sqDist set.
		Case - No Intersection: returns 0, sqDist set.
			--------------------------------------
		Param<s>:	input Sphere
		Param<b>:	input OBB
		Param<sqDist>: Distance squared between teh sphere and OBB
*/
//--------------------------------------------------------------------------------
uint8 TestOBBSphere(const OBB& b, const Sphere& s, float& sqDist);


//--------------------------------------------------------------------------------
//	@	TestSpherePlane()
//--------------------------------------------------------------------------------
/*		Summary: Tests if a Sphere intersects a plane
			--------------------------------------
		Post:	
		Case - No Intersection, sphere on +ve side: returns 0, d set.
		Case - No Intersection, sphere on -ve side: returns 1, d set.
		Case - Intersection: returns 2, d set.
			--------------------------------------
		Param<s>:	input Sphere
		Param<p>:	input plane
		Param<d>:	Distance between the sphere and the plane
*/
//--------------------------------------------------------------------------------
uint8 TestPlaneSphere(const Sphere& s, const Plane4& p, float& d);



#endif