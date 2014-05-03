//================================================================================
// @ Vector4.h
// 
// Class: Vector4
//
// 3 dimensional homogeneous vector class.
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

#ifndef VECTOR4_H
#define VECTOR4_H

#include "Dg_io.h"
#include "HPoint.h"

//--------------------------------------------------------------------------------
//	@	Vector4
//--------------------------------------------------------------------------------
class Vector4 : public HPoint
{
	friend class Point4;
	friend class Plane4;
	friend class Matrix44;
	friend class Quaternion;
	friend class VQS;
public:

    // constructor/destructor
    Vector4(): HPoint(0.0f) {} //Set w to 0.0f
    Vector4( float _x, float _y, float _z) :
        HPoint(_x, _y, _z, 0.0f) {}
    ~Vector4() {}

	//Input
	friend DgReader& operator>>(DgReader& in, Vector4& dest);

    // copy operations
    Vector4(const Vector4&);
    Vector4& operator=(const Vector4&);

    // Comparison.
    bool operator== (const Vector4&) const;
    bool operator!= (const Vector4&) const;
    bool IsZero() const;
    bool IsUnit() const;

    // manipulators
    inline void Set( float _x, float _y, float _z);
    void Clean();       // sets near-zero elements to 0
    inline void Zero(); // sets all elements to 0
    void Normalize();   // sets to unit vector

    // operators

    // Arithmetic operations
    Vector4 operator+(const Vector4&) const;
    Vector4 operator-(const Vector4&) const;
    Vector4	operator*(float) const;
    friend Vector4 operator*(float, const Vector4&);
    Vector4	operator/(float) const;
	Vector4 operator- () const;

    // Arithmetic updates
    Vector4& operator+=(const Vector4&);
    Vector4& operator-=(const Vector4&);
    Vector4& operator*=(float);
    Vector4& operator/=(float);
	Vector4& operator*=(const VQS&);
	Vector4 operator*(const VQS&);

    // dot product
    friend float Dot(const Vector4&, const Vector4&);
	friend Vector4 Cross(const Vector4&, const Vector4&);
	
	//other 
	float Length() const;
	float LengthSquared() const;
	Vector4 Perpendicular() const;

	//Friend functions
	friend Vector4 operator* (const Vector4&, const Matrix44&);

    // useful defaults
    static const Vector4    xAxis;
    static const Vector4    yAxis;
    static const Vector4    zAxis;
    static const Vector4    origin;
        
};

//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	@	Vector4::Set()
//-------------------------------------------------------------------------------
// Set vector elements
//-------------------------------------------------------------------------------
inline void Vector4::Set( float _x, float _y, float _z)
{
    x = _x; y = _y; z = _z;

}   // End: Vector4::Set()


//-------------------------------------------------------------------------------
//	@	Vector4::Zero()
//-------------------------------------------------------------------------------
// Zero all elements
//-------------------------------------------------------------------------------
inline void Vector4::Zero()
{
    x = y = z = 0.0f;

}   // End: Vector4::Zero()


//-------------------------------------------------------------------------------
//		Useful functions
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//		Returns a random normalised vector
//-------------------------------------------------------------------------------
Vector4 GetRandomVector();


//-------------------------------------------------------------------------------
//		Returns a random normalised vector, orthogonal to the input
//-------------------------------------------------------------------------------
Vector4 GetRandomOrthonormalVector(const Vector4& axis);


//-------------------------------------------------------------------------------
//		Returns a random normalised vector within a cone
//-------------------------------------------------------------------------------
Vector4 GetRandomConeVector(const Vector4& axis, float theta );

#endif