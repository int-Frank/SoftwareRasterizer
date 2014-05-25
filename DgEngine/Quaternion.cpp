//================================================================================
// @ Quaternion.cpp
// 
// Description: This file defines methods for the Quaternion class 
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

#include "Quaternion.h"
#include "CommonMath.h"
#include "Vector4.h"
#include "Point4.h"
#include "Matrix44.h"
#include "BasisR3.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------
Quaternion Quaternion::zero( 0.0f, 0.0f, 0.0f, 0.0f );
Quaternion Quaternion::identity( 1.0f, 0.0f, 0.0f, 0.0f );


//-------------------------------------------------------------------------------
//	@	Quaternion::Quaternion()
//-------------------------------------------------------------------------------
//		Axis-angle constructor
//-------------------------------------------------------------------------------
Quaternion::Quaternion( const Vector4& axis, float angle )
{
    Set( axis, angle );

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::Quaternion()
//-------------------------------------------------------------------------------
//		To-from vector constructor
//-------------------------------------------------------------------------------
Quaternion::Quaternion( const Vector4& from, const Vector4& to )
{
    Set( from, to );

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::Quaternion()
//-------------------------------------------------------------------------------
//		Vector constructor
//-------------------------------------------------------------------------------
Quaternion::Quaternion( const Vector4& vector )
{
    Set( 0.0f, vector.x, vector.y, vector.z );

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::Quaternion()
//-------------------------------------------------------------------------------
//		Rotation matrix constructor
//-------------------------------------------------------------------------------
Quaternion::Quaternion( const Matrix44& rotation )
{
	//Get trace
    float trace = rotation.mV[0] + rotation.mV[5] + rotation.mV[10]; 
    if ( trace > 0.0f )
    {
        float s = ::DgSqrt( trace + 1.0f );
        w = s*0.5f;
        float recip = 0.5f/s;
        x = (rotation(2,1) - rotation(1,2))*recip;
        y = (rotation(0,2) - rotation(2,0))*recip;
        z = (rotation(1,0) - rotation(0,1))*recip;
    }
    else
    {
        uint32 i = 0;
        if ( rotation(1,1) > rotation(0,0) )
            i = 1;
        if ( rotation(2,2) > rotation(i,i) )
            i = 2;
        uint32 j = (i+1)%3;
        uint32 k = (j+1)%3;
        float s = ::DgSqrt( rotation(i,i) - rotation(j,j) - rotation(k,k) + 1.0f );
        (*this)[i] = 0.5f*s;
        float recip = 0.5f/s;
        w = (rotation(k,j) - rotation(j,k))*recip;
        (*this)[j] = (rotation(j,i) + rotation(i,j))*recip;
        (*this)[k] = (rotation(k,i) + rotation(i,k))*recip;
    }

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::Quaternion()
//-------------------------------------------------------------------------------
//		Copy constructor
//-------------------------------------------------------------------------------
Quaternion::Quaternion(const Quaternion& other) : 
    w( other.w ), x( other.x ), y( other.y ), z( other.z )
{

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator=()
//-------------------------------------------------------------------------------
//		Assignment operator
//-------------------------------------------------------------------------------
Quaternion&
Quaternion::operator=(const Quaternion& other)
{
    w = other.w;
    x = other.x;
    y = other.y;
    z = other.z;
    
    return *this;

}   // End of Quaternion::operator=()


//-------------------------------------------------------------------------------
//	@	operator<<()
//-------------------------------------------------------------------------------
//		Text output for debugging
//-------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Quaternion& source)
{
    out << '<' << source.w << ' ' << source.x << ' ' 
        << source.y << ' ' << source.z << '>';

    return out;
    
}   // End of operator<<()
    

//-------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build Quaternion from in stream (4 values)
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, Quaternion& dest)
{
	//Read to temp values
    if ((in >> dest.w >> dest.x >> dest.y >> dest.z).fail())
    {
        std::cerr << "@operator>>(Quaternion) -> Bad read." << std::endl;
    }

	return in;

}	//End: operator>>()


//-------------------------------------------------------------------------------
//	@	Quaternion::Magnitude()
//-------------------------------------------------------------------------------
//		Quaternion magnitude (square root of norm)
//-------------------------------------------------------------------------------
float Quaternion::Magnitude() const
{
    return DgSqrt( w*w + x*x + y*y + z*z );

}   // End of Quaternion::Magnitude()


//-------------------------------------------------------------------------------
//	@	Quaternion::Norm()
//-------------------------------------------------------------------------------
//		Quaternion norm
//-------------------------------------------------------------------------------
float Quaternion::Norm() const
{
    return ( w*w + x*x + y*y + z*z );

}   // End of Quaternion::Norm()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator==()
//-------------------------------------------------------------------------------
//		Comparison operator
//-------------------------------------------------------------------------------
bool Quaternion::operator==( const Quaternion& other ) const
{
    if ( ::IsZero( other.w - w )
        && ::IsZero( other.x - x )
        && ::IsZero( other.y - y )
        && ::IsZero( other.z - z ) )
        return true;

    return false;

}   // End of Quaternion::operator==()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator!=()
//-------------------------------------------------------------------------------
//		Comparison operator
//-------------------------------------------------------------------------------
bool Quaternion::operator!=( const Quaternion& other ) const
{
    if ( ::IsZero( other.w - w )
        || ::IsZero( other.x - x )
        || ::IsZero( other.y - y )
        || ::IsZero( other.z - z ) )
        return false;

    return true;
}   // End of Quaternion::operator!=()


//-------------------------------------------------------------------------------
//	@	Quaternion::IsZero()
//-------------------------------------------------------------------------------
//		Check for zero quat
//-------------------------------------------------------------------------------
bool Quaternion::IsZero() const
{
    return ::IsZero(w*w + x*x + y*y + z*z);

}   // End of Quaternion::IsZero()


//-------------------------------------------------------------------------------
//	@	Quaternion::IsUnit()
//-------------------------------------------------------------------------------
//		Check for unit quat
//-------------------------------------------------------------------------------
bool Quaternion::IsUnit() const
{
    return ::IsZero(1.0f - w*w - x*x - y*y - z*z);

}   // End of Quaternion::IsUnit()


//-------------------------------------------------------------------------------
//	@	Quaternion::IsIdentity()
//-------------------------------------------------------------------------------
//		Check for identity quat
//-------------------------------------------------------------------------------
bool Quaternion::IsIdentity() const
{
    return ::IsZero(1.0f - w)
        && ::IsZero( x ) 
        && ::IsZero( y )
        && ::IsZero( z );

}   // End of Quaternion::IsIdentity()


//-------------------------------------------------------------------------------
//	@	Quaternion::Set()
//-------------------------------------------------------------------------------
//		Set quaternion based on fixed angles
//-------------------------------------------------------------------------------
void Quaternion::Set( float xRotation, float yRotation, float zRotation ) 
{
    zRotation *= 0.5f;
    yRotation *= 0.5f;
    xRotation *= 0.5f;

    // get sines and cosines of half angles
    float Cx, Sx;
    DgSinCos(xRotation, Sx, Cx);

    float Cy, Sy;
    DgSinCos(yRotation, Sy, Cy);

    float Cz, Sz;
    DgSinCos(zRotation, Sz, Cz);

    // multiply it out
    w = Cx*Cy*Cz - Sx*Sy*Sz;
    x = Sx*Cy*Cz + Cx*Sy*Sz;
    y = Cx*Sy*Cz - Sx*Cy*Sz;
    z = Cx*Cy*Sz + Sx*Sy*Cx;

	//Ensure valid orientation quaternion
	MakeValid();

}   // End of Quaternion::Set()


//-------------------------------------------------------------------------------
//	@	Quaternion::Set()
//-------------------------------------------------------------------------------
//		Set quaternion based on axis-angle
//-------------------------------------------------------------------------------
void Quaternion::Set( const Vector4& axis, float angle )
{
    // if axis of rotation is zero vector, just set to identity quat
    float length = axis.LengthSquared();
    if ( ::IsZero( length ) )
    {
        Identity();
        return;
    }

    // take half-angle
    angle *= 0.5f;

    float sintheta, costheta;
    DgSinCos(angle, sintheta, costheta);

    float scaleFactor = sintheta/DgSqrt( length );

    w = costheta;
    x = scaleFactor * axis.x;
    y = scaleFactor * axis.y;
    z = scaleFactor * axis.z;
	
	//Ensure valid orientation quaternion
	MakeValid();

}   // End of Quaternion::Set()


//-------------------------------------------------------------------------------
//	@	Quaternion::()
//-------------------------------------------------------------------------------
//		Set based on an origin, a target to orientate to, and a final up vector.
//-------------------------------------------------------------------------------
void Quaternion::Set(const Point4& origin, const Point4& target, const Vector4& up)
{
	//Find the direction to the target
	Vector4 direction = target - origin;

	//Find the basis vectors of the orientation
	BasisR3 basis(direction, up);

	//Convert basis to a matrix
	Matrix44 rot(basis);

	//Set quaternion from the basis
	Set(rot);

}	//End: Quaternion::Set()


//-------------------------------------------------------------------------------
//	@	Quaternion::()
//-------------------------------------------------------------------------------
//		Set quaternion based on start and end vectors
//-------------------------------------------------------------------------------
void Quaternion::Set( const Vector4& from, const Vector4& to )
{
   // get axis of rotation
    Vector4 axis = Cross(from, to);

    // get scaled cos of angle between vectors and set initial quaternion
    Set(  Dot(from, to), axis.x, axis.y, axis.z );
    // quaternion at this point is ||from||*||to||*( cos(theta), r*sin(theta) )

    // normalize to remove ||from||*||to|| factor
    Normalize();
    // quaternion at this point is ( cos(theta), r*sin(theta) )
    // what we want is ( cos(theta/2), r*sin(theta/2) )

    // set up for half angle calculation
    w += 1.0f;

    // now when we normalize, we'll be dividing by sqrt(2*(1+cos(theta))), which is 
    // what we want for r*sin(theta) to give us r*sin(theta/2)  (see pages 487-488)
    // 
    // w will become 
    //                 1+cos(theta)
    //            ----------------------
    //            sqrt(2*(1+cos(theta)))        
    // which simplifies to
    //                cos(theta/2)

    // before we normalize, check if vectors are opposing
    if ( w <= EPSILON )
    {
        // rotate pi radians around orthogonal vector
        // take cross product with x axis
        if ( from.z*from.z > from.x*from.x )
            Set( 0.0f, 0.0f, from.z, -from.y );
        // or take cross product with z axis
        else
            Set( 0.0f, from.y, -from.x, 0.0f );
    }
   
    // normalize again to get rotation quaternion
    Normalize();

}   // End of Quaternion::Set()


//-------------------------------------------------------------------------------
//	@	Quaternion::Set()
//-------------------------------------------------------------------------------
//		Set based on Rotation matrix
//-------------------------------------------------------------------------------
void Quaternion::Set( const Matrix44& rotation )
{
	//Get trace
    float trace = rotation.mV[0] + rotation.mV[5] + rotation.mV[10]; 
    if ( trace > 0.0f )
    {
        float s = ::DgSqrt( trace + 1.0f );
        w = s*0.5f;
        float recip = 0.5f/s;
        x = (rotation(2,1) - rotation(1,2))*recip;
        y = (rotation(0,2) - rotation(2,0))*recip;
        z = (rotation(1,0) - rotation(0,1))*recip;
    }
    else
    {
        uint32 i = 0;
        if ( rotation(1,1) > rotation(0,0) )
            i = 1;
        if ( rotation(2,2) > rotation(i,i) )
            i = 2;
        uint32 j = (i+1)%3;
        uint32 k = (j+1)%3;
        float s = ::DgSqrt( rotation(i,i) - rotation(j,j) - rotation(k,k) + 1.0f );
        (*this)[i] = 0.5f*s;
        float recip = 0.5f/s;
        w = (rotation(k,j) - rotation(j,k))*recip;
        (*this)[j] = (rotation(j,i) + rotation(i,j))*recip;
        (*this)[k] = (rotation(k,i) + rotation(i,k))*recip;
    }

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::Set()
//-------------------------------------------------------------------------------
//		Set based on 3 basis vectors
//-------------------------------------------------------------------------------
void Quaternion::Set(const BasisR3& basis)
{
	//Convert basis to a matrix
	Matrix44 rot(basis);

	//Set quaternion from the basis
	Set(rot);

}   // End of Quaternion::Quaternion()


//-------------------------------------------------------------------------------
//	@	Quaternion::MakeValid()
//-------------------------------------------------------------------------------
//		Ensures quaternion is valid for rotating
//-------------------------------------------------------------------------------
void Quaternion::MakeValid()
{
	//Set near-zero elements to zero
    Clean();

	//Find Norm
	float lengthsq = w*w + x*x + y*y + z*z;

	//Return if valid
	if (::IsZero(1.0f - lengthsq))
		return;

	//If zero quaternion
    if ( ::IsZero( lengthsq ) )
    {
		w = 1.0f;
        x = y = z = 0.0f;
    }
    else
    {
        float factor = DgInvSqrt( lengthsq );
        w *= factor;
        x *= factor;
        y *= factor;
        z *= factor;
    }

}   // End of Quaternion::Identity


//-------------------------------------------------------------------------------
//	@	Quaternion::GetAxisAngle()
//-------------------------------------------------------------------------------
//		Get axis-angle based on quaternion
//-------------------------------------------------------------------------------
void Quaternion::GetAxisAngle( Vector4& axis, float& angle )
{
    angle = 2.0f*DgACos( w );
    float length = ::DgSqrt( 1.0f - w*w );
    if ( ::IsZero(length) )
        axis.Zero();
    else
    {
        length = 1.0f/length;
        axis.Set( x*length, y*length, z*length );
    }

}   // End of Quaternion::GetAxisAngle()


//-------------------------------------------------------------------------------
//	@	Quaternion::Clean()
//-------------------------------------------------------------------------------
//		Set elements close to zero equal to zero
//-------------------------------------------------------------------------------
void Quaternion::Clean()
{
    if ( ::IsZero( w ) )
        w = 0.0f;
    if ( ::IsZero( x ) )
        x = 0.0f;
    if ( ::IsZero( y ) )
        y = 0.0f;
    if ( ::IsZero( z ) )
        z = 0.0f;

}   // End of Quaternion::Clean()


//-------------------------------------------------------------------------------
//	@	Quaternion::Normalize()
//-------------------------------------------------------------------------------
//		Set to unit quaternion
//-------------------------------------------------------------------------------
void Quaternion::Normalize()
{
    float lengthsq = w*w + x*x + y*y + z*z;

    if ( ::IsZero( lengthsq ) )
    {
        Zero();
    }
    else
    {
        float factor = DgInvSqrt( lengthsq );
        w *= factor;
        x *= factor;
        y *= factor;
        z *= factor;
    }

}   // End of Quaternion::Normalize()


//-------------------------------------------------------------------------------
//	@	Quaternion::Conjugate()
//-------------------------------------------------------------------------------
//		Compute complex conjugate
//-------------------------------------------------------------------------------
Quaternion Conjugate( const Quaternion& quat ) 
{
    return Quaternion( quat.w, -quat.x, -quat.y, -quat.z );

}   // End of Conjugate()


//-------------------------------------------------------------------------------
//	@	Quaternion::Conjugate()
//-------------------------------------------------------------------------------
//		Set self to complex conjugate
//-------------------------------------------------------------------------------
const Quaternion& Quaternion::Conjugate()
{
    x = -x;
    y = -y;
    z = -z;

    return *this;

}   // End of Conjugate()


//-------------------------------------------------------------------------------
//	@	Quaternion::Inverse()
//-------------------------------------------------------------------------------
//		Compute quaternion inverse
//-------------------------------------------------------------------------------
Quaternion Inverse( const Quaternion& quat )
{
    float norm = quat.w*quat.w + quat.x*quat.x + quat.y*quat.y + quat.z*quat.z;
    // if we're the zero quaternion, just return identity
    if ( ::IsZero( norm ) )
    {
        return Quaternion();
    }

    float normRecip = 1.0f / norm;
    return Quaternion( normRecip*quat.w, -normRecip*quat.x, -normRecip*quat.y, 
                   -normRecip*quat.z );

}   // End of Inverse()


//-------------------------------------------------------------------------------
//	@	Quaternion::Inverse()
//-------------------------------------------------------------------------------
//		Set self to inverse
//-------------------------------------------------------------------------------
const Quaternion& Quaternion::Inverse()
{
    float norm = w*w + x*x + y*y + z*z;
    // if we're the zero quaternion, just return
    if ( ::IsZero( norm ) )
        return *this;

    float normRecip = 1.0f / norm;
    w = normRecip*w;
    x = -normRecip*x;
    y = -normRecip*y;
    z = -normRecip*z;

    return *this;

}   // End of Inverse()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator+()
//-------------------------------------------------------------------------------
//		Add quat to self and return
//-------------------------------------------------------------------------------
Quaternion Quaternion::operator+( const Quaternion& other ) const
{
    return Quaternion( w + other.w, x + other.x, y + other.y, z + other.z );

}   // End of Quaternion::operator+()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator+=()
//-------------------------------------------------------------------------------
//		Add quat to self, store in self
//-------------------------------------------------------------------------------
Quaternion& Quaternion::operator+=( const Quaternion& other )
{
    w += other.w;
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;

}   // End of Quaternion::operator+=()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator-()
//-------------------------------------------------------------------------------
//		Subtract quat from self and return
//-------------------------------------------------------------------------------
Quaternion Quaternion::operator-( const Quaternion& other ) const
{
    return Quaternion( w - other.w, x - other.x, y - other.y, z - other.z );

}   // End of Quaternion::operator-()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator-=()
//-------------------------------------------------------------------------------
//		Subtract quat from self, store in self
//-------------------------------------------------------------------------------
Quaternion& Quaternion::operator-=( const Quaternion& other )
{
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;

}   // End of Quaternion::operator-=()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator-()
//-------------------------------------------------------------------------------
//		Negate self and return
//-------------------------------------------------------------------------------
Quaternion Quaternion::operator-() const
{
    return Quaternion(-w, -x, -y, -z);

}    // End of Quaternion::operator-()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator*()
//-------------------------------------------------------------------------------
//		Scalar multiplication
//-------------------------------------------------------------------------------
Quaternion operator*( float scalar, const Quaternion& quat )
{
    return Quaternion( scalar*quat.w, scalar*quat.x, scalar*quat.y, scalar*quat.z );

}   // End of operator*()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator*=()
//-------------------------------------------------------------------------------
//		Scalar multiplication by self
//-------------------------------------------------------------------------------
Quaternion& Quaternion::operator*=( float scalar )
{
    w *= scalar;
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;

}   // End of Quaternion::operator*=()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator*()
//-------------------------------------------------------------------------------
//		Quaternion multiplication
//-------------------------------------------------------------------------------
Quaternion Quaternion::operator*( const Quaternion& other ) const
{
    return Quaternion( w*other.w - x*other.x - y*other.y - z*other.z,
                   w*other.x + x*other.w + y*other.z - z*other.y,
                   w*other.y + y*other.w + z*other.x - x*other.z,
                   w*other.z + z*other.w + x*other.y - y*other.x );

}   // End of Quaternion::operator*()


//-------------------------------------------------------------------------------
//	@	Quaternion::operator*=()
//-------------------------------------------------------------------------------
//		Quaternion multiplication by self
//-------------------------------------------------------------------------------
Quaternion& Quaternion::operator*=( const Quaternion& other )
{
    Set( w*other.w - x*other.x - y*other.y - z*other.z,
         w*other.x + x*other.w + y*other.z - z*other.y,
         w*other.y + y*other.w + z*other.x - x*other.z,
         w*other.z + z*other.w + x*other.y - y*other.x );
  
    return *this;

}   // End of Quaternion::operator*=()


//-------------------------------------------------------------------------------
//	@	Dot()
//-------------------------------------------------------------------------------
//		Dot product friend operator
//-------------------------------------------------------------------------------
float Dot( const Quaternion& quat1, const Quaternion& quat2 )
{
    return (quat1.w*quat2.w + quat1.x*quat2.x + quat1.y*quat2.y + quat1.z*quat2.z);

}   // End of Dot()


//-------------------------------------------------------------------------------
//	@	Quaternion::Rotate()
//-------------------------------------------------------------------------------
//		Rotate vector by quaternion
//		Assumes quaternion is normalized!
//-------------------------------------------------------------------------------
Vector4 Quaternion::Rotate( const Vector4& vector ) const
{
    float vMult = 2.0f*(x*vector.x + y*vector.y + z*vector.z);
    float crossMult = 2.0f*w;
    float pMult = crossMult*w - 1.0f;

    return Vector4( pMult*vector.x + vMult*x + crossMult*(y*vector.z - z*vector.y),
                    pMult*vector.y + vMult*y + crossMult*(z*vector.x - x*vector.z),
                    pMult*vector.z + vMult*z + crossMult*(x*vector.y - y*vector.x)  );

}   // End of Quaternion::Rotate()


//-------------------------------------------------------------------------------
//	@	Quaternion::Rotate()
//-------------------------------------------------------------------------------
//		Rotate point by quaternion, returns a point
//		Assumes quaternion is normalized!
//-------------------------------------------------------------------------------
Point4 Quaternion::Rotate( const Point4& point ) const
{
    float vMult = 2.0f*(x*point.x + y*point.y + z*point.z);
    float crossMult = 2.0f*w;
    float pMult = crossMult*w - 1.0f;

    return Point4( pMult*point.x + vMult*x + crossMult*(y*point.z - z*point.y),
                   pMult*point.y + vMult*y + crossMult*(z*point.x - x*point.z),
                   pMult*point.z + vMult*z + crossMult*(x*point.y - y*point.x)  );

}   // End of Quaternion::Rotate()


//-------------------------------------------------------------------------------
//	@	Quaternion::RotateSelf()
//-------------------------------------------------------------------------------
//		Rotate Vector4 by quaternion, store in self
//		Assumes quaternion is normalized!
//-------------------------------------------------------------------------------
void Quaternion::RotateSelf( Vector4& vector ) const
{
    float vMult = 2.0f*(x*vector.x + y*vector.y + z*vector.z);
    float crossMult = 2.0f*w;
    float pMult = crossMult*w - 1.0f;

    vector.Set( pMult*vector.x + vMult*x + crossMult*(y*vector.z - z*vector.y),
               pMult*vector.y + vMult*y + crossMult*(z*vector.x - x*vector.z),
               pMult*vector.z + vMult*z + crossMult*(x*vector.y - y*vector.x)  );

}   // End of Quaternion::RotateSelf()


//-------------------------------------------------------------------------------
//	@	Quaternion::RotateSelf()
//-------------------------------------------------------------------------------
//		Rotate Point4 by quaternion, store in self
//		Assumes quaternion is normalized!
//-------------------------------------------------------------------------------
void Quaternion::RotateSelf( Point4& point ) const
{
    float vMult = 2.0f*(x*point.x + y*point.y + z*point.z);
    float crossMult = 2.0f*w;
    float pMult = crossMult*w - 1.0f;

    point.Set( pMult*point.x + vMult*x + crossMult*(y*point.z - z*point.y),
               pMult*point.y + vMult*y + crossMult*(z*point.x - x*point.z),
               pMult*point.z + vMult*z + crossMult*(x*point.y - y*point.x)  );

}   // End of Quaternion::RotateSelf()


//-------------------------------------------------------------------------------
//	@	Quaternion::Lerp()
//-------------------------------------------------------------------------------
//		Linearly interpolate two quaternions
//		This will always take the shorter path between them
//-------------------------------------------------------------------------------
void Lerp( Quaternion& result, const Quaternion& start, const Quaternion& end, float t )
{
    // get cos of "angle" between quaternions
    float cosTheta = Dot(start, end);

    // initialize result
    result = t*end;

    // if "angle" between quaternions is less than 90 degrees
    if ( cosTheta >= EPSILON )
    {
        // use standard interpolation
        result += (1.0f-t)*start;
    }
    else
    {
        // otherwise, take the shorter path
        result += (t-1.0f)*start;
    }

}   // End of Lerp()


//-------------------------------------------------------------------------------
//	@	Quaternion::Slerp()
//-------------------------------------------------------------------------------
//		Spherical linearly interpolate two quaternions
//		This will always take the shorter path between them
//-------------------------------------------------------------------------------
void Slerp( Quaternion& result, const Quaternion& start, const Quaternion& end, float t )
{
    // get cosine of "angle" between quaternions
    float cosTheta = Dot(start, end);
    float startInterp, endInterp;

    // if "angle" between quaternions is less than 90 degrees
    if ( cosTheta >= EPSILON )
    {
        // if angle is greater than zero
        if ( (1.0f - cosTheta) > EPSILON )
        {
            // use standard slerp
            float theta = DgACos( cosTheta );
            float recipSinTheta = 1.0f/DgSin( theta );

            startInterp = DgSin( (1.0f - t)*theta )*recipSinTheta;
            endInterp = DgSin( t*theta )*recipSinTheta;
        }
        // angle is close to zero
        else
        {
            // use linear interpolation
            startInterp = 1.0f - t;
            endInterp = t;
        }
    }
    // otherwise, take the shorter route
    else
    {
        // if angle is less than 180 degrees
        if ( (1.0f + cosTheta) > EPSILON )
        {
            // use slerp w/negation of start quaternion
            float theta = DgACos( -cosTheta );
            float recipSinTheta = 1.0f/DgSin( theta );

            startInterp = DgSin( (t-1.0f)*theta )*recipSinTheta;
            endInterp = DgSin( t*theta )*recipSinTheta;
        }
        // angle is close to 180 degrees
        else
        {
            // use lerp w/negation of start quaternion
            startInterp = t - 1.0f;
            endInterp = t;
        }
    }
    
    result = startInterp*start + endInterp*end;

}   // End of Slerp()


//-------------------------------------------------------------------------------
//	@	Quaternion::ApproxSlerp()
//-------------------------------------------------------------------------------
//		Approximate spherical linear interpolation of two quaternions
//		Based on "Hacking Quaternions", Jonathan Blow, Game Developer, March 2002.
//		See Game Developer, February 2004 for an alternate method.
//-------------------------------------------------------------------------------
void ApproxSlerp( Quaternion& result, const Quaternion& start, const Quaternion& end, float t )
{
    float cosTheta = Dot(start, end);

    // correct time by using cosine of angle between quaternions
    float factor = 1.0f - 0.7878088f*cosTheta;
    float k = 0.5069269f;
    factor *= factor;
    k *= factor;

    float b = 2*k;
    float c = -3*k;
    float d = 1 + k;

    t = t*(b*t + c) + d;

    // initialize result
    result = t*end;

    // if "angle" between quaternions is less than 90 degrees
    if ( cosTheta >= EPSILON )
    {
        // use standard interpolation
        result += (1.0f-t)*start;
    }
    else
    {
        // otherwise, take the shorter path
        result += (t-1.0f)*start;
    }

}   // End of ApproxSlerp()
