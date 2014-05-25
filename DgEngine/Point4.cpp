//================================================================================
// @ Point4.cpp
// 
// Description: This file defines Point4's methods
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

#include "Point4.h"
#include "Vector4.h"
#include "CommonMath.h"
#include "VQS.h"


//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------
Point4 Point4::origin( 0.0f, 0.0f, 0.0f );


//-------------------------------------------------------------------------------
//	@	Point4::Point4()
//-------------------------------------------------------------------------------
//		Copy constructor
//-------------------------------------------------------------------------------
Point4::Point4(const Point4& other) : 
	HPoint(other.x, other.y, other.z, 1.0f)
{

}   // End:  Point4::Point4()


//-------------------------------------------------------------------------------
//	@	Point4::operator=()
//-------------------------------------------------------------------------------
//		Assignment operator
//-------------------------------------------------------------------------------
Point4& Point4::operator=(const Point4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    
    return *this;

}   // End:  Point4::operator=()
   

//-------------------------------------------------------------------------------
//	@	Point4::Point4()
//--------------------------------------------------------------------------------
//		Build vector from in stream (3 values)
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, Point4& dest)
{
	//Read to temp values
    if ((in >> dest.x >> dest.y >> dest.z).fail())
    {
        std::cerr << "@operator>>(Point4) -> Bad read." << std::endl;
    }
    else
    {
        //Ensure w = 1
        dest.w = 1.0f;
    }

	return in;

}	//End: operator>>()


//-------------------------------------------------------------------------------
//	@	Point4::operator==()
//-------------------------------------------------------------------------------
//		Comparison operator
//-------------------------------------------------------------------------------
bool Point4::operator==( const Point4& other ) const
{
    if ( ::DgAreEqual( other.x, x )
        && ::DgAreEqual( other.y, y )
        && ::DgAreEqual( other.z, z ) )
        return true;

    return false;   

}   // End:  Point4::operator==()


//-------------------------------------------------------------------------------
//	@	Point4::operator!=()
//-------------------------------------------------------------------------------
//		Comparison operator
//-------------------------------------------------------------------------------
bool Point4::operator!=( const Point4& other ) const
{
    if ( ::DgAreEqual( other.x, x )
        && ::DgAreEqual( other.y, y )
        && ::DgAreEqual( other.z, z ) )
        return false;

    return true;

}   // End:  Point4::operator!=()


//-------------------------------------------------------------------------------
//	@	Point4::Clean()
//-------------------------------------------------------------------------------
//		Set elements close to zero equal to zero
//-------------------------------------------------------------------------------
void Point4::Clean()
{
    if ( ::IsZero( x ) )
        x = 0.0f;
    if ( ::IsZero( y ) )
        y = 0.0f;
    if ( ::IsZero( z ) )
        z = 0.0f;

}   // End:  Point4::Clean()


//-------------------------------------------------------------------------------
//	@	Point4::operator+()
//-------------------------------------------------------------------------------
//		Add vector to self and return
//-------------------------------------------------------------------------------
Point4 Point4::operator+( const Vector4& other ) const
{
    return Point4( x + other.x, y + other.y, z + other.z );

}   // End:  Point4::operator+()


//-------------------------------------------------------------------------------
//	@	Point4::operator+=()
//-------------------------------------------------------------------------------
//		Add vector to self, store in self
//-------------------------------------------------------------------------------
Point4& Point4::operator+=( const Vector4& other )
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;

}   // End:  Point4::operator+=()


//-------------------------------------------------------------------------------
//	@	Point4::operator-()
//-------------------------------------------------------------------------------
//		Subtract Point from self, return Vector
//-------------------------------------------------------------------------------
Vector4 Point4::operator-( const Point4& other ) const
{
    return Vector4( x - other.x, y - other.y, z - other.z );

}   // End:  Point4::operator-()


//-------------------------------------------------------------------------------
//	@	Point4::operator-()
//-------------------------------------------------------------------------------
//		Subtract vector from self, return Point
//-------------------------------------------------------------------------------
Point4 Point4::operator-( const Vector4& v ) const
{
    return Point4( x - v.x, y - v.y, z - v.z );

}   // End:  Point4::operator-()


//-------------------------------------------------------------------------------
//	@	Point4::operator-=()
//-------------------------------------------------------------------------------
//		Subtract vector from self, store in self
//-------------------------------------------------------------------------------
Point4& Point4::operator-=( const Vector4& other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;

}   // End:  Point4::operator-=()


//-------------------------------------------------------------------------------
//	@	Point4::operator-()
//--------------------------------------------------------------------------------
//		Negate self and return; w is unchanged
//--------------------------------------------------------------------------------
Point4 Point4::operator- () const
{
	return Point4(-x, -y, -z);

}	//End: Point4::operator-()


//-------------------------------------------------------------------------------
//	@	Point4::operator*()
//-------------------------------------------------------------------------------
//		Scalar multiplication
//-------------------------------------------------------------------------------
Point4 Point4::operator*( float scalar ) const
{
    return Point4( scalar*x, scalar*y, scalar*z );

}   // End:  operator*()


//-------------------------------------------------------------------------------
//	@	Point4::operator*()
//-------------------------------------------------------------------------------
//		Scalar multiplication
//-------------------------------------------------------------------------------
Point4 operator*( float scalar, const Point4& vector )
{
    return Point4(scalar*vector.x, scalar*vector.y, scalar*vector.z);

}   // End:  operator*()


//-------------------------------------------------------------------------------
//	@	Point4::operator*=()
//-------------------------------------------------------------------------------
//		Scalar multiplication by self
//-------------------------------------------------------------------------------
Point4& Point4::operator*=( float scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;

}   // End:  Point4::operator*=()


//-------------------------------------------------------------------------------
//	@	Point4::operator/()
//-------------------------------------------------------------------------------
//		Scalar division
//-------------------------------------------------------------------------------
Point4 Point4::operator/( float scalar ) const
{
    return Point4( x/scalar, y/scalar, z/scalar );

}   // End:  operator/()


//-------------------------------------------------------------------------------
//	@	Point4::operator/=()
//-------------------------------------------------------------------------------
//		Scalar division by self
//-------------------------------------------------------------------------------
Point4& Point4::operator/=( float scalar )
{
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;

}   // End:  Point4::operator/=()


//-------------------------------------------------------------------------------
//	@	Point4::operator*=()
//--------------------------------------------------------------------------------
//		Transform Point4 with a VQS, save to self
//--------------------------------------------------------------------------------
Point4& Point4::operator*=( const VQS& vqs)
{
	//Scale
	x *= vqs.s;
	y *= vqs.s;
	z *= vqs.s;

	//Rotate
	vqs.q.RotateSelf(*this);

	//Translate
	x += vqs.v.x;
	y += vqs.v.y;
	z += vqs.v.z;

	return *this;

}	//End: Point4::operator*=()


//-------------------------------------------------------------------------------
//	@	Point4::operator*()
//--------------------------------------------------------------------------------
//		Transform Point4 with a VQS, return Point4
//--------------------------------------------------------------------------------
Point4 Point4::operator*( const VQS& vqs)
{
	//Temp and scale
	Point4 temp(x*vqs.s, y*vqs.s, z*vqs.s);

	//Rotate;
	vqs.q.RotateSelf(temp);

	//Translate
	temp.x += vqs.v.x;
	temp.y += vqs.v.y;
	temp.z += vqs.v.z;

	return temp;

}	//End: Point4::operator*()


//-------------------------------------------------------------------------------
//	@	Dot()
//-------------------------------------------------------------------------------
//		Dot product with vector
//-------------------------------------------------------------------------------
float Point4::Dot( const Vector4& v ) const
{
    return (x*v.x + y*v.y + z*v.z);

}   // End:  Dot()


//-------------------------------------------------------------------------------
//	@	DistPointPoint()
//--------------------------------------------------------------------------------
//		Distance between two points
//--------------------------------------------------------------------------------
float DistPointPoint(const Point4& p1, const Point4& p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;

	return ::DgSqrt(x*x + y*y + z*z);

}	//End: DistPointPoint()


//-------------------------------------------------------------------------------
//	@	SqDistPointPoint()
//--------------------------------------------------------------------------------
//		Distance between two points squared
//--------------------------------------------------------------------------------
float SqDistPointPoint(const Point4& p1, const Point4& p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;

	return (x*x + y*y + z*z);

}	//End: SqDistPointPoint()