//================================================================================
// @ Cone.h
// 
// Class: Cone
//
// A cone is defined by an origin an axis vector and an angle of deviation. 
// This implementation also defines a cut-off radius r, as opposed to an
// infinite cone or a cut-off plane.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef CONE_H
#define CONE_H

#include "DgTypes.h"
#include "Point4.h"
#include "Vector4.h"
#include "Circle4.h"
#include "Primitive3D.h"

class VQS;
class Sphere;

class Cone : public Primitive3D
{
public:
	//Constructor / destructor
	Cone();
	Cone(const Point4&, const Vector4&, float r, float t);
	~Cone(){}

	//Copy operations
	Cone(const Cone&);
	Cone& operator= (const Cone&);
	
	//Mutators
	void SetOrigin(const Point4& p)			{origin = p;}
	void SetAxisSafe(const Vector4&);
	void SetAxis(const Vector4& a)			{axis = a;}	//Assume input axis is normalized!
	void SetRadius(float r)					{assert(r >= 0.0f); radius = r;}
	void SetAngle(float r);
	void Set(const Point4&, const Vector4&, float r, float t);

	//Accessors
	const Point4& Origin()	const	{return origin;}
	const Vector4& Axis()	const	{return axis;}
	float Radius()			const	{return radius;}
	float CosTheta()		const	{return cosTheta;} 
	float SinTheta()		const	{return sinTheta;} 
	float SqCosTheta()		const	{return cosTheta * cosTheta;} 
	float SqSinTheta()		const	{return sinTheta * sinTheta;} 
	float InvSinTheta()		const	{return 1.0f / sinTheta;} 
	Circle4 OuterRing()		const;
	
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
	uint8 TestSphere(const Sphere&) const;
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
	Cone* clone() const {return new Cone(*this);}

private:
	//Data members
	Point4 origin;
	Vector4 axis;
	float radius;
	float cosTheta, sinTheta;

};

//--------------------------------------------------------------------------------
/*		Summary: Test for intersection between a Cone and Sphere
			--------------------------------------
		Post:	
		Case 0 - No Collision.
		Case 1 - Collision.
			--------------------------------------
		Param<cone>:	 input Cone
		Param<Sphere>:	 input Sphere
*/
//--------------------------------------------------------------------------------
uint8 TestConeSphere(const Cone&, const Sphere&);

#endif