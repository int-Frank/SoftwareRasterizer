//================================================================================
// @ VQS.cpp
// 
// Description:
//
// Definitions for the VQS class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "VQS.h"
#include "BasisR3.h"
#include "CommonMath.h"
#include "Point4.h"
#include "Matrix44.h"
#include "pugixml.hpp"

//--------------------------------------------------------------------------------
//		Static members
//--------------------------------------------------------------------------------
VQS VQS::DEFAULT(Vector4::origin, Quaternion(1.0f, 0.0f, 0.0f, 0.0f), 1.0f);


//--------------------------------------------------------------------------------
//	@	VQS::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
VQS& VQS::operator=(const VQS& g)
{
	v = g.v;
	q = g.q;
	s = g.s;

	return *this;

}	//End: VQS::operator=();



//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Read from input
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, VQS& dest)
{
	//Read from input
    if ((in >> dest.v >> dest.q >> dest.s).fail())
    {
        std::cerr << "@operator>>(VQS)->Bad read." << std::endl;
    }
    else
    {
        //Ensure q is valid
        dest.q.MakeValid();
    }

	return in;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	Read_VQS()
//--------------------------------------------------------------------------------
//		Read vqs as vector, quaternion, scalar
//--------------------------------------------------------------------------------
void Read_VQS(pugi::xml_node& node, VQS& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "v")
		{
			DgString(it->child_value()) >> dest.v;
		}
		else if (tag == "q")
		{
			DgString(it->child_value()) >> dest.q;
		}
		else if (tag == "s")
		{
			float s;
			DgString(it->child_value()) >> s;
			if (s < 0.0f)
				dest.s = 1.0f;
			else
				dest.s = s;
		}
	}

	//Ensure q is valid
	dest.q.MakeValid();

}	//End: Read_VQS()


//--------------------------------------------------------------------------------
//	@	Read_LookAt()
//--------------------------------------------------------------------------------
//		Read vqs from an origin, a point to look at and an up vector
//--------------------------------------------------------------------------------
void Read_LookAt(pugi::xml_node& node, VQS& dest)
{
	Point4 origin(0.0f, 0.0f, 0.0f);
	Point4 target(1.0f, 0.0f, 0.0f);
	Vector4 up(0.0f, 0.0f, 1.0f);
	float scale = 1.0f;
	if (scale == 1.0f)
		char df = 8;


	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "origin")
		{
			DgString(it->child_value()) >> origin;
		}
		else if (tag == "target")
		{
			DgString(it->child_value()) >> target;
		}
		else if (tag == "up")
		{
			DgString(it->child_value()) >> up;
		}
		else if (tag == "s")
		{
			float temp;
			DgString(it->child_value()) >> temp;
			if (temp > 0.0f)
				scale = temp;
		}
	}

	Quaternion q;
	q.Set(origin, target, up);

	Vector4 v(origin - Point4::origin);

	dest.Set(v, q, scale);

}	//End: Read_LookAt()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Read from XML node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, VQS& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

        if (tag == "vqs")
		{
			Read_VQS(*it, dest);
		}
		else if (tag == "lookat")
		{
			Read_LookAt(*it, dest);
		}
    }
	
	return node;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Read from input
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const VQS& source)
{
	out << source.v << '\n' << source.q << '\n' << source.s;

	return out;
}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	VQS::operator*()
//--------------------------------------------------------------------------------
//		Transform a 3D Point
//--------------------------------------------------------------------------------
Point4 VQS::operator*(Point4 p) const
{
	//Scale
	p *= s;

	//Rotate;
	q.RotateSelf(p);

	//Translate
	p += v;

	return p;

}	//End: VQS::operator()


//--------------------------------------------------------------------------------
//	@	VQS::Clean
//--------------------------------------------------------------------------------
//		Ensure valid VQS, especially q
//--------------------------------------------------------------------------------
void VQS::MakeValid()
{
	//No need to clean vector

	//Clean quaternion
	q.MakeValid();

	//Clean scale;
	if (s < EPSILON)
		s = 0.0f;

}	//End: VQS::Clean()


//--------------------------------------------------------------------------------
//	@	VQS::operator*()
//--------------------------------------------------------------------------------
//		Transform a 3D Vector
//--------------------------------------------------------------------------------
Vector4 VQS::operator*(Vector4 p) const
{
	//Scale
	p *= s;

	//Rotate;
	q.RotateSelf(p);

	//Translate: DO NOT TRANSLATE VECTORS
	//p += v;

	return p;

}	//End: VQS::operator*()


//--------------------------------------------------------------------------------
//	@	VQS::Translate()
//--------------------------------------------------------------------------------
//		Translate a 3D Point
//--------------------------------------------------------------------------------
Point4 VQS::Translate(Point4 p) const
{
	//Translate
	p += v;

	return p;

}	//End: VQS::Translate()


//--------------------------------------------------------------------------------
//	@	VQS::Rotate()
//--------------------------------------------------------------------------------
//		Rotate a 3D Point
//--------------------------------------------------------------------------------
Point4 VQS::Rotate(Point4 p) const
{
	//Rotate;
	q.RotateSelf(p);

	return p;

}	//End: VQS::Rotate()


//--------------------------------------------------------------------------------
//	@	VQS::Scale()
//--------------------------------------------------------------------------------
//		Scale coordinates of a 3D Point
//--------------------------------------------------------------------------------
Point4 VQS::Scale(Point4 p) const
{
	//Scale
	p *= s;

	return p;

}	//End: VQS::Scale()


//--------------------------------------------------------------------------------
//	@	VQS::Translate()
//--------------------------------------------------------------------------------
//		Translate a 3D Vector
//--------------------------------------------------------------------------------
Vector4 VQS::Translate(Vector4 p) const
{
	//Translate
	p += v;

	return p;

}	//End: VQS::Translate()


//--------------------------------------------------------------------------------
//	@	VQS::Rotate()
//--------------------------------------------------------------------------------
//		Rotate a 3D Vector
//--------------------------------------------------------------------------------
Vector4 VQS::Rotate(Vector4 p) const
{
	//Rotate;
	q.RotateSelf(p);

	return p;

}	//End: VQS::Rotate()


//--------------------------------------------------------------------------------
//	@	VQS::Scale()
//--------------------------------------------------------------------------------
//		Scale coordinates of a 3D Vector
//--------------------------------------------------------------------------------
Vector4 VQS::Scale(Vector4 p) const
{
	//Scale
	p *= s;

	return p;

}	//End: VQS::Scale()


//--------------------------------------------------------------------------------
//	@	VQS::TranslateSelf()
//--------------------------------------------------------------------------------
//		Translate a HPoint, store in self
//--------------------------------------------------------------------------------
void VQS::TranslateSelf(HPoint& p) const
{
	//Translate
	p.x += v.x;
	p.y += v.y;
	p.z += v.z;

}	//End: VQS::TranslateSelf()


//--------------------------------------------------------------------------------
//	@	VQS::RotateSelf()
//--------------------------------------------------------------------------------
//		Rotate a Point4, store in self
//--------------------------------------------------------------------------------
void VQS::RotateSelf(Point4& p) const
{
	//Rotate;
	q.RotateSelf(p);

}	//End: VQS::RotateSelf()


//--------------------------------------------------------------------------------
//	@	VQS::RotateSelf()
//--------------------------------------------------------------------------------
//		Rotate a Vector4, store in self
//--------------------------------------------------------------------------------
void VQS::RotateSelf(Vector4& v) const
{
	//Rotate;
	q.RotateSelf(v);

}	//End: VQS::RotateSelf()


//--------------------------------------------------------------------------------
//	@	VQS::ScaleSelf()
//--------------------------------------------------------------------------------
//		Scale coordinates of a HPoint (first 3 elements), store in self
//--------------------------------------------------------------------------------
void VQS::ScaleSelf(HPoint& p) const
{
	//Scale
	p.x *= s;
	p.y *= s;
	p.z *= s;

}	//End: VQS::ScaleSelf()


//--------------------------------------------------------------------------------
//	@	VQS::Set()
//--------------------------------------------------------------------------------
//		Set all data
//--------------------------------------------------------------------------------
void VQS::Set(const Vector4& _v, const Quaternion& _q, float _s)
{
	v = _v;
	q = _q;
	
	//Ensure q is valid
	q.MakeValid();

	s = _s;

}	//End: VQS::Set()


//--------------------------------------------------------------------------------
//	@	VQS::operator*()
//--------------------------------------------------------------------------------
//		Concatenation, return VQS
//--------------------------------------------------------------------------------
VQS VQS::operator*(const VQS& rhs) const
{
	VQS result;

	//Combine translation vectors
	result.v = q.Rotate(rhs.v)*s + v;

	//Combine quaternions
	result.q = q * rhs.q;

	//Combine scales
	result.s = s * rhs.s;

	//Return result
	return result;

}	//End: VQS::operator*()


//--------------------------------------------------------------------------------
//	@	VQS::Operator*=()
//--------------------------------------------------------------------------------
//		Concatenation, store in self
//--------------------------------------------------------------------------------
VQS& VQS::operator*=(const VQS& rhs)
{
	//Combine translation vectors
	v = q.Rotate(rhs.v)*s + v;

	//Combine quaternions
	q = q * rhs.q;

	//Combine scales
	s = s * rhs.s;

	return *this;

}	//End: VQS::operator*=()


//--------------------------------------------------------------------------------
//	@	VQS::Inverse()
//--------------------------------------------------------------------------------
//		Sets self to inverse
//--------------------------------------------------------------------------------
const VQS& VQS::Inverse()
{
	//The method to find a VQS inverse
	//[1/s*(q-1*(-v)*q), q-1, 1/s]

	//Inverse scale
	s = 1.0f/s;

	//Inverse quaternion
	q.Inverse();

	//Inverse vector
	v = q.Rotate(-v) * s;

	return *this;

}	//End: Inverse()


//--------------------------------------------------------------------------------
//	@	Inverse()
//--------------------------------------------------------------------------------
//		Returns inverse VQS
//--------------------------------------------------------------------------------
VQS Inverse(const VQS& other)
{
	VQS temp;

	//Inverse scale
	temp.s = 1.0f/other.s;

	//Inverse quaternion
	temp.q = Inverse(other.q);

	//Inverse vector
	temp.v = temp.q.Rotate(-other.v) * temp.s;

	return temp;

}	//End: Inverse()