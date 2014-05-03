//================================================================================
// @ VQS.h
// 
// Class: VQS
//
// Vector, Quaternion, Scalar. This is an alternate to matrices to represent
// transformations in 3D space. In this class we have a translation component
// (Vector), orientation (Quaternion) and scaling component (Scalar). The main
// advantage over a 4x4 matrix is using a quaternion for orientation, arguably
// a more complete and robust method for orientation representation. 
//
// Some disatvantages include: we are restricted to the types of transformations
// we can do. We cannot shear or reflect for example. Also We must use uniform
// scaling.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef VQS_H
#define VQS_H

#include "Point4.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Dg_io.h"
#include "CommonMath.h"

namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//	@	VQS
//--------------------------------------------------------------------------------
class VQS
{
	friend class Vector4;
	friend class Point4;
	friend class Matrix44;
public:
	//Constructor/Destructor
	VQS(): v(0.0f, 0.0f, 0.0f), q(1.0f, 0.0f, 0.0f, 0.0f), s(1.0f) {}
	VQS(const Vector4& _v, const Quaternion& _q, float _s):
		v(_v), q(_q), s(_s) {}
	~VQS() {}

	//Copy operations
	VQS(const VQS& g): v(g.v), q(g.q), s(g.s) {}
	VQS& operator=(const VQS&);
	
	//Input
	friend DgReader& operator>>(DgReader& in, VQS& dest);
	friend pugi::xml_node& operator>>(pugi::xml_node&, VQS& dest);
	friend void Read_VQS(pugi::xml_node&, VQS&);
	friend void Read_LookAt(pugi::xml_node&, VQS&);
	friend DgWriter& operator<<(DgWriter& out, const VQS& source);
	
	//Set data
	void Set(const Vector4& _v, const Quaternion& _q, float _s);
	inline void SetV(const Vector4&);
	inline void SetQ(const Quaternion&);
	inline void SetS(float);

	//Ensure valid VQS.
	void MakeValid();

	//Update data
	inline void UpdateV(const Vector4&);
	inline void UpdateQ(const Quaternion&);
	inline void UpdateS(float);

	//Concatenation
	VQS operator* (const VQS&) const;
	VQS& operator*= (const VQS&);
	
	//Operators
	Point4 operator*(Point4) const;
	Vector4 operator*(Vector4) const;

	//Transformations
	Point4 Translate(Point4) const;
	Point4 Rotate(Point4) const;
	Point4 Scale(Point4) const;
	
	Vector4 Translate(Vector4) const;
	Vector4 Rotate(Vector4) const;
	Vector4 Scale(Vector4) const;

	void TranslateSelf(HPoint&) const;
	void RotateSelf(Point4&) const;
	void RotateSelf(Vector4&) const;
	void ScaleSelf(HPoint&) const;

	//Inverse 
	const VQS& Inverse();
	friend VQS Inverse(const VQS& other);

	//Returns
	inline void Get(Vector4& _v, Quaternion& _q, float& _s) const {_v = v; _q = q; _s = s;}
	const Vector4& V()	 const	{return v;}
	const Quaternion& Q() const	{return q;}
	float S()	 const	{return s;}

	static VQS DEFAULT;

private:
	//Data members
	
	Vector4		v;		//translation
	Quaternion	q;		//rotation
	float		s;		//scale
};

//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	VQS::Set()
//--------------------------------------------------------------------------------
//		Set translation vector
//--------------------------------------------------------------------------------
inline void VQS::SetV(const Vector4& _v)
{
	v = _v;

}	//End: VQS::Set()


//--------------------------------------------------------------------------------
//	@	VQS::SetQ()
//--------------------------------------------------------------------------------
//		Set quaternion
//--------------------------------------------------------------------------------
inline void VQS::SetQ(const Quaternion& _q)
{
	q = _q;
	q.MakeValid();

}	//End: VQS::Set()


//--------------------------------------------------------------------------------
//	@	VQS::SetS()
//--------------------------------------------------------------------------------
//		Set scale
//--------------------------------------------------------------------------------
inline void VQS::SetS(float _s)
{
	s = _s;

}	//End: VQS::Set()


//--------------------------------------------------------------------------------
//	@	VQS::UpdateV()
//--------------------------------------------------------------------------------
//		Update translation vector
//--------------------------------------------------------------------------------
inline void VQS::UpdateV(const Vector4& _v)
{
	v += _v;

}	//End: VQS::UpdateV()


//--------------------------------------------------------------------------------
//	@	VQS::UpdateQ()
//--------------------------------------------------------------------------------
//		Update quaternion
//--------------------------------------------------------------------------------
inline void VQS::UpdateQ(const Quaternion& _q)
{
	q *= _q;
	q.MakeValid();

}	//End: VQS::UpdateQ()


//--------------------------------------------------------------------------------
//	@	VQS::UpdateS()
//--------------------------------------------------------------------------------
//		Update scale
//--------------------------------------------------------------------------------
inline void VQS::UpdateS(float _s)
{
	s *= _s;

}	//End: VQS::UpdateS()

#endif