//================================================================================
// @ Matrix44.h
// 
// Class: Matrix44
//
// 4 by 4 matrix calss.
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

#ifndef MATRIX44_H
#define MATRIX44_H

#include "Vector4.h"
#include "Point4.h"
#include "Dg_io.h"
#include "DgTypes.h"
#include "VQS.h"

class Matrix33;
class BasisR3;

//--------------------------------------------------------------------------------
//	@	Matrix44
//--------------------------------------------------------------------------------
class Matrix44
{
	friend class Quaternion;
public:
	//constructor/destructor
	inline Matrix44() {Identity();}
	Matrix44(const VQS&);
	Matrix44(const BasisR3&);
	inline ~Matrix44() {}

	//Output
	friend DgWriter& operator<<(DgWriter& out, const Matrix44& source);

	//Copy operations
	Matrix44(const Matrix44&);
	Matrix44& operator=(const Matrix44&);

	//Accessors
	float& operator()(uint32 i, uint32 j);
	float operator()(uint32 i, uint32 j) const;
	inline const float* GetFloatPtr() {return mV;}

	//Comparison
	bool operator== (const Matrix44&) const;
	bool operator!= (const Matrix44&) const;
	bool IsZero() const;
	bool IsIdentity() const;

	//Manipulators
	void SetRows(	const HPoint& row1, const HPoint& row2, 
					const HPoint& row3, const HPoint& row4);
	void GetRows(	HPoint& row1, HPoint& row2, 
					HPoint& row3, HPoint& row4);
	void SetColumns(const HPoint& col1, const HPoint& col2, 
					const HPoint& col3, const HPoint& col4);
	void GetColumns(HPoint& col1, HPoint& col2,
					HPoint& col3, HPoint& col4);
	void SetRow(unsigned short i, const HPoint& row);
	void SetColumn(unsigned short i, const HPoint& col);

	void Set(const BasisR3&);

	void Clean();
	void Identity();

	Matrix44& AffineInverse();
	friend Matrix44 AffineInverse(const Matrix44&);

	Matrix44& Transpose();
	friend Matrix44 Transpose(const Matrix44&);

	//transformations
	Matrix44& Translation(const HPoint&);
	Matrix44& Rotation(float zRotation, float yRotation, float xRotation);
	Matrix44& Rotation(const Vector4& axis, float angle);
	Matrix44& Rotation(const Quaternion&);

	Matrix44& Scaling(const HPoint& scale);
	Matrix44& Scaling(float val);

	Matrix44& RotationX(float);
	Matrix44& RotationY(float);
	Matrix44& RotationZ(float);

	void GetFixedAngles(float& zRotation, float& yRotation, float& xRotation);
	void GetAxisAngle(Vector4& axis, float& angle);

	//operators

	Matrix44 operator+ (const Matrix44&) const;
	Matrix44& operator+= (const Matrix44&);
	Matrix44 operator- (const Matrix44&) const;
	Matrix44& operator-= (const Matrix44&);

	Matrix44 operator-() const;

	//Multiplication
	Matrix44& operator*= (const Matrix44&);
	Matrix44 operator* (const Matrix44&) const;

	//Column vector multiplier
	Vector4 operator* (const Vector4&) const;
	Point4 operator* (const Point4&) const;

	//Row vector multiplier
	friend Vector4 operator* (const Vector4&, const Matrix44&);
	friend Point4 operator* (const Point4&, const Matrix44&);

	//Scalar multipication
	Matrix44& operator*= (float);
	friend Matrix44 operator* (float, const Matrix44&);
	Matrix44 operator* (float) const;

	//Low-level data accessors - implementation-dependant
	operator float*() {return mV;}
	operator const float*() const {return mV;}

protected:
	//Data Members
	float mV[16];

};


//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Matrix44::operator()
//--------------------------------------------------------------------------------
//		2D array accessor
//--------------------------------------------------------------------------------
inline float& Matrix44::operator()(uint32 i, uint32 j)
{
	return mV[i + 4*j];

}	//End: Matrix44::operator()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator()
//--------------------------------------------------------------------------------
//		2D array accessor
//--------------------------------------------------------------------------------
inline float Matrix44::operator()(uint32 i, uint32 j) const
{
	return mV[i + 4*j];

}	//End: Matrix44::operator()

#endif