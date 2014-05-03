//================================================================================
// @ Vector2.h
// 
// Class: Vector2
//
// 2 dimensional geometric vector class.
//
// -------------------------------------------------------------------------------
//
// Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef VECTOR2_H
#define VECTOR2_H

#include "Dg_io.h"

//--------------------------------------------------------------------------------
//	@	Vector2
//--------------------------------------------------------------------------------
class Vector2
{
public:
	//constructor/destructor
	Vector2() {}
	Vector2(float xval, float yval): x(xval), y(yval) {}
	~Vector2() {}

	//Output
	friend DgWriter& operator<<(DgWriter& out, const Vector2& source);

	//Input
	friend DgReader& operator>> (DgReader& in, Vector2& dest);

	//copy operations
	Vector2(const Vector2& v);
	Vector2& operator= (const Vector2& v);

	//comparison
	bool operator== (const Vector2&) const;
	bool operator!= (const Vector2&) const;
	bool IsZero() const;
	bool IsUnit() const;

	//manipulators
	void Set(float, float);
	void Clean();		//sets near-zero elements to zero
	void Zero();		//sets all elements to zero
	void Normalize();	//normalises vector

	//vector operations
	Vector2 operator+ (const Vector2&) const;
	friend Vector2& operator+= (Vector2& self, const Vector2& other);
	Vector2 operator- (const Vector2&) const;
	friend Vector2& operator-= (Vector2& lhs, const Vector2& rhs);
	Vector2 operator- () const;

	//scalar operations
	Vector2 operator* (float);
	friend Vector2 operator* (float, const Vector2&);
	Vector2& operator*= (float);
	Vector2 operator/ (float);
	Vector2& operator/= (float);

	//friend functions
	friend float Dot(const Vector2&, const Vector2&);
	friend float PerpDot(const Vector2&, const Vector2&);
	inline Vector2 Perpendicular() const {return Vector2(-y,x);}
	friend float DistVector2Vector2(const Vector2&, const Vector2&);
	friend float SqDistVector2Vector2(const Vector2&, const Vector2&);

	//other 
	float Length() const;
	float LengthSquared() const;

	//useful defaults
	static Vector2		xAxis;
	static Vector2		yAxis;
	static Vector2		origin;

	//member variables
	float x, y;
};

#endif