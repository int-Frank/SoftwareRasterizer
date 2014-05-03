//================================================================================
// @ HPoint.h
// 
// Class: HPoint
//
// HPoint represents a homogeneous point of the form (x,y,z,w).  Affine points 
// are characterized by w = 1 (see class Point4) and affine vectors are 
// characterized by w = 0 (see class Vector4).
//
// -------------------------------------------------------------------------------
//
// Original Author: David H. Eberly
// Retrieved From: http://www.geometrictools.com/LibMathematics/Algebra/Algebra.html
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef HPOINT_H
#define HPOINT_H

#include "Dg_io.h"
#include "Vector3.h"

//--------------------------------------------------------------------------------
//	@	HPoint
//--------------------------------------------------------------------------------
class HPoint
{
	friend class Point4;
	friend class Vector4;
	friend class Matrix44;
	friend class VQS;
	friend class Quaternion;
public:
    // Construction and destruction.
	HPoint() {}
	HPoint(float _w): w(_w) {}
    HPoint (float _x, float _y, float _z, float _w):
		x(_x), y(_y), z(_z), w(_w) {}
	~HPoint () {}

	// accessors
    inline float& operator[]( unsigned int i )      { return (&x)[i]; }
    inline float operator[]( unsigned int i ) const { return (&x)[i]; }

	//Output
	friend DgWriter& operator<<(DgWriter& out, const HPoint& source);

	//Input
	friend DgReader& operator>>(DgReader& in, HPoint& dest);

	//Conversion
	operator Vector3() {return Vector3(x, y, z);}

    // Coordinate access.
	inline float X() const { return x; }
	inline float Y() const { return y; }
	inline float Z() const { return z; }
	inline float W() const { return w; }
	inline float& X()	   { return x; }
	inline float& Y()	   { return y; }
	inline float& Z()	   { return z; }
	inline float& W()	   { return w; }

    // Copy operations
	HPoint (const HPoint& p): x(p.x), y(p.y), z(p.z), w(p.w) {}
    HPoint& operator= (const HPoint& pnt);

    // Comparison.
    virtual bool operator== (const HPoint& pnt) const;
    virtual bool operator!= (const HPoint& pnt) const;

protected:
    float x, y, z, w;
};


#endif
