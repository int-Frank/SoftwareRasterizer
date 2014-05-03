//================================================================================
// @ Point4.h
// 
// Class: Point4
//
// 3 dimensional homogeneous point class.
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

#ifndef POINT4_H
#define POINT4_H

#include "Dg_io.h"
#include "HPoint.h"

class Vector4;

//--------------------------------------------------------------------------------
//	@	Point4
//--------------------------------------------------------------------------------
class Point4 : public HPoint
{
	friend class Plane4;
	friend class Matrix44;
	friend class Quaternion;
	friend class VQS;
public:

    // constructor/destructor
    Point4(): HPoint(1.0f) {}	//Set w to 1
    Point4( float _x, float _y, float _z) :
        HPoint(_x, _y, _z, 1.0f) {}
    ~Point4() {}

	//Input
	friend DgReader& operator>>(DgReader& in, Point4& dest);

    // copy operations
    Point4(const Point4&);
    Point4& operator=(const Point4&);

    // Comparison.
    bool operator== (const Point4&) const;
    bool operator!= (const Point4&) const;

    // manipulators
    inline void Set( float _x, float _y, float _z);
    void Clean();       // sets near-zero elements to 0
    inline void Zero(); // sets all elements to 0

    // operators

    // Arithmetic operations
    Point4 operator+(const Vector4&) const;
    Point4 operator-(const Vector4&) const;
    Point4	operator*(float) const;
    friend Point4 operator*(float, const Point4&);
    Point4	operator/(float) const;
	Point4 operator- () const;
	
	Vector4 operator-(const Point4&) const;

    // Arithmetic updates
    Point4& operator+=(const Vector4&);
    Point4& operator-=(const Vector4&);
    Point4& operator*=(float);
    Point4& operator/=(float);
	Point4& operator*=(const VQS&);
	Point4 operator*(const VQS&);

    // dot product
    float Dot( const Vector4& ) const;
	friend float SqDistPointPoint(const Point4&, const Point4&);
	friend float DistPointPoint(const Point4&, const Point4&);
	
	//Friend functions
	friend Point4 operator* (const Point4&, const Matrix44&);
	
    // useful defaults
    static Point4    origin;

};

//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// Set vector elements
//-------------------------------------------------------------------------------
inline void Point4::Set( float _x, float _y, float _z)
{
    x = _x; y = _y; z = _z;
}   // End: Point4::Set()


//-------------------------------------------------------------------------------
// Zero all elements
//-------------------------------------------------------------------------------
inline void Point4::Zero()
{
    x = y = z = 0.0f;
}   // End: Point4::Zero()

#endif