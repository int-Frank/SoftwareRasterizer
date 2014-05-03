//================================================================================
// @ Vector3.h
// 
// Class: Vector3
//
// 3 dimensional geometric vector class.
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

#ifndef VECTOR3_H
#define VECTOR3_H

#include "Dg_io.h"

//--------------------------------------------------------------------------------
//	@	Vector3
//--------------------------------------------------------------------------------
class Vector3
{
public:
	//constructor/destructor
	Vector3() {}
	Vector3(float xval, float yval, float zval): 
		x(xval), y(yval), z(zval) {}
	~Vector3() {}

	//Output
	friend DgWriter& operator<<(DgWriter& out, const Vector3& source);

	//Input
	friend DgReader& operator>> (DgReader& in, Vector3& dest);

	//copy operations
	Vector3(const Vector3& v): 
		x(v.x), y(v.y), z(v.z) {}
	Vector3& operator= (const Vector3& v);

	// accessors
    inline float& operator[]( unsigned int i )      { return (&x)[i]; }
    inline float operator[]( unsigned int i ) const { return (&x)[i]; }

	//comparison
	bool operator== (const Vector3&) const;
	bool operator!= (const Vector3&) const;
	bool IsZero() const;
	bool IsUnit() const;

	//manipulators
	inline void Set(float, float, float);
	void Clean();		//sets near-zero elements to zero
	inline void Zero();			//sets all elements to zero
	void Normalize();	//normalises vector

	//vector operations
	Vector3 operator+ (const Vector3&) const;
	friend Vector3& operator+= (Vector3& self, const Vector3& other);
	Vector3 operator- (const Vector3&) const;
	friend Vector3& operator-= (Vector3& lhs, const Vector3& rhs);
	Vector3 operator- () const;

	//scalar operations
	Vector3 operator* (float);
	friend Vector3 operator* (float, const Vector3&);
	Vector3& operator*= (float);
	Vector3 operator/ (float);
	Vector3& operator/= (float);

	//friend functions
	friend float Dot(const Vector3&, const Vector3&);
	friend Vector3 Cross(const Vector3&, const Vector3&);
	friend float DistVector3Vector3(const Vector3&, const Vector3&);
	friend float SqDistVector3Vector3(const Vector3&, const Vector3&);

	//other 
	float Length() const;
	float LengthSquared() const;

	//useful defaults
	static Vector3		xAxis;
	static Vector3		yAxis;
	static Vector3		zAxis;
	static Vector3		origin;
	static Vector3		unity;

	//Data Members
	float x, y, z;
	
};

//--------------------------------------------------------------------------------
//		INLINES
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Vector3::Set()
//--------------------------------------------------------------------------------
//		Set x, y, z
//--------------------------------------------------------------------------------
inline void Vector3::Set(float xval, float yval, float zval)
{
	x = xval; y = yval; z = zval;

}	//End: Vector3::Set()


//--------------------------------------------------------------------------------
//	@	Vector3::Zero()
//--------------------------------------------------------------------------------
//		Zero all elements
//--------------------------------------------------------------------------------
inline void Vector3::Zero()
{
	x = y = z = 0.0f;

}	//End: Vector3::Zero()


#endif