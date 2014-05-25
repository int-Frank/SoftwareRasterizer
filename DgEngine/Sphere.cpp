//================================================================================
// @ Sphere.cpp
// 
// Description: This file contains Sphere's class definitions as well as 
// primitive testing functions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Sphere.h"
#include "Cone.h"
#include "OBB.h"
#include "CommonMath.h"
#include "Vector4.h"
#include "Plane4.h"
#include "Line4.h"
#include "Ray4.h"
#include "LineSegment4.h"
#include "VQS.h"


//--------------------------------------------------------------------------------
//	@	Sphere::Sphere()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Sphere::Sphere(const Point4& p, float r): center(p), radius(r)
{
    if (r < 0.0f)
    {
        std::cerr << "@Sphere::Sphere() -> Invalid radius: " << r << std::endl;
        r = 0.0f;
    }

}	//End: Sphere::Sphere()


//--------------------------------------------------------------------------------
//	@	Sphere::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Sphere& Sphere::operator= (const Sphere& s)
{
	center = s.center;
	radius = s.radius;

	return *this;

}	//End: Sphere::operator=()


//--------------------------------------------------------------------------------
//	@	Sphere::operator==()
//--------------------------------------------------------------------------------
//		Comparison operator
//--------------------------------------------------------------------------------
bool Sphere::operator== (const Sphere& s) const
{
	return ( center == s.center && radius == s.radius );

}	//End: Sphere::operator==()


//--------------------------------------------------------------------------------
//	@	Sphere::operator!=()
//--------------------------------------------------------------------------------
//		Comparison operator
//--------------------------------------------------------------------------------
bool Sphere::operator!= (const Sphere& s) const
{
	return ( center != s.center || radius != s.radius );

}	//End: Sphere::operator!=()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Sphere& source)
{
	out << '[' << source.center << ', ' << source.radius << ']';

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build vector from in stream (4 values)
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, Sphere& dest)
{
	//Read to temp values
    if ((in >> dest.center >> dest.radius).fail())
    {
        std::cerr << "@operator>>(HPoint) -> Bad read." << std::endl;
    }

	return in;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	Sphere::Translate()
//--------------------------------------------------------------------------------
//		Move the sphere
//--------------------------------------------------------------------------------
void Sphere::Translate(const Vector4& v)
{
	center += v;
}	//End: Sphere::Translate()


//--------------------------------------------------------------------------------
//	@	Sphere::Set()
//--------------------------------------------------------------------------------
//		Set sphere from box
//--------------------------------------------------------------------------------
void Sphere::Set(const OBB& b)
{
	center = b.m_center;

	Vector4 diagonal = (b.m_basis.x0()*b.e[0] +
						b.m_basis.x1()*b.e[1] +
						b.m_basis.x2()*b.e[2]);

	radius = diagonal.Length();

}	//End: Sphere::Set()


//--------------------------------------------------------------------------------
//	@	Sphere::ClosestPoint()
//--------------------------------------------------------------------------------
//		Returns closest point from a point to a sphere
//--------------------------------------------------------------------------------
Point4 Sphere::ClosestPoint(const Point4& p) const
{
	Vector4 v = p - center;
	float lengthSq = v.LengthSquared();

	if (lengthSq <= radius*radius)
	{
		return p;
	}

	return center + radius * v/DgSqrt(lengthSq);

}	//End: Sphere::ClosestPoint()


//--------------------------------------------------------------------------------
//	@	Sphere::Transform()
//--------------------------------------------------------------------------------
//		Transform with a VQS
//--------------------------------------------------------------------------------
void Sphere::Transform(const VQS& vqs)
{
	center *= vqs;
	radius *= vqs.S();

}	//End: Sphere::Transform()


//--------------------------------------------------------------------------------
//	@	Sphere::TransformQuick()
//--------------------------------------------------------------------------------
//		Transform quickly with a VQS
//--------------------------------------------------------------------------------
void Sphere::TransformQuick(const VQS& vqs)
{
	center *= vqs;
	radius *= vqs.S();

}	//End: Sphere::TransformQuick()


//--------------------------------------------------------------------------------
//	@	Sphere::TestSphere()
//--------------------------------------------------------------------------------
//		Sphere-Sphere test
//--------------------------------------------------------------------------------
uint8 Sphere::TestSphere(const Sphere& s) const
{
	float dummy;
	return TestSphereSphere(s, *this, dummy);
}	//End: Sphere::TestSphere()


//--------------------------------------------------------------------------------
//	@	Sphere::TestCone()
//--------------------------------------------------------------------------------
//		Sphere-Cone test
//--------------------------------------------------------------------------------
uint8 Sphere::TestCone(const Cone& c) const
{
	return TestConeSphere(c, *this);
}	//End: Sphere::TestSphere()


//--------------------------------------------------------------------------------
//	@	Sphere::IsInside()
//--------------------------------------------------------------------------------
//		Tests if a point is inside the sphere
//--------------------------------------------------------------------------------
bool Sphere::IsInside(const Point4& p) const
{
	return (p - center).LengthSquared() <= radius * radius;
}	//End: Sphere::IsInside()


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
uint8 TestSphereSphere(const Sphere& s1, const Sphere& s2, float& sqDist)
{
	//Add radii
	float sqSumRadii = s1.Radius() + s2.Radius();
	sqSumRadii *= sqSumRadii;	
	
	//Distance^2 between sphere centers
	sqDist = (s2.Center() - s1.Center()).LengthSquared();

	if (sqDist <= sqSumRadii)			//if intersection
	{
		return 1;
	}

	return 0;					//no intersection

}	//End: TestSphereSphere()


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
uint8 TestOBBSphere(const OBB& b, const Sphere& s, float& sqDist)
{
	//Find closest point on the OBB to the sphere
	Point4 closestPoint = b.ClosestPoint(s.Center());

	Vector4 v = closestPoint - s.Center();
	sqDist = v.LengthSquared();
	if (sqDist <= s.SqRadius())
		return 1;

	return 0;

}	//End: TestOBBSphere()


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
uint8 TestPlaneSphere(const Sphere& s, const Plane4& p, float& d)
{
	//Distance center to plane
	float val = p.Test(s.Center());
	d = DgAbs(val);

	if (d <= s.Radius())	//Intersecting
	{
		return 2;
	}

	if (val > 0.0f)						//Not intersecting +ve side
		return 0;

	return 1;							//Not intersecting -ve side

}	//End: TestPlaneSphere()