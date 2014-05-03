//================================================================================
// @ BasisR3.cpp
// 
// Description:
//
// This files defines the methods for the BasisR3 class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "BasisR3.h"
#include "VQS.h"
#include "CommonMath.h"

//--------------------------------------------------------------------------------
//		Useful constants
//--------------------------------------------------------------------------------
const BasisR3 BasisR3::xyz(Vector4(1.0f, 0.0f, 0.0f), 
						   Vector4(0.0f, 1.0f, 0.0f),
						   Vector4(0.0f, 0.0f, 1.0f));

//--------------------------------------------------------------------------------
//	@	BasisR3::BasisR3()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
BasisR3::BasisR3(): m_x0(Vector4::xAxis), m_x1(Vector4::yAxis), m_x2(Vector4::zAxis)
{
	
}	//End: BasisR3::BasisR3()


//--------------------------------------------------------------------------------
//	@	BasisR3::BasisR3()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
BasisR3::BasisR3(const Vector4& in_x0, const Vector4& in_x1)
{
	Set(in_x0, in_x1);

}	//End: BasisR3::BasisR3()


//--------------------------------------------------------------------------------
//	@	BasisR3::BasisR3()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
BasisR3::BasisR3(const BasisR3& other): 
	m_x0(other.m_x0), m_x1(other.m_x1), m_x2(other.m_x2)
{

}	//End: BasisR3::BasisR3()


//--------------------------------------------------------------------------------
//	@	BasisR3::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
BasisR3& BasisR3::operator=(const BasisR3& other)
{
	m_x0 = other.m_x0;
	m_x1 = other.m_x1;
	m_x2 = other.m_x2;

	return *this;

}	//End: BasisR3::operator=()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build the obb from input
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, BasisR3& dest)
{
	Vector4 x0, x1;
	in >> x0 >> x1;
	dest.Set(x0, x1);
	return in;

}	//End: BasisR3::BasisR3()


//--------------------------------------------------------------------------------
//	@	BasisR3::Set()
//--------------------------------------------------------------------------------
//		Set basis from two vectors
//--------------------------------------------------------------------------------
void BasisR3::Set(const Vector4& in_x0, const Vector4& in_x1)
{
	bool is_x1_zero = in_x1.IsZero();

	//Check for zero vectors, handle separately
	if (in_x0.IsZero())
	{
		//Both x0, x1 are zero vectors
		if (is_x1_zero)
		{
			m_x0 = Vector4::xAxis;
			m_x1 = Vector4::yAxis;
			m_x2 = Vector4::zAxis;

			return;
		}
		//x0 only is zero vector
		else
		{
			//Build the basis off in_x1
			m_x0 = in_x1;
			m_x0.Normalize();

			//Set x1
			m_x1 = m_x0.Perpendicular();

			//Find perpendicular vector to x0, x1.
			m_x2 = Cross(m_x0, m_x1);

			return;
		}
	}
	//x1 only is zero vector
	else if (is_x1_zero)
	{
		//Build the basis off in_x0
		m_x0 = in_x0;
		m_x0.Normalize();

		//Set x1
		m_x1 = m_x0.Perpendicular();

		//Find perpendicular vector to x0, x1.
		m_x2 = Cross(m_x0, m_x1);

		return;
	}

	//Assign x0
	m_x0 = in_x0;
	m_x0.Normalize();

	//Calculate x2
	m_x2 = Cross(m_x0, in_x1);

	//Test to see if in_x0 and in_x1 are parallel
	if (IsZero(m_x2.LengthSquared()))
	{
		//Find a perpendicular vector
		m_x1 = m_x0.Perpendicular();
		
		//Calculate x2
		m_x2 = Cross(m_x0, m_x1);

		return;
	}
	
	//Normalize x2
	m_x2.Normalize();
	
	//Calculate x1
	m_x1 = Cross(m_x2, m_x0);

}	//End: BasisR3::Set()


//--------------------------------------------------------------------------------
//	@	BasisR3::Transform()
//--------------------------------------------------------------------------------
//		Transform basis, renormalizes after rotation.
//--------------------------------------------------------------------------------
BasisR3 BasisR3::Transform(const VQS& vqs) const
{
	//New vectors
	Vector4 temp_x0 = m_x0;
	Vector4 temp_x1 = m_x1;

	vqs.Q().RotateSelf(temp_x0);
	vqs.Q().RotateSelf(temp_x1);

	//Safely set basis
	return BasisR3(temp_x0, temp_x1);

}	//End: BasisR3::Transform()


//--------------------------------------------------------------------------------
//	@	BasisR3::TransformQuick()
//--------------------------------------------------------------------------------
//		Transform basis, no orthonormal tests
//--------------------------------------------------------------------------------
BasisR3 BasisR3::TransformQuick(const VQS& vqs) const
{
	//New vectors
	Vector4 temp_x0 = m_x0;
	Vector4 temp_x1 = m_x1;

	vqs.Q().RotateSelf(temp_x0);
	vqs.Q().RotateSelf(temp_x1);

	return BasisR3(temp_x0, temp_x1, Cross(m_x0, m_x1));

}	//End: BasisR3::TransformQuick()


//--------------------------------------------------------------------------------
//	@	BasisR3::TransformSelf()
//--------------------------------------------------------------------------------
//		Transforms self, stores in self
//--------------------------------------------------------------------------------
void BasisR3::TransformSelf(const VQS& vqs)
{
	//New Vectors
	Vector4 temp_x0 = m_x0;
	Vector4 temp_x1 = m_x1;

	vqs.Q().RotateSelf(temp_x0);
	vqs.Q().RotateSelf(temp_x1);

	Set(temp_x0, temp_x1);

}	//End: BasisR3::TransformSelf()


//--------------------------------------------------------------------------------
//	@	BasisR3::TransformSelfQuick()
//--------------------------------------------------------------------------------
//		Transform self, no orthonormal checks
//--------------------------------------------------------------------------------
void BasisR3::TransformSelfQuick(const VQS& vqs)
{
	vqs.Q().RotateSelf(m_x0);
	vqs.Q().RotateSelf(m_x1);
	m_x2 = Cross(m_x0, m_x1);

}	//End: BasisR3::TransformSelfQuick()