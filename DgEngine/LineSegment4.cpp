//================================================================================
// @ LineSegment4.cpp
// 
// Description:
//
// This file defines LineSegment4's methods and Testing functions 
// involving LineSegments, Rays and Lines.
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

#include "LineSegment4.h"
#include "Line4.h"
#include "Ray4.h"
#include "CommonMath.h"


//--------------------------------------------------------------------------------
//	@	LineSegment4::LineSegment4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
LineSegment4::LineSegment4(): 
origin(0.0f, 0.0f, 0.0f),
direction(1.0f, 0.0f, 0.0f) 
{
}	//End: LineSegment4::LineSegment4


//--------------------------------------------------------------------------------
//	@	LineSegment4::LineSegment4()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
LineSegment4::LineSegment4(const Point4& p1, const Point4& p2):
origin(p1),
direction(p2-p1)
{
}	//End: LineSegment4::LineSegment4()


//--------------------------------------------------------------------------------
//	@	LineSegment4::LineSegment4()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
LineSegment4::LineSegment4(const LineSegment4& other):
origin(other.origin),
direction(other.direction)
{
}	//End: LineSegment4::LineSegment4()


//--------------------------------------------------------------------------------
//	@	LineSegment4::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
LineSegment4& LineSegment4::operator= (const LineSegment4& other)
{
	origin = other.origin;
	direction = other.direction;

	return *this;

}	//End: LineSegment4::operator=


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const LineSegment4& source)
{
	out << "[" << source.GetP1() << ", " 
		<< source.GetP2() << "]";

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	LineSegment4::Get()
//--------------------------------------------------------------------------------
//		Accessor
//--------------------------------------------------------------------------------
void LineSegment4::Get(Point4& p1, Point4& p2) const
{
	p1 = origin;
	p2 = origin + direction;

}	//End: LineSegment4::Get()


//--------------------------------------------------------------------------------
//	@	LineSegment4::Length()
//--------------------------------------------------------------------------------
//		Returns length of the LineSegment
//--------------------------------------------------------------------------------
float LineSegment4::Length() const
{
	return direction.Length();

}	//End: LineSegment4::Length()


//--------------------------------------------------------------------------------
//	@	LineSegment4::LengthSquared()
//--------------------------------------------------------------------------------
//		Returns length squared of the LineSegment
//--------------------------------------------------------------------------------
float LineSegment4::LengthSquared() const
{
	return direction.LengthSquared();

}	//End: LineSegment4::LengthSquared()


//--------------------------------------------------------------------------------
//	@	LineSegment4::operator==()
//--------------------------------------------------------------------------------
//		Are two LineSegment4's equal?
//--------------------------------------------------------------------------------
bool LineSegment4::operator== (const LineSegment4& LS) const
{
	return ((LS.origin == origin && LS.direction == direction) ||
			(LS.origin == origin+direction && LS.direction == -direction));

}	//End: LineSegment4::operator==()


//--------------------------------------------------------------------------------
//	@	LineSegment4::operator!=()
//--------------------------------------------------------------------------------
//		Are two LineSegment4's not equal?
//--------------------------------------------------------------------------------
bool LineSegment4::operator!= (const LineSegment4& LS) const
{
	return !((LS.origin == origin && LS.direction == direction) ||
			(LS.origin == origin+direction && LS.direction == -direction));

}	//End: LineSegment4::operator!=


//--------------------------------------------------------------------------------
//	@	LineSegment4::ClosestPoint()
//---------------------------------------------------------------------------
//		Returns the closest point on line segment to point
//-----------------------------------------------------------------------------
Point4 LineSegment4::ClosestPoint( const Point4& point ) const
{
    Vector4 w = point - origin;

    float proj = Dot(w,direction);
    // endpoint 0 is closest point
    if ( proj <= 0.0f )
        return origin;
    else
    {
        float vsq = Dot(direction,direction);
        // endpoint 1 is closest point
        if ( proj >= vsq )
            return origin + direction;
        // else somewhere else in segment
        else
            return origin + (proj/vsq)*direction;
    }

}	//End: ClosestPoint()


//--------------------------------------------------------------------------------
//	@	SqDistLineSegmentLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between two LineSegments
			--------------------------------------
		Post:	returns 1. sqDist set, u1 and u2 are set.
			--------------------------------------
		Param<LS1>:	 input linesegment
		Param<LS2>:	 input linesegment
		Param<sqDist>: squared distance between the linesegments
		Param<u1>:	 distance along LS1 to the intersection or 
		             closest point to LS2
		Param<u2>:	 distance along LS2 to the intersection or 
		             closest point to LS1
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineSegmentLineSegment(const LineSegment4& LS1, const LineSegment4& LS2, 
								   float& sqDist, float& u1, float& u2)
{
	Point4 o1 = LS1.Origin();
	Point4 o2 = LS2.Origin();
	Vector4 d1 = LS1.Direction();
	Vector4 d2 = LS2.Direction();

	//compute intermediate parameters
	Vector4 w0 = o1 - o2;
	float a = Dot(d1, d1);
	float b = Dot(d1, d2);
	float c = Dot(d2, d2);
	float d = Dot(d1, w0);
	float e = Dot(d2, w0);
	float denom = a*c - b*b;

	//parameters to compute u1 and u2
	float sn, sd, tn, td;

	//if denom is zero, try finding closest point on LS2 to origin
	if (::IsZero(denom))
	{
		sd = td = c;
		sn = 0.0f;
		tn = e;
	}
	else
	{
		//clamp u1 within [0,1]
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
		//clamp u1 to 1
		else if (sn > sd)
		{
			sn = sd;
			tn = e + b;
			td = c;
		}
	}

	// clamp u2 within [0,1]
    // clamp u2 to 0
    if (tn < 0.0f)
    {
        u2 = 0.0f;
        // clamp u1 to 0
        if ( -d < 0.0f )
        {
            u1 = 0.0f;
        }
        // clamp u1 to 1
        else if ( -d > a )
        {
            u1 = 1.0f;
        }
        else
        {
            u1 = -d/a;
        }
    }
	// clamp u2 to 1
    else if (tn > td)
    {
        u2 = 1.0f;
        // clamp u1 to 0
        if ( (-d+b) < 0.0f )
        {
            u1 = 0.0f;
        }
        // clamp u1 to 1
        else if ( (-d+b) > a )
        {
            u1 = 1.0f;
        }
        else
        {
            u1 = (-d+b)/a;
        }
    }
    else
    {
        u2 = tn/td;
        u1 = sn/sd;
    }

	//compute difference vector and distance squared
	Vector4 wc = w0 + u1*d1 - u2*d2;
	sqDist = wc.LengthSquared();

	return 1;

}	//End: SqDistLineSegmentLineSegment()


//--------------------------------------------------------------------------------
//	@	SqDistLineLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a Line and a LineSegment.
			--------------------------------------
		Post:	returns 1. sqDist set, ul and uls are set.
			--------------------------------------
		Param<L>:	 input line
		Param<LS>:	 input linesegment
		Param<sqDist>: squared distance between the linesegments
		Param<ul>:	 distance along L to the intersection or 
		             closest point to LS
		Param<uls>:	 distance along LS to the intersection or 
		             closest point to L
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineLineSegment(const Line4& L, const LineSegment4& LS,  
							float& sqDist, float& ul, float& uls )
{
    Point4 os = LS.Origin();
	Point4 ol = L.Origin();
	Vector4 ds = LS.Direction();
	Vector4 dl = L.Direction();

	//compute intermediate parameters
	Vector4 w0 = os - ol;
	float a = Dot(ds, ds);
	float b = Dot(ds, dl);
	float c = Dot(dl, dl);
	float d = Dot(ds, w0);
	float e = Dot(dl, w0);

    float denom = a*c - b*b;

    // if denom is zero, try finding closest point on segment1 to origin0
    if ( ::IsZero(denom) )
    {
        uls = 0.0f;
        ul = e/c;
        // compute difference vector and distance squared
        Vector4 wc = w0 - ul*dl;
        sqDist =  Dot(wc,wc);
		return 1;
    }
    else
    {
        // parameters to compute uls, ul
        float sn;

        // clamp uls within [0,1]
        sn = b*e - c*d;
  
        // clamp uls to 0
        if (sn < 0.0f)
        {
            uls = 0.0f;
            ul = e/c;
        }
        // clamp uls to 1
        else if (sn > denom)
        {
            uls = 1.0f;
            ul = (e+b)/c;
        }
        else
        {
            uls = sn/denom;
            ul = (a*e - b*d)/denom;
        }

        // compute difference vector and distance squared
        Vector4 wc = w0 + uls*ds - ul*dl;
		sqDist = wc.LengthSquared();

		return 1;
    }

}   // End of ::SqDistLineLineSegment()


//--------------------------------------------------------------------------------
//	@	SqDistLineSegmentRay()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a LineSegment and a Ray.
			--------------------------------------
		Post:	returns 1. sqDist set, uls and ur are set.
			--------------------------------------
		Param<LS>:	 input line
		Param<R>:	 input linesegment
		Param<sqDist>: squared distance between the linesegments
		Param<uls>:	 distance along LS to the intersection or 
		             closest point to R
		Param<ur>:	 distance along R to the intersection or 
		             closest point to LS
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineSegmentRay( const LineSegment4& LS, const Ray4& R, 
						   float& sqDist, float& uls, float& ur )
{
    Point4 os = LS.Origin();
	Point4 or = R.Origin();
	Vector4 ds = LS.Direction();
	Vector4 dr = R.Direction();

	//compute intermediate parameters
	Vector4 w0 = os - or;
	float a = Dot(ds, ds);
	float b = Dot(ds, dr);
	float c = Dot(dr, dr);
	float d = Dot(ds, w0);
	float e = Dot(dr, w0);

    float denom = a*c - b*b;
    // parameters to compute uls, ur
    float sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if ( ::IsZero(denom) )
    {
        // clamp uls to 0
        sd = td = c;
        sn = 0.0f;
        tn = e;
    }
    else
    {
        // clamp uls within [0,1]
        sd = td = denom;
        sn = b*e - c*d;
        tn = a*e - b*d;
  
        // clamp uls to 0
        if (sn < 0.0f)
        {
            sn = 0.0f;
            tn = e;
            td = c;
        }
        // clamp uls to 1
        else if (sn > sd)
        {
            sn = sd;
            tn = e + b;
            td = c;
        }
    }

    // clamp ur within [0,+inf]
    // clamp ur to 0
    if (tn < 0.0f)
    {
        ur = 0.0f;
        // clamp uls to 0
        if ( -d < 0.0f )
        {
            uls = 0.0f;
        }
        // clamp uls to 1
        else if ( -d > a )
        {
            uls = 1.0f;
        }
        else
        {
            uls = -d/a;
        }
    }
    else
    {
        ur = tn/td;
        uls = sn/sd;
    }

    // compute difference vector and distance squared
    Vector4 wc = w0 + uls*ds - ur*dr;

    sqDist = wc.LengthSquared();
	return 1;

}   // End of ::SqDistLineSegmentRay()


//--------------------------------------------------------------------------------
//	@	SqDistLineSegmentPoint()
//--------------------------------------------------------------------------------
/*		Summary: Shortest distance (squared) between a LineSegment and a Point.
			--------------------------------------
		Post:	Case endpoint 0 is closest: returns 1. P, u set.
		 		Case endpoint 1 is closest: returns 2. P, u set.
				Case somewhere in the middle: returns 3. P, u set.
			--------------------------------------
		Param<LS>:		input linesegment
		Param<P>:		closest point is stored here
		Param<sqDist>:	distance to P
		Param<u>:		distance along LS to the closest point to P
*/
//--------------------------------------------------------------------------------
uint8 SqDistLineSegmentPoint( const LineSegment4& LS, const Point4& P,
							  float& sqDist, float& u ) 
{
	Vector4 ds = LS.Direction();
    Vector4 w = P - LS.Origin();

    float proj = Dot(w,ds);
    // endpoint 0 is closest point
    if ( proj <= 0 )
    {
        u = 0.0f;
		sqDist = w.LengthSquared();
		return 1;
    }
    else
    {
        float vsq = Dot(ds, ds);
        // endpoint 1 is closest point
        if ( proj >= vsq )
        {
            u = 1.0f;
			sqDist = w.LengthSquared() - 2.0f*proj + vsq;
			return 2;
        }
        // otherwise somewhere else in segment
        else
        {
            u = proj/vsq;
			sqDist = w.LengthSquared() - u*proj;
            return 3;
        }
    }

}   // End of ::SqDistLineSegmentPoint()


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineSegmentLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Finds the closest points between two linesegments
			--------------------------------------
		Post:	returns 1. p1, p2 set.
			--------------------------------------
		Param<segment1>:	input linesegment
		Param<segment2>:	input linesegment
		Param<p1>:			point along segment1 closest to segment2
		Param<p2>:			point along segment2 closest to segment1
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineSegmentLineSegment(	const LineSegment4& segment1, 
					const LineSegment4& segment2,
					Point4& p1, Point4& p2   )
{
    Point4 o1 = segment1.Origin();
	Point4 o2 = segment2.Origin();
	Vector4 d1 = segment1.Direction();
	Vector4 d2 = segment2.Direction();

	//compute intermediate parameters
	Vector4 w0 = o1 - o2;
	float a = Dot(d1, d1);
	float b = Dot(d1, d2);
	float c = Dot(d2, d2);
	float d = Dot(d1, w0);
	float e = Dot(d2, w0);

    float denom = a*c - b*b;
    // parameters to compute u1, u2
    float u1, u2;
    float sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if ( ::IsZero(denom) )
    {
        // clamp u1 to 0
        sd = td = c;
        sn = 0.0f;
        tn = e;
    }
    else
    {
        // clamp u1 within [0,1]
        sd = td = denom;
        sn = b*e - c*d;
        tn = a*e - b*d;
  
        // clamp u1 to 0
        if (sn < 0.0f)
        {
            sn = 0.0f;
            tn = e;
            td = c;
        }
        // clamp u1 to 1
        else if (sn > sd)
        {
            sn = sd;
            tn = e + b;
            td = c;
        }
    }

    // clamp u2 within [0,1]
    // clamp u2 to 0
    if (tn < 0.0f)
    {
        u2 = 0.0f;
        // clamp u1 to 0
        if ( -d < 0.0f )
        {
            u1 = 0.0f;
        }
        // clamp u1 to 1
        else if ( -d > a )
        {
            u1 = 1.0f;
        }
        else
        {
            u1 = -d/a;
        }
    }
    // clamp u2 to 1
    else if (tn > td)
    {
        u2 = 1.0f;
        // clamp u1 to 0
        if ( (-d+b) < 0.0f )
        {
            u1 = 0.0f;
        }
        // clamp u1 to 1
        else if ( (-d+b) > a )
        {
            u1 = 1.0f;
        }
        else
        {
            u1 = (-d+b)/a;
        }
    }
    else
    {
        u2 = tn/td;
        u1 = sn/sd;
    }

    // compute closest points
    p1 = o1 + u1*d1;
    p2 = o2 + u2*d2;

	return 1;
}   // End of ClosestPointsLineSegmentLineSegment()


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineLineSegment()
//--------------------------------------------------------------------------------
/*		Summary: Finds the closest points between a linesegment and a line
			--------------------------------------
		Post:	returns 1. p1, p2 set.
			--------------------------------------
		Param<segment>:	input linesegment
		Param<line>:	input line
		Param<p1>:		point along segment closest to line
		Param<p2>:		point along line closest to segment
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineLineSegment( const Line4& line, const LineSegment4& segment, 
									Point4& p1, Point4& p2)
{
    Point4 os = segment.Origin();
	Point4 ol = line.Origin();
	Vector4 ds = segment.Direction();
	Vector4 dl = line.Direction();

	//compute intermediate parameters
	Vector4 w0 = os - ol;
	float a = Dot(ds, dl);
	float b = Dot(ds, dl);
	float c = Dot(dl, dl);
	float d = Dot(ds, w0);
	float e = Dot(dl, w0);

    float denom = a*c - b*b;

    // if denom is zero, try finding closest point on line to segment origin
    if ( ::IsZero(denom) )
    {
        // compute closest points
        p2 = os;
        p1 = ol + (e/c)*dl;
		return 1;
    }
    else
    {
        // parameters to compute u1, u2
        float u1, u2;
        float sn;

        // clamp u1 within [0,1]
        sn = b*e - c*d;
  
        // clamp u1 to 0
        if (sn < 0.0f)
        {
            u1 = 0.0f;
            u2 = e/c;
        }
        // clamp u1 to 1
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

        // compute closest points
        p1 = ol + u2*dl;
        p2 = os + u1*ds;
		return 1;
    }

}   // End of ClosestPointsLineLineSegment()


//--------------------------------------------------------------------------------
//	@	ClosestPointsLineSegmentRay()
//--------------------------------------------------------------------------------
/*		Summary: Finds the closest points between a linesegment and a ray
			--------------------------------------
		Post:	returns 1. p1, p2 set.
			--------------------------------------
		Param<segment>:	input linesegment
		Param<ray>:		input ray
		Param<p1>:		point along segment closest to ray
		Param<p2>:		point along ray closest to segment
*/
//--------------------------------------------------------------------------------
uint8 ClosestPointsLineSegmentRay( const LineSegment4& segment, const Ray4& ray, 
								   Point4& p1, Point4& p2 )
{
    Point4 os = segment.Origin();
	Point4 or = ray.Origin();
	Vector4 ds = segment.Direction();
	Vector4 dr = ray.Direction();

	//compute intermediate parameters
	Vector4 w0 = os - or;
	float a = Dot(ds, ds);
	float b = Dot(ds, dr);
	float c = Dot(dr, dr);
	float d = Dot(ds, w0);
	float e = Dot(dr, w0);

    float denom = a*c - b*b;
    // parameters to compute u1, u2
    float u1, u2;
    float sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if ( ::IsZero(denom) )
    {
        // clamp u1 to 0
        sd = td = c;
        sn = 0.0f;
        tn = e;
    }
    else
    {
        // clamp u1 within [0,1]
        sd = td = denom;
        sn = b*e - c*d;
        tn = a*e - b*d;
  
        // clamp u1 to 0
        if (sn < 0.0f)
        {
            sn = 0.0f;
            tn = e;
            td = c;
        }
        // clamp u1 to 1
        else if (sn > sd)
        {
            sn = sd;
            tn = e + b;
            td = c;
        }
    }

    // clamp u2 within [0,+inf]
    // clamp u2 to 0
    if (tn < 0.0f)
    {
        u2 = 0.0f;
        // clamp u1 to 0
        if ( -d < 0.0f )
        {
            u1 = 0.0f;
        }
        // clamp u1 to 1
        else if ( -d > a )
        {
            u1 = 1.0f;
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

    // compute closest points
    p1 = os + u1*ds;
    p2 = or + u2*dr;
	return 1;

}   // End of ClosestPointsLineSegmentRay()