//================================================================================
// @ Vector4.cpp
// 
// Description: This file defines Vector4's methods
//
// -------------------------------------------------------------------------------
//
// Original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Vector4.h"
#include "CommonMath.h"
#include "VQS.h"
#include "SimpleRNG.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

const Vector4 Vector4::xAxis( 1.0f, 0.0f, 0.0f );
const Vector4 Vector4::yAxis( 0.0f, 1.0f, 0.0f );
const Vector4 Vector4::zAxis( 0.0f, 0.0f, 1.0f );
const Vector4 Vector4::origin( 0.0f, 0.0f, 0.0f );


//-------------------------------------------------------------------------------
//	@	Vector4::Vector4()
//-------------------------------------------------------------------------------
//		Copy constructor
//-------------------------------------------------------------------------------
Vector4::Vector4(const Vector4& other) : 
	HPoint(other.x, other.y, other.z, 0.0f)
{

}   // End:  Vector4::Vector4()


//-------------------------------------------------------------------------------
//	@	Vector4::operator=()
//-------------------------------------------------------------------------------
//		Assignment operator
//-------------------------------------------------------------------------------
Vector4& Vector4::operator=(const Vector4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    
    return *this;

}   // End:  Vector4::operator=()
   

//-------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build vector from in stream (3 values)
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, Vector4& dest)
{
	if ((in >> dest.x >> dest.y >> dest.z).fail())
    {
        std::cerr << "@operator>>(Vector4) -> Bad read." << std::endl;
    }
    else
    {
        dest.w = 0.0f;
    }

	return in;

}	//End: operator>>()


//-------------------------------------------------------------------------------
//	@	Vector4::Length()
//-------------------------------------------------------------------------------
//		Vector length
//-------------------------------------------------------------------------------
float Vector4::Length() const
{
    return DgSqrt( x*x + y*y + z*z);

}   // End:  Vector4::Length()


//-------------------------------------------------------------------------------
//	@	Vector4::LengthSquared()
//-------------------------------------------------------------------------------
//		Vector length squared (avoids square root)
//-------------------------------------------------------------------------------
float Vector4::LengthSquared() const
{
    return ( x*x + y*y + z*z );

}   // End:  Vector4::LengthSquared()


//-------------------------------------------------------------------------------
//	@	Vector4::operator==()
//-------------------------------------------------------------------------------
//		Comparison operator
//-------------------------------------------------------------------------------
bool Vector4::operator==( const Vector4& other ) const
{
    if ( ::DgAreEqual( other.x, x )
        && ::DgAreEqual( other.y, y )
        && ::DgAreEqual( other.z, z ) )
        return true;

    return false;   

}   // End:  Vector4::operator==()


//-------------------------------------------------------------------------------
//	@	Vector4::operator!=()
//-------------------------------------------------------------------------------
//		Comparison operator
//-------------------------------------------------------------------------------
bool Vector4::operator!=( const Vector4& other ) const
{
    if ( ::DgAreEqual( other.x, x )
        && ::DgAreEqual( other.y, y )
        && ::DgAreEqual( other.z, z ) )
        return false;

    return true;

}   // End:  Vector4::operator!=()


//-------------------------------------------------------------------------------
//	@	Vector4::IsZero()
//-------------------------------------------------------------------------------
//		Check for zero vector
//-------------------------------------------------------------------------------
bool Vector4::IsZero() const
{
    return ::IsZero(x*x + y*y + z*z);

}   // End:  Vector4::IsZero()


//-------------------------------------------------------------------------------
//	@	Vector4::IsUnit()
//-------------------------------------------------------------------------------
//		Check for unit vector
//-------------------------------------------------------------------------------
bool Vector4::IsUnit() const
{
    return ::IsZero(1.0f - x*x - y*y - z*z);

}   // End:  Vector4::IsUnit()


//-------------------------------------------------------------------------------
//	@	Vector4::Clean()
//-------------------------------------------------------------------------------
//		Set elements close to zero equal to zero
//-------------------------------------------------------------------------------
void Vector4::Clean()
{
    if ( ::IsZero( x ) )
        x = 0.0f;
    if ( ::IsZero( y ) )
        y = 0.0f;
    if ( ::IsZero( z ) )
        z = 0.0f;

}   // End:  Vector4::Clean()


//-------------------------------------------------------------------------------
//	@	Vector4::Normalize()
//-------------------------------------------------------------------------------
//		Set to unit vector
//-------------------------------------------------------------------------------
void Vector4::Normalize()
{
    float lengthsq = x*x + y*y + z*z;

    if ( ::IsZero( lengthsq ) )
    {
        Zero();
    }
    else
    {
        float factor = DgInvSqrt( lengthsq );
        x *= factor;
        y *= factor;
        z *= factor;
    }

}   // End:  Vector4::Normalize()


//-------------------------------------------------------------------------------
//	@	Vector4::operator+()
//-------------------------------------------------------------------------------
//		Add vector to self and return
//-------------------------------------------------------------------------------
Vector4 Vector4::operator+( const Vector4& other ) const
{
    return Vector4( x + other.x, y + other.y, z + other.z );

}   // End:  Vector4::operator+()


//-------------------------------------------------------------------------------
//	@	Vector4::operator+=()
//-------------------------------------------------------------------------------
//		Add vector to self, store in self
//-------------------------------------------------------------------------------
Vector4& Vector4::operator+=( const Vector4& other )
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;

}   // End:  Vector4::operator+=()


//-------------------------------------------------------------------------------
//	@	Vector4::operator-()
//-------------------------------------------------------------------------------
//		Subtract vector from self and return
//-------------------------------------------------------------------------------
Vector4 Vector4::operator-( const Vector4& other ) const
{
    return Vector4( x - other.x, y - other.y, z - other.z );

}   // End:  Vector4::operator-()


//-------------------------------------------------------------------------------
//	@	Vector4::operator-=()
//-------------------------------------------------------------------------------
//		Subtract vector from self, store in self
//-------------------------------------------------------------------------------
Vector4& Vector4::operator-=( const Vector4& other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;

}   // End:  Vector4::operator-=()


//-------------------------------------------------------------------------------
//	@	Vector4::operator-()
//--------------------------------------------------------------------------------
//		Negate self and return; w is unchanged
//--------------------------------------------------------------------------------
Vector4 Vector4::operator- () const
{
	return Vector4(-x, -y, -z);

}	//End: Vector3::operator-()


//-------------------------------------------------------------------------------
//	@	Vector4::operator*()
//-------------------------------------------------------------------------------
//		Scalar multiplication
//-------------------------------------------------------------------------------
Vector4 Vector4::operator*( float scalar ) const
{
    return Vector4( scalar*x, scalar*y, scalar*z );

}   // End:  operator*()


//-------------------------------------------------------------------------------
//	@	Vector4::operator*()
//-------------------------------------------------------------------------------
//		Scalar multiplication
//-------------------------------------------------------------------------------
Vector4 operator*( float scalar, const Vector4& vector )
{
    return Vector4(scalar*vector.x, scalar*vector.y, scalar*vector.z);

}   // End:  operator*()


//-------------------------------------------------------------------------------
//	@	Vector4::operator*=()
//-------------------------------------------------------------------------------
//		Scalar multiplication by self
//-------------------------------------------------------------------------------
Vector4& Vector4::operator*=( float scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;

}   // End:  Vector4::operator*=()


//-------------------------------------------------------------------------------
//	@	Vector4::operator/=()
//-------------------------------------------------------------------------------
//		Scalar division
//-------------------------------------------------------------------------------
Vector4 Vector4::operator/( float scalar ) const
{
    return Vector4( x/scalar, y/scalar, z/scalar );

}   // End:  operator/=()


//-------------------------------------------------------------------------------
//	@	Vector4::operator/=()
//-------------------------------------------------------------------------------
//		Scalar division by self
//-------------------------------------------------------------------------------
Vector4& Vector4::operator/=( float scalar )
{
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;

}   // End:  Vector4::operator/=()


//-------------------------------------------------------------------------------
//	@	Vector4::operator*=()
//--------------------------------------------------------------------------------
//		Transform self from a VQS
//--------------------------------------------------------------------------------
Vector4& Vector4::operator*=( const VQS& vqs)
{
	//Scale
	x *= vqs.s;
	y *= vqs.s;
	z *= vqs.s;

	//Rotate;
	vqs.q.RotateSelf(*this);

	//Translate: WE DON'T TRANSLATE VECTORS

	return *this;

}	//End: VQS::operator*=()


//-------------------------------------------------------------------------------
//	@	Vector4::operator*()
//--------------------------------------------------------------------------------
//		Transform using a VQS, return Vector4
//--------------------------------------------------------------------------------
Vector4 Vector4::operator*( const VQS& vqs)
{
	//Temp and scale
	Vector4 temp(x*vqs.s, y*vqs.s, z*vqs.s);

	//Rotate;
	 vqs.q.RotateSelf(temp);

	 return temp;

	//Translate: WE DON'T TRANSLATE VECTORS

}	//End: VQS::operator*()


//-------------------------------------------------------------------------------
//	@	Dot()
//-------------------------------------------------------------------------------
//		Dot product friend operator
//-------------------------------------------------------------------------------
float Dot( const Vector4& vector1, const Vector4& vector2 )
{
    return (vector1.x*vector2.x + 
		vector1.y*vector2.y + vector1.z*vector2.z);

}   // End:  Dot()


//-------------------------------------------------------------------------------
//	@	Cross()
//--------------------------------------------------------------------------------
//		Cross product: treats input as vectors, returns vector (w=0)
//--------------------------------------------------------------------------------
Vector4 Cross(const Vector4& v1, const Vector4& v2)
{
	return Vector4(	v1.y*v2.z - v1.z*v2.y,
					v1.z*v2.x - v1.x*v2.z,
					v1.x*v2.y - v1.y*v2.x  );

}	//End: Cross()


//--------------------------------------------------------------------------------
//	@	Vector4::Perpendicular()
//--------------------------------------------------------------------------------
//		Returns a perpendicular vector
//--------------------------------------------------------------------------------
Vector4 Vector4::Perpendicular() const
{
	if (::IsZero(x))
	{
		return Vector4(0.0f, -z, y);
	}
	else
	{
		return Vector4(-y, x, 0.0f);
	}

}	//End: Vector4::Perpendicular()


//-------------------------------------------------------------------------------
//		Returns a random unit vector
//-------------------------------------------------------------------------------
Vector4 GetRandomVector()
{
	SimpleRNG generator;

	float theta = generator.GetUniform<float>(0.0f, TwoPI);
	float phi = generator.GetUniform<float>(0.0f, PI);

	float sinTheta = DgSin(theta);

	float x = sinTheta * DgCos(phi);
	float y = sinTheta * DgSin(phi);
	float z = DgCos(theta);

	return Vector4(x, y, z);
}


//-------------------------------------------------------------------------------
//		Returns a random orthonormal vector.
//		axis must be a unit vector.
//-------------------------------------------------------------------------------
Vector4 GetRandomOrthonormalVector(const Vector4& axis)
{
	SimpleRNG generator;

	//Get a random unit vector
	Vector4 v(GetRandomVector());

	//Find the cross product, to find random orthogonal vector to the axis
	Vector4 o(Cross(axis, v));

	if (o.IsZero())
		o = axis.Perpendicular();

	o.Normalize();

	return o;
}


//-------------------------------------------------------------------------------
//		Returns a random unit vector at angle theta from the axis.
//		axis must be a unit vector.
//-------------------------------------------------------------------------------
Vector4 GetRandomConeVector(const Vector4& axis, float theta )
{
	SimpleRNG generator;

	//Find random angle [0, theta]
	float phi = generator.GetUniform<float>(0.0f, theta);

	return (DgCos(phi) * axis + DgSin(phi) * GetRandomOrthonormalVector(axis));

}