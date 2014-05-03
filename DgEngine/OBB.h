//================================================================================
// @ OBB.h
// 
// Class: OBB
//
// Orientated Bounding Box. A 3D rectangular prism that can be rotated and
// transformed. It is defined by an orientation (basis), center and length
// of the three sides. Typically used as a bounding geometry.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef OBB_H
#define OBB_H

#include "Point4.h"
#include "BasisR3.h"
#include "DgTypes.h"
#include "Tuple.h"
#include "Primitive3D.h"

class VQS;

//--------------------------------------------------------------------------------
//	@	OBB
//--------------------------------------------------------------------------------
class OBB : public Primitive3D
{
	friend class Sphere;
public:
	//Constructor/Destructor
	OBB(){}
	OBB(const Point4& c, const BasisR3& b, const Tuple<float>& l):
		m_center(c), m_basis(b), e(l) {}
	~OBB(){}

	//Copy operations, default are ok
	OBB(const OBB& b): m_center(b.m_center), m_basis(b.m_basis), e(b.e) {}
	OBB& operator=(const OBB&);

	//! Build from xml_node
	friend pugi::xml_node& operator>>(pugi::xml_node&, OBB& dest);

	//Manipulate box
	void Set(const Point4& c, const BasisR3& b, const Tuple<float>& l);
	
	//Generate a random point inside the box
	Point4 GetPointInside();

	//Getters
	const Point4& center() const {return m_center;}
	const BasisR3& basis() const {return m_basis;}
	const Tuple<float>& halfLengths() const {return e;}
	
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
	OBB* clone() const {return new OBB(*this);}


private:
	//Data members
	Point4 m_center;
	BasisR3 m_basis;
	Tuple<float> e;	//half lengths

public:
	//OBB Plane intersecion
	friend uint8 TestOBBPlane(const OBB& bb, const Plane4& p);
	friend uint8 TestFrustumOBB(const Frustum& f, const OBB& b);
};


//--------------------------------------------------------------------------------
//	@	TestOBBOBB()
//--------------------------------------------------------------------------------
//		Test OBB and OBB for collision
//--------------------------------------------------------------------------------
uint8 TestOBBOBB(const OBB&, const OBB&);

#endif