//================================================================================
// @ Ray4.cpp
// 
// Description:
//
// This file defines Ray4's methods and Testing functions involving Rays and Lines.
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

#include "Ray4.h"
#include "Line4.h"
#include "CommonMath.h"
#include "Vector4.h"
#include "VQS.h"

//--------------------------------------------------------------------------------
//	@	Ray4::Ray4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Ray4::Ray4(): 
origin(0.0f, 0.0f, 0.0f),
direction(1.0f, 0.0f, 0.0f) 
{
}	//End: Ray4::Ray4


//--------------------------------------------------------------------------------
//	@	Ray4::Ray4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Ray4::Ray4(const Point4& O, const Vector4& D):
origin(O),
direction(D)
{
	//Normalise
	direction.Normalize();

}	//End: Ray4::Ray4()


//--------------------------------------------------------------------------------
//	@	Ray4::Ray4()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Ray4::Ray4(const Ray4& other):
origin(other.origin), direction(other.direction)
{
}	//End: Ray4::Ray4()


//--------------------------------------------------------------------------------
//	@	Ray4::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Ray4& Ray4::operator= (const Ray4& other)
{
	origin = other.origin;
	direction = other.direction;

	return *this;

}	//End: Ray4::operator=()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Ray4& source)
{
	out << "[" << source.Origin() << ", " 
		<< source.Direction() << "]";

	return out;
    
}   // End: operator<<()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Input 
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, Ray4& dest)
{
	//Read to these
	Point4 p;
	Vector4 v;

	//Read in values
	in >> p >> v;

	//Set origin and direction
	dest.Set(p, v);

	return in;
    
}   // End of operator>>()


//--------------------------------------------------------------------------------
//	@	Ray4::Get()
//--------------------------------------------------------------------------------
//		Accessor
//--------------------------------------------------------------------------------
void Ray4::Get(Point4& O, Vector4& D) const
{
	O = origin;
	D = direction;
}	//End: Ray4::Get()


//--------------------------------------------------------------------------------
//	@	Ray4::operator==()
//--------------------------------------------------------------------------------
//		Are two Ray4's equal?
//--------------------------------------------------------------------------------
bool Ray4::operator== (const Ray4& R) const
{
	return (R.origin == origin && R.direction == direction);

}	//End: Ray4::operator==()


//--------------------------------------------------------------------------------
//	@	Ray4::operator!=()
//--------------------------------------------------------------------------------
//		Are two Ray4's not equal?
//--------------------------------------------------------------------------------
bool Ray4::operator!= (const Ray4& R) const
{
	return !(R.origin == origin && R.direction == direction);

}	//End: Ray4::operator!=()


//--------------------------------------------------------------------------------
//	@	Ray4::Transform()
//--------------------------------------------------------------------------------
//		Transform a Ray
//--------------------------------------------------------------------------------
void Ray4::Transform( const VQS& vqs)
{
	//Transform origin
	origin *= vqs;

	//Transform direction
	direction *= vqs;
	direction.Normalize();

}	//End: VQS::Transform()


//--------------------------------------------------------------------------------
//	@	Ray4::TransformQuick()
//--------------------------------------------------------------------------------
//		Transform a Ray, no directional normalization
//--------------------------------------------------------------------------------
void Ray4::TransformQuick( const VQS& vqs)
{
	//Transform origin
	origin *= vqs;

	//Transform direction
	direction *= vqs;

}	//End: VQS::TransformQuick()


//--------------------------------------------------------------------------------
//	@	Ray4::ClosestPoint()
//--------------------------------------------------------------------------------
//		Returns closest point on a Ray4 to a point
//--------------------------------------------------------------------------------
Point4 Ray4::ClosestPoint(const Point4& P) const
{
	Vector4 w(P - origin);

	float proj = Dot(w,direction);

	//endpoint 0 is closest point
	if (proj <= 0.0f)
	{
		return origin;
	}
	else
	{
		float vsq = Dot(direction, direction);
		return origin + (proj/vsq)*direction;
	}

}	//End: Ray4::ClosestPoint()


//--------------------------------------------------------------------------------
//	@	SqDistRayRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between two Rays
			--------------------------------------
		Post:	returns 1. sqDist set, u1 and u2 are set.
			--------------------------------------
		Param<R1>:	 input ray
		Param<R2>:	 input ray
		Param<sqDist>: squared distance between the lines
		Param<u1>:	 distance along R1 to the intersection or 
		             closest point to R2
		Param<u2>:	 distance along R2 to the intersection or 
		             closest point to R1
*/
//--------------------------------------------------------------------------------
uint8 SqDistRayRay(const Ray4& R1, const Ray4& R2, 
				   float& sqDist, float& u1, float& u2)
{
	Vector4 d1(R1.Direction());
	Vector4 d2(R2.Direction());

	//compute intermediate parameters
	Vector4 w0(R1.Origin() - R2.Origin());
	float a = Dot(d1, d1);
	float b = Dot(d1, d2);
	float c = Dot(d2, d2);
	float d = Dot(d1, w0);
	float e = Dot(d2, w0);
	float denom = a*c - b*b;

	//parameters to compute u1 and u2
	float sn, sd, tn, td;

	//if denom is zero, try finding closest point on R2 to origin1
	if (::IsZero(denom))
	{
		//clamp u1 to 0
		sd = td = c;
		sn = 0.0f;
		tn = e;
	}
	else
	{
		//clamp u1 within[0, +inf]
		sd = td = denom;
		sn = b*e - c*d;
		tn = a*e - b*d;

		//clamp u1 to 0
		if (sn < 0.0f)
		{
			sn = 0.0f;
			tn = e;
			td = c;
		}
	}

	//clamp u2 within [0, +inf]
	//clamp u2 to 0
	if (tn < 0.0f)
	{
		u2 = 0.0f;
		//clamp u1 to 0
		if (-d < 0.0f)
		{
			u1 = 0.0f;
		}
		else
		{
			u1 = -d/a;
		}
	}
	else
	{
		u2 = tn/td;
		u1 = sn/sd;
	}

	//compute difference vector and distance squared
	Vector4 wc(w0 + u1*d1 - u2*d2);
	sqDist =  wc.LengthSquared();

	return 1;
	
}	//End: SqDistRayRay()


//--------------------------------------------------------------------------------
//	@	SqDistLineRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a Line4 and a Ray4. Based 
                 on article and code by Dan Sunday at www.geometryalgorithms.com
			--------------------------------------
		Post:	Case demon == 0: returns 2. sqDist set, ur and ul are set.
				Case else: returns 1. sqDist set, ur and ul are set.
			--------------------------------------
		Param<L>:	 input ray
		Param<R>:	 input ray
		Param<sqDist>: squared distance between the lines
		Param<ul>:	 distance along L to the intersection or 
		             closest point to R
		Param<ur>:	 distance along R to the intersection or 
		             closest point to L
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineRay( const Line4& R, const Ray4& L, 
                   float& sqDist, float& ul, float& ur )
{
    Vector4 dr(R.Direction());
	Vector4 dl(L.Direction());

	//compute intermediate parameters
	Vector4 w0(R.Origin() - L.Origin());
	float a = Dot(dr, dr);
	float b = Dot(dr, dl);
	float c = Dot(dl, dl);
	float d = Dot(dr, w0);
	float e = Dot(dl, w0);

    float denom = a*c - b*b;

    // if denom is zero, try finding closest point on R2 to origin1
    if ( ::IsZero(denom) )
    {
        ur = 0.0f;
        ul = e/c;
        // compute difference vector and distance squared
        Vector4 wc(w0 - ul*dl);

        sqDist =  wc.LengthSquared();
		return 2;
    }
    else
    {
        // parameters to compute ur, ul
        float sn;

        // clamp ur within [0,1]
        sn = b*e - c*d;
  
        // clamp ur to 0
        if (sn < 0.0f)
        {
            ur = 0.0f;
            ul = e/c;
        }
        // clamp ur to 1
        else if (sn > denom)
        {
            ur = 1.0f;
            ul = (e+b)/c;
        }
        else
        {
            ur = sn/denom;
            ul = (a*e - b*d)/denom;
        }

        // compute difference vector and distance squared
        Vector4 wc(w0 + ur*dr - ul*dl);
        sqDist =  wc.LengthSquared();

		return 1;
    }

}   // End: ::SqDistLineRay()


//--------------------------------------------------------------------------------
//	@	SqDistPointRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a point and a ray
			--------------------------------------
		Post:	Case origin is closest point: returns 2. sqDist set, u set.
				Case else: returns 1. sqDist set, u set.
			--------------------------------------
		Param<R>:	 input ray
		Param<P>:	 input point
		Param<sqDist>: squared distance from the point to the ray
		Param<u>:	 distance along R to the closest point to P
*/
//--------------------------------------------------------------------------------
uint8 SqDistPointRay(const Point4& P, const Ray4& R, float& sqDist, float& u)
{
	Vector4 dr(R.Direction());
	Vector4 w(P - R.Origin());

	float proj = Dot(w,dr);

	//origin is closest point
	if (proj <= 0.0f)
	{
		u = 0.0f;
		sqDist = w.LengthSquared();
		return 2;
	}
	//else use normal line check
	else
	{
		float vsq = Dot(dr,dr);
		u = proj/vsq;
		sqDist =  w.LengthSquared() - u*proj;
		return 1;
	}

}	//End: SqDistPointRay()


//--------------------------------------------------------------------------------
//	@	ClosestPointsRayRay()
//--------------------------------------------------------------------------------
/*		Summary: Finds closest points between two Rays
			--------------------------------------
		Post:	returns 1. P1, P2 set.
			--------------------------------------
		Param<R1>: input ray
		Param<R2>: input ray
		Param<P1>: the point on R1 that is closest to R2
		Param<P2>: the point on R2 that is closest to R1
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsRayRay(const Ray4& R1, const Ray4& R2, Point4& P1, Point4& P2)
{
	Point4 o1(R1.Origin());
	Point4 o2(R2.Origin());
	Vector4 d1(R1.Direction());
	Vector4 d2(R2.Direction());

	//compute intermediate parameters
	Vector4 w0(o1 - o2);
	float a = Dot(d1, d1);
	float b = Dot(d1, d2);
	float c = Dot(d2, d2);
	float d = Dot(d1, w0);
	float e = Dot(d2, w0);

    float denom = a*c - b*b;
	//parameters to compute u1, u2
	float u1, u2;
	float sn, sd, tn, td;

	//if denom is zero, try finding closest point on R2 to origin1
	if (::IsZero(denom))
	{
		sd = td = c;
		sn = 0.0f;
		tn = e;
	}
	else
	{
		//start by clamping u1;
		sd = td = denom;
		sn = b*e - c*d;
		tn = a*e - b*d;

		//clamp u1 to 0
		if (sn < 0.0f)
		{
			sn = 0.0f;
			tn = e;
			td = c;
		}
	}

	//clamp u2 within [0,+inf]
	//clamp u2 to zero
	if (tn < 0.0f)
	{
		u2 = 0.0f;
		//clamp u1 to 0
		if (-d < 0.0f)
		{
			u1 = 0.0f;
		}
		else
		{
			u2 = -d/a;
		}
	}
	else
	{
		u2 = tn/td;
		u1 = sn/sd;
	}

	//compute closest points
	P1 = o1 + u1*d1;
	P2 = o2 + u2*d2;

	return 1;
}	//End: ::ClosestPointsRayRay()


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineRay()
//--------------------------------------------------------------------------------
/*		Summary: Finds closest points between a Line and a Ray
			--------------------------------------
		Post:	returns 1. P1, P2 set.
			--------------------------------------
		Param<L>: input line
		Param<R>: input ray
		Param<pl>: the point on L that is closest to R
		Param<pr>: the point on R that is closest to L
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineRay(const Line4& L, const Ray4& R, Point4& pl, Point4& pr)
{
	Point4 or(R.Origin());
	Point4 ol(L.Origin());
	Vector4 dr(R.Direction());
	Vector4 dl(L.Direction());

	//compute intermediate parameters
	Vector4 w0(or - ol);
	float a = Dot(dr, dr);
	float b = Dot(dr, dl);
	float c = Dot(dl, dl);
	float d = Dot(dr, w0);
	float e = Dot(dl, w0);

    float denom = a*c - b*b;

	//if denom is zero, try finding closest point on R to L.origin
	if (::IsZero(denom))
	{
		//compute closest points
		pr = or;
		pl = ol + (e/c)*dl;
	}
	else
	{
		//parameters to compute u1 and u2
		float sn, u1, u2;

		//clamp u1 to within [0,1]
		sn = b*e - c*d;

		//clamp u1 to 0
		if (sn < 0.0f)
		{
			u1 = 0.0f;
			u2 = e/c;
		}
		//clamp u1 to 1
		else if (sn > denom)
		{
			u1 = 1.0f;
			u2 = (e+b)/c;
		}
		else
		{
			u1 = sn/denom;
			u2 = (a*e - b*d)/denom;
		}

		//compute closest points
		pr = or + u1*dr;
		pl = ol + u2*dl;
	}

	return 1;

}	//End: ::ClosestPointsLineRay()