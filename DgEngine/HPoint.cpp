//================================================================================
// @ HPoint.cpp
// 
// Description: This file defines HPoint methods.
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

#include "HPoint.h"
#include "CommonMath.h"

//-------------------------------------------------------------------------------
//	@	HPoint::operator=()
//-------------------------------------------------------------------------------
// Assignment operator
//-------------------------------------------------------------------------------
HPoint& HPoint::operator=(const HPoint& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    
    return *this;

}   // End:  HPoint::operator=()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const HPoint& source)
{
	out << '<' << source.x << ' ' << source.y << ' ' <<
		source.z << ' ' << source.w << '>';

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build vector from in stream (4 values)
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, HPoint& dest)
{
	//Read to temp values
    if ((in >> dest.x >> dest.y >> dest.z >> dest.w).fail())
    {
        std::cerr << "@operator>>(HPoint) -> Bad read." << std::endl;
    }

	return in;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	HPoint::operator==()
//--------------------------------------------------------------------------------
//		Are two HPoints equal?
//--------------------------------------------------------------------------------
bool HPoint::operator== (const HPoint& other) const
{
    if ( ::DgAreEqual( other.x, x )
        && ::DgAreEqual( other.y, y )
        && ::DgAreEqual( other.z, z )
        && ::DgAreEqual( other.w, w ) )
        return true;

    return false; 

}	//End: HPoint::operator==()


//--------------------------------------------------------------------------------
//	@	HPoint::operator!=()
//--------------------------------------------------------------------------------
//		Are two HPoints not equal?
//--------------------------------------------------------------------------------
bool HPoint::operator!= (const HPoint& other) const
{
    if ( ::DgAreEqual( other.x, x )
        && ::DgAreEqual( other.y, y )
        && ::DgAreEqual( other.z, z )
        && ::DgAreEqual( other.w, w ) )
        return false;

    return true; 

}	//End: HPoint::operator!=()
