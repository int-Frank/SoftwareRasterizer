//================================================================================
// @ Cone.cpp
// 
// Description: This file contains Cone's class definitions as well as 
// primitive testing functions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Cone.h"
#include "CommonMath.h"
#include "VQS.h"
#include "Circle4.h"
#include "Sphere.h"

//--------------------------------------------------------------------------------
//	@	Cone::Cone()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Cone::Cone(): origin(Point4::origin), axis(Vector4::xAxis),
	radius(1.0f), cosTheta(0.8660254037f), sinTheta(0.5f) //theta = 30 degrees
{

}	//End: Cone::Cone()


//--------------------------------------------------------------------------------
//	@	Cone::Cone()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Cone::Cone(const Point4& p, const Vector4& a, float r, float t): 
	origin(p)
{
	SetRadius(r);
	SetAxis(a);
	SetAngle(t);
}	//End: Cone::Cone()


//--------------------------------------------------------------------------------
//	@	Cone::Cone()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Cone::Cone(const Cone& other): origin(other.origin),
	axis(other.axis), radius(other.radius),
	cosTheta(other.cosTheta), sinTheta(other.sinTheta)
{

}	//End: Cone::Cone()


//--------------------------------------------------------------------------------
//	@	Cone::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Cone& Cone::operator=(const Cone& other)
{
	origin = other.origin;
	axis = other.axis;
	radius = other.radius;
	cosTheta = other.cosTheta;
	sinTheta = other.sinTheta;

	return *this;

}	//End: Cone::operator=()


//--------------------------------------------------------------------------------
//	@	Cone::Transform()
//--------------------------------------------------------------------------------
//		VQS Transformation
//--------------------------------------------------------------------------------
void Cone::Transform(const VQS& vqs)
{
	//Translate origin
	origin *= vqs;

	//Rotate axis
	axis = vqs.Rotate(axis);
	axis.Normalize();

	//Scale radius
	radius *= vqs.S();

}	//End: Cone::Transform()


//--------------------------------------------------------------------------------
//	@	Cone::TransformQuick()
//--------------------------------------------------------------------------------
//		VQS Transformation
//--------------------------------------------------------------------------------
void Cone::TransformQuick(const VQS& vqs)
{
	//Translate origin
	origin *= vqs;

	//Rotate axis
	axis = vqs.Rotate(axis);

	//Scale radius
	radius *= vqs.S();

}	//End: Cone::TransformQuick()


//--------------------------------------------------------------------------------
//	@	Cone::SetAngle()
//--------------------------------------------------------------------------------
//		Set the cone angle
//--------------------------------------------------------------------------------
void Cone::SetAngle(float t)
{
	t = DgMax(0.0f, t);
	t = DgMin(PI, t);

	DgSinCos(t, sinTheta, cosTheta);

}	//End: Cone::SetAngle()


//--------------------------------------------------------------------------------
//	@	Cone::SetAxisSafe()
//--------------------------------------------------------------------------------
//		Set the cone axis, normalize to be safe
//--------------------------------------------------------------------------------
void Cone::SetAxisSafe(const Vector4& a)
{
	axis = a;
	axis.Normalize();

}	//End: Cone::SetAxis()


//--------------------------------------------------------------------------------
//	@	Cone::Set()
//--------------------------------------------------------------------------------
//		Set all data
//--------------------------------------------------------------------------------
void Cone::Set(const Point4& p, const Vector4& a, float r, float t)
{
	origin = p;
	SetAxis(a);
	SetRadius(r);
	SetAngle(t);

}	//End: Cone::Set()


//--------------------------------------------------------------------------------
//	@	Cone::OuterRing()
//--------------------------------------------------------------------------------
//		Get the outer ring of the Cone
//--------------------------------------------------------------------------------
Circle4 Cone::OuterRing() const
{
	return Circle4(	origin + (radius * cosTheta * axis), 
					axis, 
					radius * sinTheta);
}	//End: Cone::OuterRing()


//--------------------------------------------------------------------------------
//	@	Cone::TestSphere()
//--------------------------------------------------------------------------------
//		Cone-Sphere test
//--------------------------------------------------------------------------------
uint8 Cone::TestSphere(const Sphere& s) const
{
	return TestConeSphere(*this, s);

}	//End: Sphere::TestSphere()


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
uint8 TestConeSphere(const Cone& cone, const Sphere& sphere)
{
	//First check sphere against cone sphere
	Vector4 A = sphere.Center() - cone.Origin();
	float sumRad = cone.Radius() + sphere.Radius();
	sumRad *= sumRad;
	float sqDist = A.LengthSquared();
	if (sqDist > sumRad )
	{
		return 0;
	}

	Point4 U = cone.Origin() - (sphere.Radius()*cone.InvSinTheta())*cone.Axis();
	Vector4 D = sphere.Center() - U;
	float dsqr = Dot(D,D);
	float E = Dot(cone.Axis(), D);

	if (E > 0 && E*E >= dsqr*cone.SqCosTheta())
	{
		D = sphere.Center() - cone.Origin();
		dsqr = Dot(D, D);
		E = -Dot(cone.Axis(), D);

		if (E > 0 && E*E >= dsqr*cone.SqSinTheta())
		   return uint8(dsqr <= sphere.SqRadius());
		else
			return 1;

	}
	return 0;

}	//End: TestConeSphere()


//--------------------------------------------------------------------------------
//		Tests if a point is inside the cone
//--------------------------------------------------------------------------------
bool Cone::IsInside(const Point4& p) const
{
	Vector4 v = p - origin;
	float length = v.Length();
	float cosPhi = Dot(v/length, axis);

	return (length < radius) && (cosPhi > cosTheta);

}	//End: Sphere::IsInside()

