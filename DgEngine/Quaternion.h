//================================================================================
// @ Quaternion.h
// 
// Class: Quaternion
//
// Quaternion class with methods tailored for rotating points and vectors. The
// quaternion 4-tuple can theoretically take on any values, however for 
// orientation representation in 3D space, it is required the quaternion is 
// normalised. It is up to the user to ensure the 
// quaternion is normalised if setting each element itself, for eg
//
//		Quaternion q(1.0, 2.3, -2.9, 4.6);	//Not a valid rotational quaternion
//		q.Normalize();						//Valid rotational quaternion
//
// Quaternions constructed or set by any other means will be valid for eg
//
//		//Valid quaternion set
//		void Set( float zRotation, float yRotation, float xRotation ); 
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

#ifndef QUATERNION_H
#define QUATERNION_H

#include "Dg_io.h"
#include "DgTypes.h"

class Vector4;
class Point4;
class Matrix44;
class BasisR3;
class HPoint;

//--------------------------------------------------------------------------------
//	@	Quaternion
//--------------------------------------------------------------------------------
class Quaternion
{
    friend class Matrix44;
    friend class VQS;
public:
    // constructor/destructor
    inline Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    inline Quaternion( float _w, float _x, float _y, float _z ) :
        w(_w), x(_x), y(_y), z(_z) {}
    inline ~Quaternion() {}

	//Constructors from 4D math
	Quaternion(const Vector4& axis, float angle);
    Quaternion(const Vector4& from, const Vector4& to);
	explicit Quaternion(const Vector4&);
	explicit Quaternion(const Matrix44& rotation);

    // copy operations
    Quaternion(const Quaternion& other);
    Quaternion& operator=(const Quaternion& other);

    //Input/output
    friend DgWriter& operator<<(DgWriter& out, const Quaternion& source);
    friend DgReader& operator>>(DgReader& in, Quaternion& dest);

    // accessors
    inline float& operator[]( uint32 i )         { return (&x)[i]; }
    inline float operator[]( uint32 i ) const    { return (&x)[i]; }

    float Magnitude() const;
    float Norm() const;

    // comparison
    bool operator==(const Quaternion&) const;
    bool operator!=(const Quaternion&) const;
    bool IsZero() const;
    bool IsUnit() const;
    bool IsIdentity() const;

    // manipulators
    inline void Set( float _w, float _x, float _y, float _z );
    void Set( float zRotation, float yRotation, float xRotation ); 

	//! Set based on an origin, a target to orientate to, and a final up vector.
	void Set(const Point4& origin, const Point4& target, const Vector4& up);

	void Set( const Vector4& axis, float angle );
	void Set(const Vector4& from, const Vector4& to);
	void Set(const Matrix44& rotation);
	void Set(const BasisR3& basis);

	//Ensures quaternion is a valid rotational quaternion
	void MakeValid();

    void GetAxisAngle(Vector4& axis, float& angle);

    void Clean();				// sets near-zero elements to 0
    inline void Zero();			// sets all elements to 0
    void Normalize();			// sets to unit quaternion
    inline void Identity();		// sets to identity quaternion

    // complex conjugate
    friend Quaternion Conjugate(const Quaternion&);
    const Quaternion& Conjugate();

    // invert quaternion
    friend Quaternion Inverse(const Quaternion&);
    const Quaternion& Inverse();

    // operators

    // addition/subtraction
    Quaternion operator+(const Quaternion&) const;
    Quaternion& operator+=(const Quaternion&);
    Quaternion operator-(const Quaternion&) const;
    Quaternion& operator-=(const Quaternion&);

    Quaternion operator-() const;

    // scalar multiplication
    friend Quaternion    operator*( float scalar, const Quaternion& vector );
    Quaternion&          operator*=(float);

    // quaternion concatenation
    Quaternion operator*(const Quaternion&) const;
    Quaternion& operator*=(const Quaternion&);

    // dot product
    friend float       Dot(const Quaternion&, const Quaternion&);

    // vector rotation
	Vector4   Rotate(const Vector4&) const;
	Point4    Rotate(const Point4&) const;
	void	  RotateSelf(Vector4&) const;
	void	  RotateSelf(Point4&) const;


    // interpolation
    friend void Lerp( Quaternion& result, const Quaternion& start, const Quaternion& end, float t );
    friend void Slerp( Quaternion& result, const Quaternion& start, const Quaternion& end, float t );
    friend void ApproxSlerp( Quaternion& result, const Quaternion& start, const Quaternion& end, float t );

    // useful defaults
    static Quaternion   zero;
    static Quaternion   identity;
        
private:
    // member variables
    float w, x, y, z;      

};


//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//	@	Quaternion::Set()
//-------------------------------------------------------------------------------
//		Set vector elements
//-------------------------------------------------------------------------------
inline void 
Quaternion::Set( float _w, float _x, float _y, float _z )
{
    w = _w; x = _x; y = _y; z = _z;

}   // End of Quaternion::Set()


//-------------------------------------------------------------------------------
//	@	Quaternion::Zero()
//-------------------------------------------------------------------------------
//		Zero all elements
//-------------------------------------------------------------------------------
inline void 
Quaternion::Zero()
{
    x = y = z = w = 0.0f;
}   // End of Quaternion::Zero()


//-------------------------------------------------------------------------------
//	@	Quaternion::Identity()
//-------------------------------------------------------------------------------
//		Set to identity quaternion
//-------------------------------------------------------------------------------
inline void 
Quaternion::Identity()
{
    x = y = z = 0.0f;
    w = 1.0f;
}   // End of Quaternion::Identity

#endif
