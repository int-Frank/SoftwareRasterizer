//================================================================================
// @ BasisR3.h
// 
// Class: Basis3D
//
// A set of 3 orthonormal vectors. A basis is built from  two vectors x0 and x1, 
// the third is calculated by finding the cross product x0 cross x1.
//
//                    x2   x0
//                     |   /
//                     |  /
//                     | /
//         x1__________|/
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef BASISR3_H
#define BASISR3_H

#include "Vector4.h"
#include "Dg_io.h"

class VQS;

//--------------------------------------------------------------------------------
//	@	BasisR3
//--------------------------------------------------------------------------------
class BasisR3
{
	friend class Quaternion;
	friend class Matrix44;
public:

	//Constructor / destructor
	BasisR3();
	BasisR3(const Vector4& _x0, 
			const Vector4& _x1);

	//Copy operations
	BasisR3(const BasisR3&);
	BasisR3& operator=(const BasisR3&);

	friend DgReader& operator>>(DgReader&, BasisR3&);

	//Set axis
	void Set(const Vector4& _x0, 
			 const Vector4& _x1);

	//Manipulators
	BasisR3 Transform(const VQS&) const;
	BasisR3 TransformQuick(const VQS&) const;

	//Updates
	void TransformSelf(const VQS&);
	void TransformSelfQuick(const VQS&);

	//Getters
	const Vector4& x0() const {return m_x0;}
	const Vector4& x1() const {return m_x1;}
	const Vector4& x2() const {return m_x2;}
	const Vector4& operator[](uint8 i) const {return (&m_x0)[i];}

	//Useful constants
	static const BasisR3 xyz;

private:

	//Quick construction from vectors, assumes all vectors are orthonormal!
	BasisR3(const Vector4& i0, const Vector4& i1, const Vector4& i2):
		m_x0(i0), m_x1(i1), m_x2(i2) {}

private:
	//Data members
	Vector4 m_x0, m_x1, m_x2;
};

#endif