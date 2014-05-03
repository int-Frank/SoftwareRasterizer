//================================================================================
// @ Circle4.cpp
// 
// Description: This file contains Circle4's class definitions as well as 
// primitive testing functions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Circle4.h"
#include "Sphere.h"
#include "VQS.h"

//--------------------------------------------------------------------------------
//	@	Circle4::Circle()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Circle4::Circle4(const Point4& p, const Vector4& v, float r):
	center(p),
	basis(v, v.Perpendicular()),
	radius(r)
{

}	//End: Circle4::Circle4()


//--------------------------------------------------------------------------------
//	@	Circle4::Circle()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Circle4::Circle4(const Circle4& other): 
	center(other.center), basis(other.basis), radius(other.radius)
{

}	//End: Circle4::Circle4()


//--------------------------------------------------------------------------------
//	@	Circle4::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Circle4& Circle4::operator=(const Circle4& other)
{
	center = other.center;
	basis = other.basis;
	radius = other.radius;

	return *this;
}	//End: Circle4::operator=()


//--------------------------------------------------------------------------------
//	@	Circle4::Transform()
//--------------------------------------------------------------------------------
//		VQS Transformation
//--------------------------------------------------------------------------------
void Circle4::Transform(const VQS& vqs)
{
	//Translate center
	center = vqs.Translate(center);

	//Rotate basis
	basis.TransformSelf(vqs);

	//Scale radius
	radius *= vqs.S();

}	//End: Circle4::Transform()


//--------------------------------------------------------------------------------
//	@	Circle4::TransformQuick()
//--------------------------------------------------------------------------------
//		Quick VQS Transformation (Introduces a small amount of error)
//--------------------------------------------------------------------------------
void Circle4::TransformQuick(const VQS& vqs)
{
	//Translate center
	center = vqs.Translate(center);

	//Rotate basis
	basis.TransformSelfQuick(vqs);

	//Scale radius
	radius *= vqs.S();

}	//End: Circle4::Transform()


//--------------------------------------------------------------------------------
//	@	Circle4::Set()
//--------------------------------------------------------------------------------
//		Set a circle from a point, vector and radius
//--------------------------------------------------------------------------------
void Circle4::Set(const Point4& p, const Vector4& v, float r)
{
	center = p;
	basis.Set(v, v.Perpendicular());
	SetRadius(r);

}	//End: Circle4::Set()


//--------------------------------------------------------------------------------
//	@	Circle4::ClosestPoint()
//--------------------------------------------------------------------------------
//		Closest point on a circle to a point
//--------------------------------------------------------------------------------
Point4 Circle4::ClosestPoint(const Point4& inPoint) const
{
	// Projection of P-C onto plane is Q-C = P-C - Dot(N,P-C)*N.
    Vector4 PmC = inPoint - center;
    Vector4 QmC = PmC - Dot(basis.x0(), PmC) * basis.x0();
    float sqLengthQmC = QmC.LengthSquared();

    if (sqLengthQmC > 0.0f)
    {
        return center + radius * QmC/sqLengthQmC;
    }
    else
    {
        // All circle points are equidistant from P.  Return one of them.
        return center + radius * basis.x1();
    }

}	//End: Circle4::ClosestPoint()


//--------------------------------------------------------------------------------
//	@	SqDistCirclePoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a circle and a point
			--------------------------------------
		Post:	
		Case - No special case, returns 0. sqDist set.
		Case - All circle points are equidistant from inPoint: returns 1. sqDist set.
			--------------------------------------
		Param<inCircle>:	input circle
		Param<inPoint>:		input Point
		Param<sqDist>: squared distance between tcircle and line
*/
//--------------------------------------------------------------------------------
uint8 SqDistCirclePoint(const Circle4& inCircle, const Point4& inPoint, float& sqDist)
{
	// Projection of P-C onto plane is Q-C = P-C - Dot(N,P-C)*N.
    Vector4 PmC = inPoint - inCircle.Center();
    Vector4 QmC = PmC - Dot(inCircle.Basis().x0(), PmC) * inCircle.Basis().x0();
    float sqLengthQmC = QmC.LengthSquared();
	
	Point4 closestPoint; 
	uint8 result = 0;
    
	if (sqLengthQmC > 0.0f)
    {
        closestPoint = inCircle.Center() + inCircle.Radius() * QmC/sqLengthQmC;
    }
    else
    {
        // All circle points are equidistant from P.  Return one of them.
        closestPoint = inCircle.Center() + inCircle.Radius() * inCircle.Basis().x1();
		result = 1;
    }

	sqDist = (inPoint - closestPoint).LengthSquared();
	return result;

}	//End: SqDistCirclePoint()


//--------------------------------------------------------------------------------
//	@	TestCircleSphere()
//--------------------------------------------------------------------------------
/*		Summary: Tests if a circle intersects a sphere
			--------------------------------------
		Post:	
		Case - Intersection, returns 1, sqDist set.
		Case - No Intersection, returns 0, sqDist set.
			--------------------------------------
		Param<c>:	input circle
		Param<s>:	input Sphere
		Param<sqDist>: Distance squared between circle and sphere
*/
//--------------------------------------------------------------------------------
uint8 TestCircleSphere(const Circle4& c, const Sphere& s, float& sqDist)
{
	//Get distance between circle and sphere center
	SqDistCirclePoint(c, s.Center(), sqDist);

	if (sqDist <= s.SqRadius())
		return 1;

	return 0;

}	//End: TestCircleSphere()