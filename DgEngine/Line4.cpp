//================================================================================
// @ Line4.cpp
// 
// Description:
//
// This file defines Line4's methods and Testing functions involving Lines.
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

#include "Line4.h"
#include "CommonMath.h"

//--------------------------------------------------------------------------------
//	@	Line4::Line4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Line4::Line4(): 
origin(0.0f, 0.0f, 0.0f),
direction(1.0f, 0.0f, 0.0f) 
{
}	//End: Line4::Line4


//--------------------------------------------------------------------------------
//	@	Line4::Line4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Line4::Line4(const Point4& O, const Vector4& D):
origin(O),
direction(D)
{
	//Normalise direction
	direction.Normalize();

}	//End: Line4::Line4()


//--------------------------------------------------------------------------------
//	@	Line4::Line4()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Line4::Line4(const Line4& other):
origin(other.origin),
	direction(other.direction)
{
}	//End: Line4::Line4()


//--------------------------------------------------------------------------------
//	@	Line4::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Line4& Line4::operator= (const Line4& other)
{
	origin = other.origin;
	direction = other.direction;

	return *this;

}	//End: Line4::operator=()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Line4& source)
{
	out << "[" << source.Origin() << ", " 
		<< source.Direction() << "]";

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	Line4::Get()
//--------------------------------------------------------------------------------
//		Accessor
//--------------------------------------------------------------------------------
void Line4::Get(Point4& O, Vector4& D) const
{
	O = origin;
	D = direction;

}	//End: Line4::Get()


//--------------------------------------------------------------------------------
//	@	Line4::operator==()
//--------------------------------------------------------------------------------
//		Are two Line4's equal?
//--------------------------------------------------------------------------------
bool Line4::operator== (const Line4& L) const
{
	return (L.origin == origin && L.direction == direction);

}	//End: Line4::operator==()


//--------------------------------------------------------------------------------
//	@	Line4::operator!=()
//--------------------------------------------------------------------------------
//		Are two Line4's not equal?
//--------------------------------------------------------------------------------
bool Line4::operator!= (const Line4& L) const
{
	return !(L.origin == origin && L.direction == direction);
}	//End: Line4::operator!=()


//--------------------------------------------------------------------------------
//	@	Line4::ClosestPoint()
//--------------------------------------------------------------------------------
//		Returns closest point on a Line4 to a point.
//--------------------------------------------------------------------------------
Point4 Line4::ClosestPoint (const Point4& point) const
{
	Vector4 w = point - origin;

	float vsq = Dot(direction,direction);
	float proj = Dot(w,direction);

	return origin + (proj/vsq)*direction;

}	//End: Line4::ClosestPoint()


//--------------------------------------------------------------------------------
//	@	SqDistLineLine()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between two lines
			--------------------------------------
		Post:	
		Case - Lines not parallel: returns 0. sqDist set, u1 and u2 are set.
		Case - Lines parallel: returns 1. sqDist set, u1 and u2 are set with u1 = 0.
			--------------------------------------
		Param<L1>:	 input line
		Param<L2>:	 input line
		Param<sqDist>: squared distance between the lines
		Param<u1>:	 distance along L1 to the intersection or 
		             closest point to L2
		Param<u2>:	 distance along L2 to the intersection or 
		             closest point to L1
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineLine(const Line4& L1, const Line4& L2, 
					 float& sqDist, float& u1, float& u2)
{
	Point4 o1(L1.Origin());
	Point4 o2(L2.Origin());
	Vector4 d1(L1.Direction());
	Vector4 d2(L2.Direction());

	//compute intermediate parameters
	Vector4 w0(o1 - o2);
	float a = Dot(d1, d1);
	float b = Dot(d1, d2);
	float c = Dot(d2, d2);
	float d = Dot(d1, w0);
	float e = Dot(d2, w0);
	float denom = a*c - b*b;
	if (::IsZero(denom))
	{
		u1 = 0.0f;
		u2 = e/c;
		Vector4 wc(w0 - u2*d1);
		sqDist = wc.LengthSquared();
		return 1;
	}
	else
	{
		u1 = ((b*e - c*d)/denom);
		u2 = ((a*e - b*d)/denom);
		Vector4 wc(w0 + u1*d1 - u2*d2);
		sqDist = wc.LengthSquared();
		return 0;
	}
}	//End: Line4::SqDistLineLine


//--------------------------------------------------------------------------------
//	@	SqDistLinePoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a point and a line
			--------------------------------------
		Post: returns 0. sqDist set, u set.
			--------------------------------------
		Param<L>:	 input line
		Param<P>:	 input point
		Param<sqDist>: squared distance from the point to the line
		Param<u>:	 distance along L to the closest point to P
*/
//--------------------------------------------------------------------------------
uint8 SqDistLinePoint(const Line4& L, const Point4& P, float& sqDist, float& u)
{
	Vector4 dir(L.Direction());
	Vector4 w(P - L.Origin());

	float vsq = Dot(dir, dir);
	float proj = Dot(w, dir);
	u = proj/vsq;

	sqDist = w.LengthSquared() - u*proj;

	return 0;

}	//End: SqDistLinePoint()


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineLine()
//--------------------------------------------------------------------------------
/*		Summary: Finds closest points between two Lines
			--------------------------------------
		Post:	
		Case Lines not parallel: returns 0. P1, P2 set.
		Case Lines parallel: returns 1. P1, P2 set with P1 = L1.origin.
			--------------------------------------
		Param<L1>: input line
		Param<L2>: input line
		Param<P1>: the point on L1 that is closest to L2
		Param<P2>: the point on L2 that is closest to L1
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineLine(const Line4& L1, const Line4& L2, 
							Point4& P1, Point4& P2)
{
	Point4 o1(L1.Origin());
	Point4 o2(L2.Origin());
	Vector4 d1(L1.Direction());
	Vector4 d2(L2.Direction());

	//compute intermediate parameters
	Vector4 w0(o1 - o2);
	float a = Dot(d1, d1);
	float b = Dot(d1, d2);
	float c = Dot(d2, d2);
	float d = Dot(d1, w0);
	float e = Dot(d2, w0);
	float denom = a*c - b*b;

	if (::IsZero(denom))
	{
		P1 = o1;
		P2 = o2 + (e/c)*d1;
		return 1;
	}
	else
	{
		P1 = o1 + ((b*e - c*d)/denom)*d1;
		P2 = o2 + ((a*e - b*d)/denom)*d2;
		return 0;
	}

}	//End: ClosestPointsLineLine()