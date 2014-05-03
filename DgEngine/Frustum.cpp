//================================================================================
// @ Frustum.cpp
// 
// Description: This file contains Frustum's class definitions as well as 
// primitive testing functions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Frustum.h"
#include "Point4.h"
#include "Sphere.h"
#include "OBB.h"
#include "BasisR3.h"


//--------------------------------------------------------------------------------
//	@	Frustum::init()
//--------------------------------------------------------------------------------
//		Copy frustum from other
//--------------------------------------------------------------------------------
void Frustum::init(const Frustum& other)
{
	for (uint8 i = 0; i < NUMFACES; ++i)
		planes[i] = other.planes[i];

	origin = other.origin;

}	//End: Frustum::init()


//--------------------------------------------------------------------------------
//	@	Frustum::Frustum()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Frustum::Frustum(const Frustum& other)
{
	init(other);

}	//End: Frustum::Frustum()


//--------------------------------------------------------------------------------
//	@	Frustum::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Frustum& Frustum::operator=(const Frustum& other)
{
	init(other);

	return *this;

}	//End: Frustum::operator=()


//--------------------------------------------------------------------------------
//		Build planes from data. When building from the basis, x0 is across
//		the frustum, x1 is up and x2 is down the frustum.
//--------------------------------------------------------------------------------
void Frustum::Build(const Point4& p0, const BasisR3& basis, 
					float near_z, float far_z,
					float wd2, float hd2)
{
	//Set origin
	origin = p0;

	//near/far clipping plane
	planes[0].Set(basis.x2(), p0+basis.x2()*near_z);
	planes[1].Set(-basis.x2(), p0+basis.x2()*far_z);

	//Left and right planes
	Vector4 QP = basis.x1()*wd2;
	Vector4 OP1 = basis.x2() + QP;
	Vector4 OP2 = basis.x2() - QP;
	
	planes[2].Set(Cross(OP2,basis.x0()), p0);
	planes[3].Set(Cross(basis.x0(),OP1), p0);

	//top/bottom planes
	QP = basis.x0()*hd2;
	OP1 = basis.x2() + QP;
	OP2 = basis.x2() - QP;

	planes[4].Set(Cross(OP1,basis.x1()), p0);
	planes[5].Set(Cross(basis.x1(),OP2), p0);

}	//End: Frustum::Build()



//--------------------------------------------------------------------------------
//	@	Frustum::Distance()
//--------------------------------------------------------------------------------
//		Frustum cull a particle.
//--------------------------------------------------------------------------------
bool Frustum::TestParticle(const Point4& p, float radius) const
{
	if (planes[0].Test(p) < 0.0f)
		return false;
	if (planes[1].Test(p) < 0.0f)
		return false;
	if (planes[2].Test(p) < -radius)
		return false;
	if (planes[3].Test(p) < -radius)
		return false;
	if (planes[4].Test(p) < -radius)
		return false;
	if (planes[5].Test(p) < -radius)
		return false;

	return true;

}	//End: Frustum::IsInside()



//--------------------------------------------------------------------------------
//	@	Frustum::IsInside()
//--------------------------------------------------------------------------------
//		Tests if a point lies inside the frustum
//--------------------------------------------------------------------------------
bool Frustum::IsInside(const Point4& p) const
{
	for (uint8 i = 0; i < NUMFACES; ++i)
	{
		if (planes[i].Test(p) < 0.0f)
			return false;
	}

	return true;

}	//End: Frustum::IsInside()


//--------------------------------------------------------------------------------
//	@	TestFrustumSphere()
//--------------------------------------------------------------------------------
/*		Summary: Test for intersection between a Frustum and a Sphere
			--------------------------------------
		Output bit code:	
		0		0		0		0		0		0		0		0
		in/out	**		bottom	top 	left	right	far		near
			--------------------------------------
		Param<f>:	 input frustum
		Param<s>:	 input sphere
*/
//--------------------------------------------------------------------------------
uint8 TestFrustumSphere(const Frustum& f, const Sphere& s)
{
	uint8 result = Frustum::INSIDE;
	float dummy;

	for (uint8 i = 0; i < Frustum::NUMFACES; ++i)
	{
		switch (TestPlaneSphere(s, f.planes[i], dummy))
		{
		case 1:
			{
				return Frustum::OUTSIDE;
			}
		case 2:
			{
				result |= (1 << i);
			}
		}
	}

	return result;

}	//End: TestFrustumSphere()


//--------------------------------------------------------------------------------
//	@	TestFrustumSphereQuick()
//--------------------------------------------------------------------------------
/*		Summary: Test for intersection between a Frustum and a Sphere
		--------------------------------------
		Post:
		Output code:	0 - No intersection
						1 - Intersection
		--------------------------------------
		Param<f>:	 input frustum
		Param<s>:	 input sphere
*/
//--------------------------------------------------------------------------------
uint8 TestFrustumSphereQuick(const Frustum& f, const Sphere& s)
{
	float dummy;

	for (uint8 i = 0; i < Frustum::NUMFACES; ++i)
	{
		if (TestPlaneSphere(s, f.planes[i], dummy) == 1)
			return 0;
	}

	return 1;

}	//End: TestFrustumSphereQuick()


//--------------------------------------------------------------------------------
//	@	TestFrustumOBB()
//--------------------------------------------------------------------------------
/*		Summary: Test for intersection between a Frustum and a OBB
			--------------------------------------
		Output bit code:
		0		0		0		0		0		0		0		0
		in/out	**		bottom	top 	left	right	far		near
			--------------------------------------
		Param<f>:	 input frustum
		Param<b>:	 input obb
*/
//--------------------------------------------------------------------------------
uint8 TestFrustumOBB(const Frustum& f, const OBB& b)
{
	uint8 result = Frustum::INSIDE;

	for (uint8 i = 0; i < Frustum::NUMFACES; ++i)
	{
		switch (TestOBBPlane(b, f.planes[i]))
		{
		case 1:
			{
				return Frustum::OUTSIDE;
			}
		case 2:
			{
				result |= (1 << i);
			}
		}
	}

	return result;

}	//End: TestFrustumOBB()


//--------------------------------------------------------------------------------
//	@	TestFrustumOBBQuick()
//--------------------------------------------------------------------------------
/*		Summary: Test for intersection between a Frustum and a OBB
		--------------------------------------
		Post:
		Output code:	0 - No intersection
						1 - Intersection
		--------------------------------------
		Param<f>:	 input frustum
		Param<b>:	 input obb
*/
//--------------------------------------------------------------------------------
uint8 TestFrustumOBBQuick(const Frustum& f, const OBB& b)
{
	for (uint8 i = 0; i < Frustum::NUMFACES; ++i)
	{
		if (TestOBBPlane(b, f.planes[i]) == 1)
			return 0;
	}

	return 1;

}	//End: TestFrustumOBBQuick()
