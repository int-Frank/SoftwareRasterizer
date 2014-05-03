//================================================================================
// @ OBB.cpp
// 
// Description: This file contains OBB's class definitions as well as 
// primitive testing functions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "OBB.h"
#include "CommonMath.h"
#include "VQS.h"
#include "Vector4.h"
#include "Plane4.h"
#include "Matrix44.h"
#include "pugixml.hpp"
#include "SimpleRNG.h"


//--------------------------------------------------------------------------------
//	@	OBB::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
OBB& OBB::operator=(const OBB& other)
{
	m_center = other.m_center;
	m_basis = other.m_basis;
	e = other.e;

	return *this;

}	//End: OBB::operator=()



//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build obb from xml node
//-------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, OBB& dest)
{
	Tuple<float> t(1.0f, 1.0f, 1.0f);
	BasisR3 b;
	Point4 p(0.0f, 0.0f, 0.0f);

	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "lengths")
		{
			DgString(it->child_value()) >> t;
		}
		else if (tag == "basis")
		{
			DgString(it->child_value()) >> b;
		}
		else if (tag == "origin")
		{
			DgString(it->child_value()) >> p;
		}
	}

	dest.Set(p, b, t);

	return node;
}


//--------------------------------------------------------------------------------
//	@	OBB::Set()
//--------------------------------------------------------------------------------
//		Set data
//--------------------------------------------------------------------------------
void OBB::Set(const Point4& c, const BasisR3& b, const Tuple<float>& l)
{
	m_center = c;
	m_basis = b;
	e = l;

}	//End: OBB::Set()


//--------------------------------------------------------------------------------
//	@	OBB::Transform()
//--------------------------------------------------------------------------------
//		Transform Bounding OBB
//--------------------------------------------------------------------------------
void OBB::Transform(const VQS& vqs)
{
	//Transform center
	m_center *= vqs;

	//Rotate
	m_basis.TransformSelf(vqs);

	//Scale
	e *= vqs.S();

}	//End: OBB::Transform()


//--------------------------------------------------------------------------------
//	@	OBB::TransformQuick()
//--------------------------------------------------------------------------------
//		Transform Bounding OBB
//--------------------------------------------------------------------------------
void OBB::TransformQuick(const VQS& vqs)
{
	//Transform center
	m_center *= vqs;

	//Rotate
	m_basis.TransformSelfQuick(vqs);

	//Scale
	e *= vqs.S();

}	//End: OBB::TransformQuick()


//--------------------------------------------------------------------------------
//	@	OBB::GetPointInside()
//--------------------------------------------------------------------------------
//		Generate a random point inside the box
//--------------------------------------------------------------------------------
Point4 OBB::GetPointInside()
{
	SimpleRNG generator;

	float a = generator.GetUniform(-e[0], e[0]);
	float b = generator.GetUniform(-e[1], e[1]);
	float c = generator.GetUniform(-e[2], e[2]);

	return m_center + a*m_basis.x0() + b*m_basis.x1() + c*m_basis.x2();

}	//End: OBB::GetPointInside()


//--------------------------------------------------------------------------------
//	@	OBB::IsInside()
//--------------------------------------------------------------------------------
//		Is the point inside the OBB?
//--------------------------------------------------------------------------------
bool OBB::IsInside(const Point4& pt) const
{
	Vector4 d = pt - m_center;

	//For each OBB axis...
	for (uint8 i = 0; i < 3; ++i)
	{
		//...project d onto that axis to get the distance
		// along the axis of d from the box center
		float dist = Dot(d, m_basis[i]);

		//if the distance is father than the box extents, clamp to the box
		if (dist > e[i] || dist < -e[i]) 
			return true;
	}

	return false;

}	//End: OBB::IsInside()


//--------------------------------------------------------------------------------
//	@	OBB::ClosestPoint()
//--------------------------------------------------------------------------------
//		Closest point on a OBB to a point
//--------------------------------------------------------------------------------
Point4 OBB::ClosestPoint(const Point4& pt) const
{
	Vector4 d = pt - m_center;

	//Start result at center of box, make steps from there
	Point4 result = m_center;

	//For each OBB axis...
	for (uint8 i = 0; i < 3; ++i)
	{
		//...project d onto that axis to get the distance
		// along the axis of d from the box center
		float dist = Dot(d, m_basis[i]);

		//if the distance is father than the box extents, clamp to the box
		if (dist > e[i]) dist = e[i];
		if (dist < -e[i]) dist = -e[i];

		//Step that distance along th eaxis to get world coordinate
		result += dist * m_basis[i];
	}

	return result;

}	//End: OBB::ClosestPoint()


//--------------------------------------------------------------------------------
//	@	TestOBBPlane()
//--------------------------------------------------------------------------------
/*		Summary: Tests if the OBB are intersecting
			--------------------------------------
		Output code:	0 - box completly on +ve side of plane 
						1 - box completly on -ve side of plane  
						2 - box is intersecting plane
			--------------------------------------
		Param<p>:	input plane
		Param<b>:	input OBB
*/
//--------------------------------------------------------------------------------
uint8 TestOBBPlane(const OBB& b, const Plane4& p)
{
	//Compute the projection interval radius of b onto L(t) = b.m_center + t*p.n
	float r =	b.e[0] * DgAbs(Dot(p.normal, b.m_basis.x0())) +
				b.e[1] * DgAbs(Dot(p.normal, b.m_basis.x1())) +
				b.e[2] * DgAbs(Dot(p.normal, b.m_basis.x2()));

	//Compute of box m_center to plane
	float s = p.Test(b.m_center);

	//Intersection
	if (DgAbs(s) > r)
	{
		if (s > 0.0f)
			return 0;
		return 1;
	}

	return 2;

}	//End: TestOBBPlane()


//--------------------------------------------------------------------------------
//		UNFINISHED
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//		Test OBB and OBB for collision
//--------------------------------------------------------------------------------
uint8 TestOBBOBB(const OBB& a, const OBB& b)
{
	//Extract common data
	const Vector4* au = &a.basis()[0];
	const Vector4* bu = &b.basis()[0];

	return 0;

}