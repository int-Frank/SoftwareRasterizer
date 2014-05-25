//================================================================================
// @ Matrix44.cpp
// 
// Description: This file defines Matrix44's methods
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

#include "Matrix44.h"
#include "CommonMath.h"
#include "Vector4.h"
#include "BasisR3.h"
#include <assert.h>

#undef CUSTOM_MATRIX_ARITHMETIC


//--------------------------------------------------------------------------------
//	@	Matrix44::Matrix44()
//--------------------------------------------------------------------------------
//		Construct from VQS
//--------------------------------------------------------------------------------
Matrix44::Matrix44(const VQS& vqs)
{
	//Set rotational and scale part
	float xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    xs = vqs.q.x+vqs.q.x;   
    ys = vqs.q.y+vqs.q.y;
    zs = vqs.q.z+vqs.q.z;
    wx = vqs.q.w*xs;
    wy = vqs.q.w*ys;
    wz = vqs.q.w*zs;
    xx = vqs.q.x*xs;
    xy = vqs.q.x*ys;
    xz = vqs.q.x*zs;
    yy = vqs.q.y*ys;
    yz = vqs.q.y*zs;
    zz = vqs.q.z*zs;

    mV[0] = (1.0f - (yy + zz)) * vqs.s;
    mV[4] = (xy - wz) * vqs.s;
    mV[8] = (xz + wy) * vqs.s;
    
    mV[1] = (xy + wz) * vqs.s;
    mV[5] = (1.0f - (xx + zz)) * vqs.s;
    mV[9] = (yz - wx) * vqs.s;
    
    mV[2] = (xz - wy) * vqs.s;
    mV[6] = (yz + wx) * vqs.s;
    mV[10] = (1.0f - (xx + yy)) * vqs.s;

	mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

	//set translation part
	mV[12] = vqs.v.x;
	mV[13] = vqs.v.y;
	mV[14] = vqs.v.z;
	mV[15] = 1.0f;

}	//End: Matrix44::Matrix44()


//--------------------------------------------------------------------------------
//	@	Matrix44::Matrix44()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Matrix44::Matrix44(const Matrix44& other)
{
	mV[0] = other.mV[0];
	mV[1] = other.mV[1];
	mV[2] = other.mV[2];
	mV[3] = other.mV[3];
	mV[4] = other.mV[4];
	mV[5] = other.mV[5];
	mV[6] = other.mV[6];
	mV[7] = other.mV[7];
	mV[8] = other.mV[8];
	mV[9] = other.mV[9];
	mV[10] = other.mV[10];
	mV[11] = other.mV[11];
	mV[12] = other.mV[12];
	mV[13] = other.mV[13];
	mV[14] = other.mV[14];
	mV[15] = other.mV[15];

}	//End: Matrix44::Matrix44()


//--------------------------------------------------------------------------------
//	@	Matrix44::Matrix44()
//--------------------------------------------------------------------------------
//		Construct from basis vectors
//--------------------------------------------------------------------------------
Matrix44::Matrix44(const BasisR3& b)
{
	mV[0] = b.m_x2.x;
	mV[1] = b.m_x2.y;
	mV[2] = b.m_x2.z;
	mV[3] = 0.0f;
	mV[4] = b.m_x1.x;
	mV[5] = b.m_x1.y;
	mV[6] = b.m_x1.z;
	mV[7] = 0.0f;
	mV[8] = -b.m_x0.x;
	mV[9] = -b.m_x0.y;
	mV[10] = -b.m_x0.z;
	mV[11] = 0.0f;
	mV[12] = 0.0f;
	mV[13] = 0.0f;
	mV[14] = 0.0f;
	mV[15] = 1.0f;

}	//End: Matrix44::Matrix44()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Matrix44& Matrix44::operator= (const Matrix44& other)
{
	mV[0] = other.mV[0];
	mV[1] = other.mV[1];
	mV[2] = other.mV[2];
	mV[3] = other.mV[3];
	mV[4] = other.mV[4];
	mV[5] = other.mV[5];
	mV[6] = other.mV[6];
	mV[7] = other.mV[7];
	mV[8] = other.mV[8];
	mV[9] = other.mV[9];
	mV[10] = other.mV[10];
	mV[11] = other.mV[11];
	mV[12] = other.mV[12];
	mV[13] = other.mV[13];
	mV[14] = other.mV[14];
	mV[15] = other.mV[15];

	return *this;

}	//End: Matrix::operator=()


//--------------------------------------------------------------------------------
//	@	operator<<()
//-------------------------------------------------------------------------------
//		Text output for debugging
//-------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Matrix44& source)
{
    // row
    for (uint32 i = 0; i < 4; ++i)
    {
        out << "| ";
        // column
        for (uint32 j = 0; j < 4; ++j )
        {
            out << source.mV[ j*4 + i ] << ' ';
        }
        out << '|' << std::endl;
    }

    return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator==()
//--------------------------------------------------------------------------------
//		Comparison operator
//--------------------------------------------------------------------------------
bool Matrix44::operator== (const Matrix44& other) const
{
	for (uint32 i = 0; i < 16; i++)
	{
		if (!DgAreEqual(mV[i], other.mV[i]))
			return false;
	}

	return true;

}	//End: Matrix44::operator==()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator!=()
//--------------------------------------------------------------------------------
//		Comparison operator
//--------------------------------------------------------------------------------
bool Matrix44::operator!= (const Matrix44& other) const
{
	for (uint32 i = 0; i < 16; i++)
	{
		if (!DgAreEqual(mV[i], other.mV[i]))
			return true;
	}

	return false;

}	//End: Matrix44::operator!=()


//--------------------------------------------------------------------------------
//	@	Matrix44::IsZero()
//--------------------------------------------------------------------------------
//		Check for zero matrix
//--------------------------------------------------------------------------------
bool Matrix44::IsZero() const
{
	for (uint32 i = 0; i < 16; i++)
	{
		if (!::IsZero(mV[i]))
			return false;
	}

	return true;

}	//End: Matrix44::IsZero()


//--------------------------------------------------------------------------------
//	@	Matrix44::IsIdentity()
//--------------------------------------------------------------------------------
//		Check for identity
//--------------------------------------------------------------------------------
bool Matrix44::IsIdentity() const
{
	return ::DgAreEqual(1.0f, mV[0])
		&& ::DgAreEqual(1.0f, mV[5])
		&& ::DgAreEqual(1.0f, mV[10])
		&& ::DgAreEqual(1.0f, mV[15])
		&& ::IsZero(mV[1])
		&& ::IsZero(mV[2])
		&& ::IsZero(mV[3])
		&& ::IsZero(mV[4])
		&& ::IsZero(mV[6])
		&& ::IsZero(mV[7])
		&& ::IsZero(mV[8])
		&& ::IsZero(mV[9])
		&& ::IsZero(mV[11])
		&& ::IsZero(mV[12])
		&& ::IsZero(mV[13])
		&& ::IsZero(mV[14]);
		
}	//End: Matrix44::IsIdentity()


//--------------------------------------------------------------------------------
//	@	Matrix44::SetRows()
//--------------------------------------------------------------------------------
//		Set Matrix, Row by row
//--------------------------------------------------------------------------------
void Matrix44::SetRows(	const HPoint& row1, const HPoint& row2, 
						const HPoint& row3, const HPoint& row4)
{
	mV[0] = row1.x;
	mV[4] = row1.y;
	mV[8] = row1.z;
	mV[12] = row1.w;

	mV[1] = row2.x;
	mV[5] = row2.y;
	mV[9] = row2.z;
	mV[13] = row2.w;

	mV[2] = row3.x;
	mV[6] = row3.y;
	mV[10] = row3.z;
	mV[14] = row3.w;

	mV[3] = row4.x;
	mV[7] = row4.y;
	mV[11] = row4.z;
	mV[15] = row4.w;

}	//End: Matrix44::SetRows()


//--------------------------------------------------------------------------------
//	@	Matrix44::GetRows()
//--------------------------------------------------------------------------------
//		Get Matrix, row by row
//--------------------------------------------------------------------------------
void Matrix44::GetRows(	HPoint& row1, HPoint& row2, 
						HPoint& row3, HPoint& row4)
{
	row1.x = mV[0];
	row1.y = mV[4];
	row1.z = mV[8];
	row1.w = mV[12];

	row2.x = mV[1];
	row2.y = mV[5];
	row2.z = mV[9];
	row2.w = mV[13];

	row3.x = mV[2];
	row3.y = mV[6];
	row3.z = mV[10];
	row3.w = mV[14];

	row4.x = mV[3];
	row4.y = mV[7];
	row4.z = mV[11];
	row4.w = mV[15];

}	//End: Matrix44::GetRows()


//--------------------------------------------------------------------------------
//	@	Matrix44::SetColumns()
//--------------------------------------------------------------------------------
//		Set Matrix, Column by column
//--------------------------------------------------------------------------------
void Matrix44::SetColumns(	const HPoint& col1, const HPoint& col2, 
							const HPoint& col3, const HPoint& col4)
{
	mV[0] = col1.x;
	mV[1] = col1.y;
	mV[2] = col1.z;
	mV[3] = col1.w;

	mV[4] = col2.x;
	mV[5] = col2.y;
	mV[6] = col2.z;
	mV[7] = col2.w;

	mV[8] = col3.x;
	mV[9] = col3.y;
	mV[10] = col3.z;
	mV[11] = col3.w;

	mV[12] = col4.x;
	mV[13] = col4.y;
	mV[14] = col4.z;
	mV[15] = col4.w;

}	//End: Matrix44::SetColumns()


//--------------------------------------------------------------------------------
//	@	Matrix44::GetColumns()
//--------------------------------------------------------------------------------
//		Get Matrix, Column by column
//--------------------------------------------------------------------------------
void Matrix44::GetColumns(	HPoint& col1, HPoint& col2, 
							HPoint& col3, HPoint& col4)
{
	col1.x = mV[0];
	col1.y = mV[1];
	col1.z = mV[2];
	col1.w = mV[3];

	col2.x = mV[4];
	col2.y = mV[5];
	col2.z = mV[6];
	col2.w = mV[7];

	col3.x = mV[8];
	col3.y = mV[9];
	col3.z = mV[10];
	col3.w = mV[11];

	col4.x = mV[12];
	col4.y = mV[13];
	col4.z = mV[14];
	col4.w = mV[15];

}	//End: Matrix44::GetColumns()


//--------------------------------------------------------------------------------
//	@	Matrix44::SetRow()
//--------------------------------------------------------------------------------
//		Set a single Row
//--------------------------------------------------------------------------------
void Matrix44::SetRow(uint16 i, const HPoint& row)
{
	mV[i] = row.x;
	mV[i+4] = row.y;
	mV[i+8] = row.z;
	mV[i+12] = row.w;

}	//End: Matrix44::SetRow()


//--------------------------------------------------------------------------------
//	@	Matrix44::SetColumn()
//--------------------------------------------------------------------------------
//		Set a single column
//--------------------------------------------------------------------------------
void Matrix44::SetColumn(uint16 i, const HPoint& col)
{
	i *= 4;
	mV[i] = col.x;
	mV[i+1] = col.y;
	mV[i+2] = col.z;
	mV[i+3] = col.w;

}	//End: Matrix44::SetColumn()


//--------------------------------------------------------------------------------
//	@	Matrix44::Set()
//--------------------------------------------------------------------------------
//		Set a rotation matrix from an orthonormal basis
//--------------------------------------------------------------------------------
void Matrix44::Set(const BasisR3& b)
{
	SetColumns(b.x2(), b.x1(), -b.x0(), Point4::origin);

}	//End: Matrix44::Set()


//--------------------------------------------------------------------------------
//	@	Matrix44::Clean()
//--------------------------------------------------------------------------------
//		Set elements close to zero equal to zero
//--------------------------------------------------------------------------------
void Matrix44::Clean()
{
	for (uint32 i = 0; i < 16; i++)
	{
		if (::IsZero(mV[i]))
			mV[i] = 0.0f;
	}

}	//End: Matrix44::Clean()


//--------------------------------------------------------------------------------
//	@	Matrix44::Indentity()
//--------------------------------------------------------------------------------
//		Set to identity matrix
//--------------------------------------------------------------------------------
void Matrix44::Identity()
{
	mV[0] = 1.0f;
	mV[1] = 0.0f;
	mV[2] = 0.0f;
	mV[3] = 0.0f;
	mV[4] = 0.0f;
	mV[5] = 1.0f;
	mV[6] = 0.0f;
	mV[7] = 0.0f;
	mV[8] = 0.0f;
	mV[9] = 0.0f;
	mV[10] = 1.0f;
	mV[11] = 0.0f;
	mV[12] = 0.0f;
	mV[13] = 0.0f;
	mV[14] = 0.0f;
	mV[15] = 1.0f;

}	//End: Matrix44::Identity()


//--------------------------------------------------------------------------------
//	@	Matrix44::AffineInverse()
//--------------------------------------------------------------------------------
//		Set self to matrix inverse, assuming a standard affine matrix
//		(bottom row is 0 0 0 1)
//--------------------------------------------------------------------------------
Matrix44& Matrix44::AffineInverse()
{
	*this = ::AffineInverse(*this);

	return *this;

}	//End: Matrix44::AffineInverse


//--------------------------------------------------------------------------------
//	@	AffineInverse()
//--------------------------------------------------------------------------------
//		Compute matrix inverse, assuming a standard affine matrix
//		(bottom row is 0 0 0 1)
//--------------------------------------------------------------------------------
Matrix44 AffineInverse(const Matrix44& mat)
{
	Matrix44 result;

	//compute upper left 3x3 matrix determinant
	float cofactor0 = mat.mV[5]*mat.mV[10] - mat.mV[6]*mat.mV[9];
	float cofactor4 = mat.mV[2]*mat.mV[9] - mat.mV[1]*mat.mV[10];
	float cofactor8 = mat.mV[5]*mat.mV[10] - mat.mV[6]*mat.mV[9];
	float det = mat.mV[0]*cofactor0 + mat.mV[4]*cofactor4 + mat.mV[8]*cofactor8;
    if (::IsZero( det ))
    {
        std::cerr << "@Matrix44::AffintInverse() -> No Inverse exists." << std::endl;
        return result;
    }

    // create adjunct matrix and multiply by 1/det to get upper 3x3
    float invDet = 1.0f/det;
    result.mV[0] = invDet*cofactor0;
    result.mV[1] = invDet*cofactor4;
    result.mV[2] = invDet*cofactor8;
   
    result.mV[4] = invDet*(mat.mV[6]*mat.mV[8] - mat.mV[4]*mat.mV[10]);
    result.mV[5] = invDet*(mat.mV[0]*mat.mV[10] - mat.mV[2]*mat.mV[8]);
    result.mV[6] = invDet*(mat.mV[2]*mat.mV[4] - mat.mV[0]*mat.mV[6]);

    result.mV[8] = invDet*(mat.mV[4]*mat.mV[9] - mat.mV[5]*mat.mV[8]);
    result.mV[9] = invDet*(mat.mV[1]*mat.mV[8] - mat.mV[0]*mat.mV[9]);
    result.mV[10] = invDet*(mat.mV[0]*mat.mV[5] - mat.mV[1]*mat.mV[4]);

    // multiply -translation by inverted 3x3 to get its inverse
    
    result.mV[12] = -result.mV[0]*mat.mV[12] - result.mV[4]*mat.mV[13] - result.mV[8]*mat.mV[14];
    result.mV[13] = -result.mV[1]*mat.mV[12] - result.mV[5]*mat.mV[13] - result.mV[9]*mat.mV[14];
    result.mV[14] = -result.mV[2]*mat.mV[12] - result.mV[6]*mat.mV[13] - result.mV[10]*mat.mV[14];

    return result;

}	//End: ::AffineInverse()


//--------------------------------------------------------------------------------
//	@	Matrix44::Transpose()
//-------------------------------------------------------------------------------
//		Set self to transpose
//-------------------------------------------------------------------------------
Matrix44& Matrix44::Transpose()
{
    float temp = mV[1];
    mV[1] = mV[4];
    mV[4] = temp;

    temp = mV[2];
    mV[2] = mV[8];
    mV[8] = temp;

    temp = mV[3];
    mV[2] = mV[12];
    mV[12] = temp;

    temp = mV[6];
    mV[6] = mV[9];
    mV[9] = temp;

    temp = mV[7];
    mV[7] = mV[13];
    mV[13] = temp;

    temp = mV[11];
    mV[11] = mV[14];
    mV[14] = temp;

    return *this;

}   // End: Matrix44::Transpose()


//--------------------------------------------------------------------------------
//	@	Transpose()
//-------------------------------------------------------------------------------
//		Compute matrix transpose
//-------------------------------------------------------------------------------
Matrix44 Transpose( const Matrix44& mat )
{
    Matrix44 result;

    result.mV[0] = mat.mV[0];
    result.mV[1] = mat.mV[4];
    result.mV[2] = mat.mV[8];
    result.mV[3] = mat.mV[12];
    result.mV[4] = mat.mV[1];
    result.mV[5] = mat.mV[5];
    result.mV[6] = mat.mV[9];
    result.mV[7] = mat.mV[13];
    result.mV[8] = mat.mV[2];
    result.mV[9] = mat.mV[6];
    result.mV[10] = mat.mV[10];
    result.mV[11] = mat.mV[14];
    result.mV[12] = mat.mV[3];
    result.mV[13] = mat.mV[7];
    result.mV[14] = mat.mV[11];
    result.mV[15] = mat.mV[15];

    return result;

}   // End: Transpose()


//--------------------------------------------------------------------------------
//	@	Matrix44::Translation()
//-------------------------------------------------------------------------------
//		Set as translation matrix based on vector
//-------------------------------------------------------------------------------
Matrix44& Matrix44::Translation( const HPoint& xlate )
{
    mV[0] = 1.0f;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = 1.0f;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = 1.0f;
    mV[11] = 0.0f;
    mV[12] = xlate.x;
    mV[13] = xlate.y;
    mV[14] = xlate.z;
    mV[15] = 1.0f;

    return *this;

}   // End: Matrix44::Translation()


//--------------------------------------------------------------------------------
//	@	Matrix44::Rotation()
//----------------------------------------------------------------------------
//		Sets the matrix to a rotation matrix (by Euler angles).
//----------------------------------------------------------------------------
Matrix44& Matrix44::Rotation( float zRotation, float yRotation, float xRotation )
{
    // This is an "unrolled" contatenation of rotation matrices X Y & Z
    float Cx, Sx;
    DgSinCos(xRotation, Sx, Cx);

    float Cy, Sy;
    DgSinCos(yRotation, Sy, Cy);

    float Cz, Sz;
    DgSinCos(zRotation, Sz, Cz);

    mV[0] =  (Cy * Cz);
    mV[4] = -(Cy * Sz);  
    mV[8] =  Sy;
    mV[12] = 0.0f;

    mV[1] =  (Sx * Sy * Cz) + (Cx * Sz);
    mV[5] = -(Sx * Sy * Sz) + (Cx * Cz);
    mV[9] = -(Sx * Cy); 
    mV[13] = 0.0f;

    mV[2] = -(Cx * Sy * Cz) + (Sx * Sz);
    mV[6] =  (Cx * Sy * Sz) + (Sx * Cz);
    mV[10] =  (Cx * Cy);
    mV[14] = 0.0f;

    mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}  // End: Matrix44::Rotation()


//--------------------------------------------------------------------------------
//	@	Matrix44::Rotation()
//----------------------------------------------------------------------------
//		Sets the matrix to a rotation matrix (by axis and angle).
//----------------------------------------------------------------------------
Matrix44& Matrix44::Rotation( const Vector4& axis, float angle )
{
    float c, s;
    DgSinCos(angle, s, c);
    float t = 1.0f - c;

    Vector4 nAxis = axis;
	nAxis.w = 0.0f;
    nAxis.Normalize();

    // intermediate values
    float tx = t*nAxis.x;  float ty = t*nAxis.y;  float tz = t*nAxis.z;
    float sx = s*nAxis.x;  float sy = s*nAxis.y;  float sz = s*nAxis.z;
    float txy = tx*nAxis.y; float tyz = tx*nAxis.z; float txz = tx*nAxis.z;

    // set matrix
    mV[0] = tx*nAxis.x + c;
    mV[4] = txy - sz; 
    mV[8] = txz + sy;
    mV[12] = 0.0f;

    mV[1] = txy + sz;
    mV[5] = ty*nAxis.y + c;
    mV[9] = tyz - sx;
    mV[13] = 0.0f;

    mV[2] = txz - sy;
    mV[6] = tyz + sx;
    mV[10] = tz*nAxis.z + c;
    mV[14] = 0.0f;

    mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}  // End: Matrix44::Rotation()


//--------------------------------------------------------------------------------
//	@	Matrix44::Rotation()
//-------------------------------------------------------------------------------
//		Set as rotation matrix based on quaternion
//-------------------------------------------------------------------------------
Matrix44& Matrix44::Rotation( const Quaternion& rotate )
{
    assert( rotate.IsUnit() );

    float xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    xs = rotate.x+rotate.x;   
    ys = rotate.y+rotate.y;
    zs = rotate.z+rotate.z;
    wx = rotate.w*xs;
    wy = rotate.w*ys;
    wz = rotate.w*zs;
    xx = rotate.x*xs;
    xy = rotate.x*ys;
    xz = rotate.x*zs;
    yy = rotate.y*ys;
    yz = rotate.y*zs;
    zz = rotate.z*zs;

    mV[0] = 1.0f - (yy + zz);
    mV[4] = xy - wz;
    mV[8] = xz + wy;
    mV[12] = 0.0f;
    
    mV[1] = xy + wz;
    mV[5] = 1.0f - (xx + zz);
    mV[9] = yz - wx;
    mV[13] = 0.0f;
    
    mV[2] = xz - wy;
    mV[6] = yz + wx;
    mV[10] = 1.0f - (xx + yy);
    mV[14] = 0.0f;

    mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End of Matrix44::Rotation()


//--------------------------------------------------------------------------------
//	@	Matrix44::Scaling()
//-------------------------------------------------------------------------------
//		Set as scaling matrix based on vector
//-------------------------------------------------------------------------------
Matrix44&  Matrix44::Scaling( const HPoint& scaleFactors )
{
    mV[0] = scaleFactors.x;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = scaleFactors.y;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = scaleFactors.z;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End: Matrix44::Scaling()


//--------------------------------------------------------------------------------
//	@	Matrix44::Scaling()
//-------------------------------------------------------------------------------
//		Uniform scaling
//-------------------------------------------------------------------------------
Matrix44&  Matrix44::Scaling( float val )
{
    mV[0] = val;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = val;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = val;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End: Matrix44::Scaling()


//--------------------------------------------------------------------------------
//	@	Matrix44::RotationX()
//-------------------------------------------------------------------------------
//		Set as rotation matrix, rotating by 'angle' radians around x-axis
//-------------------------------------------------------------------------------
Matrix44& Matrix44::RotationX( float angle )
{
    float sintheta, costheta;
    DgSinCos(angle, sintheta, costheta);

    mV[0] = 1.0f;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = costheta;
    mV[6] = sintheta;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = -sintheta;
    mV[10] = costheta;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End: Matrix44::RotationX()


//--------------------------------------------------------------------------------
//	@	Matrix44::RotationY()
//-------------------------------------------------------------------------------
//		Set as rotation matrix, rotating by 'angle' radians around y-axis
//-------------------------------------------------------------------------------
Matrix44& Matrix44::RotationY( float angle )
{
    float sintheta, costheta;
    DgSinCos(angle, sintheta, costheta);

    mV[0] = costheta;
    mV[1] = 0.0f;
    mV[2] = -sintheta;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = 1.0f;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = sintheta;
    mV[9] = 0.0f;
    mV[10] = costheta;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;    

    return *this;

}   // End: Matrix44::RotationY()


//--------------------------------------------------------------------------------
//	@	Matrix44::RotationZ()
//-------------------------------------------------------------------------------
//		Set as rotation matrix, rotating by 'angle' radians around z-axis
//-------------------------------------------------------------------------------
Matrix44& Matrix44::RotationZ( float angle )
{
    float sintheta, costheta;
    DgSinCos(angle, sintheta, costheta);

    mV[0] = costheta;
    mV[1] = sintheta;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = -sintheta;
    mV[5] = costheta;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = 1.0f;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End: Matrix44::RotationZ()


//--------------------------------------------------------------------------------
//	@	Matrix44::GetFixedAngles()
// ---------------------------------------------------------------------------
//		Gets one set of possible z-y-x fixed angles that will generate this 
//		matrix. Assumes that upper 3x3 is a rotation matrix
//----------------------------------------------------------------------------
void Matrix44::GetFixedAngles( float& zRotation, float& yRotation, float& xRotation )
{
    float Cx, Sx;
    float Cy, Sy;
    float Cz, Sz;

    Sy = mV[8];
    Cy = ::DgSqrt( 1.0f - Sy*Sy );
    // normal case
    if ( !::IsZero( Cy ) )
    {
        float factor = 1.0f/Cy;
        Sx = -mV[9]*factor;
        Cx = mV[10]*factor;
        Sz = -mV[4]*factor;
        Cz = mV[0]*factor;
    }
    // x and z axes aligned
    else
    {
        Sz = 0.0f;
        Cz = 1.0f;
        Sx = mV[6];
        Cx = mV[5];
    }

    zRotation = DgATan2( Sz, Cz );
    yRotation = DgATan2( Sy, Cy );
    xRotation = DgATan2( Sx, Cx );

}  // End: Matrix44::GetFixedAngles()


//--------------------------------------------------------------------------------
//	@	Matrix44::GetAxisAngle()
// ---------------------------------------------------------------------------
//		Gets one possible axis-angle pair that will generate this matrix
//		Assumes that upper 3x3 is a rotation matrix
//----------------------------------------------------------------------------
void Matrix44::GetAxisAngle( Vector4& axis, float& angle )
{
    float trace = mV[0] + mV[5] + mV[10];
    float cosTheta = 0.5f*(trace - 1.0f);
    angle = acosf( cosTheta );

    // angle is zero, axis can be anything
    if ( ::IsZero( angle ) )
    {
        axis = Vector4::xAxis;
    }
    // standard case
    else if ( angle < PI-EPSILON )
    {
        axis.Set( mV[6]-mV[9], mV[8]-mV[2], mV[1]-mV[4] );
        axis.Normalize();
    }
    // angle is 180 degrees
    else
    {
        uint32 i = 0;
        if ( mV[5] > mV[0] )
            i = 1;
        if ( mV[10] > mV[i + 4*i] )
            i = 2;
        uint32 j = (i+1)%3;
        uint32 k = (j+1)%3;
        float s = ::DgSqrt( mV[i + 4*i] - mV[j + 4*j] - mV[k + 4*k] + 1.0f );
        axis[i] = 0.5f*s;
        float recip = 1.0f/s;
        axis[j] = (mV[i + 4*j])*recip;
        axis[k] = (mV[k + 4*i])*recip;
    }

}  // End: Matrix44::GetAxisAngle()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator+()
//-------------------------------------------------------------------------------
//		Matrix addition 
//-------------------------------------------------------------------------------
Matrix44 Matrix44::operator+( const Matrix44& other ) const
{
    Matrix44 result;

    for (uint32 i = 0; i < 16; ++i)
    {
        result.mV[i] = mV[i] + other.mV[i];
    }

    return result;

}   // End: Matrix44::operator+()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator+=()
//-------------------------------------------------------------------------------
//		Matrix addition by self
//-------------------------------------------------------------------------------
Matrix44& Matrix44::operator+=( const Matrix44& other )
{
    for (uint32 i = 0; i < 16; ++i)
    {
        mV[i] += other.mV[i];
    }

    return *this;

}   // End: Matrix44::operator+=()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator-()
//-------------------------------------------------------------------------------
//		Matrix subtraction 
//-------------------------------------------------------------------------------
Matrix44 Matrix44::operator-( const Matrix44& other ) const
{
    Matrix44 result;

    for (uint32 i = 0; i < 16; ++i)
    {
        result.mV[i] = mV[i] - other.mV[i];
    }

    return result;

}   // End: Matrix44::operator-()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator-=()
//-------------------------------------------------------------------------------
//		Matrix subtraction by self
//-------------------------------------------------------------------------------
Matrix44& Matrix44::operator-=( const Matrix44& other )
{
    for (uint32 i = 0; i < 16; ++i)
    {
        mV[i] -= other.mV[i];
    }

    return *this;

}   // End: Matrix44::operator-=()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator-()
//-------------------------------------------------------------------------------
//		Negate self and return
//-------------------------------------------------------------------------------
Matrix44 Matrix44::operator-() const
{
    Matrix44 result;

    for (uint32 i = 0; i < 16; ++i)
    {
        result.mV[i] = -mV[i];
    }

    return result;

}    // End: Matrix44::operator-()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix multiplication
//-------------------------------------------------------------------------------
Matrix44 Matrix44::operator*( const Matrix44& other ) const
{
    Matrix44 result;

    result.mV[0] = mV[0]*other.mV[0] + mV[4]*other.mV[1] + mV[8]*other.mV[2] 
                    + mV[12]*other.mV[3];
    result.mV[1] = mV[1]*other.mV[0] + mV[5]*other.mV[1] + mV[9]*other.mV[2] 
                    + mV[13]*other.mV[3];
    result.mV[2] = mV[2]*other.mV[0] + mV[6]*other.mV[1] + mV[10]*other.mV[2] 
                    + mV[14]*other.mV[3];
    result.mV[3] = mV[3]*other.mV[0] + mV[7]*other.mV[1] + mV[11]*other.mV[2] 
                    + mV[15]*other.mV[3];

    result.mV[4] = mV[0]*other.mV[4] + mV[4]*other.mV[5] + mV[8]*other.mV[6] 
                    + mV[12]*other.mV[7];
    result.mV[5] = mV[1]*other.mV[4] + mV[5]*other.mV[5] + mV[9]*other.mV[6] 
                    + mV[13]*other.mV[7];
    result.mV[6] = mV[2]*other.mV[4] + mV[6]*other.mV[5] + mV[10]*other.mV[6] 
                    + mV[14]*other.mV[7];
    result.mV[7] = mV[3]*other.mV[4] + mV[7]*other.mV[5] + mV[11]*other.mV[6] 
                    + mV[15]*other.mV[7];

    result.mV[8] = mV[0]*other.mV[8] + mV[4]*other.mV[9] + mV[8]*other.mV[10] 
                    + mV[12]*other.mV[11];
    result.mV[9] = mV[1]*other.mV[8] + mV[5]*other.mV[9] + mV[9]*other.mV[10] 
                    + mV[13]*other.mV[11];
    result.mV[10] = mV[2]*other.mV[8] + mV[6]*other.mV[9] + mV[10]*other.mV[10] 
                    + mV[14]*other.mV[11];
    result.mV[11] = mV[3]*other.mV[8] + mV[7]*other.mV[9] + mV[11]*other.mV[10] 
                    + mV[15]*other.mV[11];

    result.mV[12] = mV[0]*other.mV[12] + mV[4]*other.mV[13] + mV[8]*other.mV[14] 
                    + mV[12]*other.mV[15];
    result.mV[13] = mV[1]*other.mV[12] + mV[5]*other.mV[13] + mV[9]*other.mV[14] 
                    + mV[13]*other.mV[15];
    result.mV[14] = mV[2]*other.mV[12] + mV[6]*other.mV[13] + mV[10]*other.mV[14] 
                    + mV[14]*other.mV[15];
    result.mV[15] = mV[3]*other.mV[12] + mV[7]*other.mV[13] + mV[11]*other.mV[14] 
                    + mV[15]*other.mV[15];

    return result;

}   // End: Matrix44::operator*()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*=()
//-------------------------------------------------------------------------------
//		Matrix multiplication by self
//-------------------------------------------------------------------------------
Matrix44& Matrix44::operator*=( const Matrix44& other )
{
    Matrix44 result;

    result.mV[0] = mV[0]*other.mV[0] + mV[4]*other.mV[1] + mV[8]*other.mV[2] 
                    + mV[12]*other.mV[3];
    result.mV[1] = mV[1]*other.mV[0] + mV[5]*other.mV[1] + mV[9]*other.mV[2] 
                    + mV[13]*other.mV[3];
    result.mV[2] = mV[2]*other.mV[0] + mV[6]*other.mV[1] + mV[10]*other.mV[2] 
                    + mV[14]*other.mV[3];
    result.mV[3] = mV[3]*other.mV[0] + mV[7]*other.mV[1] + mV[11]*other.mV[2] 
                    + mV[15]*other.mV[3];

    result.mV[4] = mV[0]*other.mV[4] + mV[4]*other.mV[5] + mV[8]*other.mV[6] 
                    + mV[12]*other.mV[7];
    result.mV[5] = mV[1]*other.mV[4] + mV[5]*other.mV[5] + mV[9]*other.mV[6] 
                    + mV[13]*other.mV[7];
    result.mV[6] = mV[2]*other.mV[4] + mV[6]*other.mV[5] + mV[10]*other.mV[6] 
                    + mV[14]*other.mV[7];
    result.mV[7] = mV[3]*other.mV[4] + mV[7]*other.mV[5] + mV[11]*other.mV[6] 
                    + mV[15]*other.mV[7];

    result.mV[8] = mV[0]*other.mV[8] + mV[4]*other.mV[9] + mV[8]*other.mV[10] 
                    + mV[12]*other.mV[11];
    result.mV[9] = mV[1]*other.mV[8] + mV[5]*other.mV[9] + mV[9]*other.mV[10] 
                    + mV[13]*other.mV[11];
    result.mV[10] = mV[2]*other.mV[8] + mV[6]*other.mV[9] + mV[10]*other.mV[10] 
                    + mV[14]*other.mV[11];
    result.mV[11] = mV[3]*other.mV[8] + mV[7]*other.mV[9] + mV[11]*other.mV[10] 
                    + mV[15]*other.mV[11];

    result.mV[12] = mV[0]*other.mV[12] + mV[4]*other.mV[13] + mV[8]*other.mV[14] 
                    + mV[12]*other.mV[15];
    result.mV[13] = mV[1]*other.mV[12] + mV[5]*other.mV[13] + mV[9]*other.mV[14] 
                    + mV[13]*other.mV[15];
    result.mV[14] = mV[2]*other.mV[12] + mV[6]*other.mV[13] + mV[10]*other.mV[14] 
                    + mV[14]*other.mV[15];
    result.mV[15] = mV[3]*other.mV[12] + mV[7]*other.mV[13] + mV[11]*other.mV[14] 
                    + mV[15]*other.mV[15];

    for (uint32 i = 0; i < 16; ++i)
    {
        mV[i] = result.mV[i];
    }

    return *this;

}   // End: Matrix44::operator*=()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix-column vector multiplication
//-------------------------------------------------------------------------------
Vector4 Matrix44::operator*( const Vector4& other ) const
{
    Vector4 result;
	
#ifdef CUSTOM_MATRIX_ARITHMETIC
	result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z;
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z;
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z;
#else
    result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z + mV[12]*other.w;
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z + mV[13]*other.w;
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z + mV[14]*other.w;
    result.w = mV[3]*other.x + mV[7]*other.y + mV[11]*other.z + mV[15]*other.w;
#endif

    return result;

}   // End: Matrix44::operator*()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix-column vector multiplication
//-------------------------------------------------------------------------------
Point4 Matrix44::operator*( const Point4& other ) const
{
    Point4 result;
	
#ifdef CUSTOM_MATRIX_ARITHMETIC
	result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z + mV[12];
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z + mV[13];
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z + mV[14];
#else
    result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z + mV[12]*other.w;
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z + mV[13]*other.w;
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z + mV[14]*other.w;
    result.w = mV[3]*other.x + mV[7]*other.y + mV[11]*other.z + mV[15]*other.w;
#endif

    return result;

}   // End: Matrix44::operator*()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix-row vector multiplication
//-------------------------------------------------------------------------------
Vector4 operator*( const Vector4& vector, const Matrix44& matrix )
{
    Vector4 result;

#ifdef CUSTOM_MATRIX_ARITHMETIC
	result.x = matrix.mV[0]*vector.x + matrix.mV[1]*vector.y 
             + matrix.mV[2]*vector.z;
    result.y = matrix.mV[4]*vector.x + matrix.mV[5]*vector.y 
             + matrix.mV[6]*vector.z;
    result.z = matrix.mV[8]*vector.x + matrix.mV[9]*vector.y 
             + matrix.mV[10]*vector.z;
#else
	result.x = matrix.mV[0]*vector.x + matrix.mV[1]*vector.y 
             + matrix.mV[2]*vector.z + matrix.mV[3]*vector.w;
    result.y = matrix.mV[4]*vector.x + matrix.mV[5]*vector.y 
             + matrix.mV[6]*vector.z + matrix.mV[7]*vector.w;
    result.z = matrix.mV[8]*vector.x + matrix.mV[9]*vector.y 
             + matrix.mV[10]*vector.z + matrix.mV[11]*vector.w;
	result.w = matrix.mV[12]*vector.x + matrix.mV[13]*vector.y 
            + matrix.mV[14]*vector.z + matrix.mV[15]*vector.w;
#endif

    return result;

}   // End: Matrix44::operator*()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix-row vector multiplication
//-------------------------------------------------------------------------------
Point4 operator*( const Point4& vector, const Matrix44& matrix )
{
    Point4 result;
	
#ifdef CUSTOM_MATRIX_ARITHMETIC
	result.x = matrix.mV[0]*vector.x + matrix.mV[1]*vector.y 
             + matrix.mV[2]*vector.z + matrix.mV[3];
    result.y = matrix.mV[4]*vector.x + matrix.mV[5]*vector.y 
             + matrix.mV[6]*vector.z + matrix.mV[7];
    result.z = matrix.mV[8]*vector.x + matrix.mV[9]*vector.y 
             + matrix.mV[10]*vector.z + matrix.mV[11];
#else
    result.x = matrix.mV[0]*vector.x + matrix.mV[1]*vector.y 
             + matrix.mV[2]*vector.z + matrix.mV[3]*vector.w;
    result.y = matrix.mV[4]*vector.x + matrix.mV[5]*vector.y 
             + matrix.mV[6]*vector.z + matrix.mV[7]*vector.w;
    result.z = matrix.mV[8]*vector.x + matrix.mV[9]*vector.y 
             + matrix.mV[10]*vector.z + matrix.mV[11]*vector.w;
	result.w = matrix.mV[12]*vector.x + matrix.mV[13]*vector.y 
            + matrix.mV[14]*vector.z + matrix.mV[15]*vector.w;
#endif

    return result;

}   // End: Matrix44::operator*()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*=()
//-------------------------------------------------------------------------------
//		Matrix-scalar multiplication
//-------------------------------------------------------------------------------
Matrix44& Matrix44::operator*=( float scalar )
{
    mV[0] *= scalar;
    mV[1] *= scalar;
    mV[2] *= scalar;
    mV[3] *= scalar;
    mV[4] *= scalar;
    mV[5] *= scalar;
    mV[6] *= scalar;
    mV[7] *= scalar;
    mV[8] *= scalar;
    mV[9] *= scalar;
    mV[10] *= scalar;
    mV[11] *= scalar;
    mV[12] *= scalar;
    mV[13] *= scalar;
    mV[14] *= scalar;
    mV[15] *= scalar;

    return *this;

}  // End: Matrix44::operator*=()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix-scalar multiplication
//-------------------------------------------------------------------------------
Matrix44 operator*( float scalar, const Matrix44& matrix )
{
    Matrix44 result;
    result.mV[0] = matrix.mV[0] * scalar;
    result.mV[1] = matrix.mV[1] * scalar;
    result.mV[2] = matrix.mV[2] * scalar;
    result.mV[3] = matrix.mV[3] * scalar;
    result.mV[4] = matrix.mV[4] * scalar;
    result.mV[5] = matrix.mV[5] * scalar;
    result.mV[6] = matrix.mV[6] * scalar;
    result.mV[7] = matrix.mV[7] * scalar;
    result.mV[8] = matrix.mV[8] * scalar;
    result.mV[9] = matrix.mV[9] * scalar;
    result.mV[10] = matrix.mV[10] * scalar;
    result.mV[11] = matrix.mV[11] * scalar;
    result.mV[12] = matrix.mV[12] * scalar;
    result.mV[13] = matrix.mV[13] * scalar;
    result.mV[14] = matrix.mV[14] * scalar;
    result.mV[15] = matrix.mV[15] * scalar;

    return result;

}  // End: operator*()


//--------------------------------------------------------------------------------
//	@	Matrix44::operator*()
//-------------------------------------------------------------------------------
//		Matrix-scalar multiplication
//-------------------------------------------------------------------------------
Matrix44 Matrix44::operator*( float scalar ) const
{
    Matrix44 result;
    result.mV[0] = mV[0] * scalar;
    result.mV[1] = mV[1] * scalar;
    result.mV[2] = mV[2] * scalar;
    result.mV[3] = mV[3] * scalar;
    result.mV[4] = mV[4] * scalar;
    result.mV[5] = mV[5] * scalar;
    result.mV[6] = mV[6] * scalar;
    result.mV[7] = mV[7] * scalar;
    result.mV[8] = mV[8] * scalar;
    result.mV[9] = mV[9] * scalar;
    result.mV[10] = mV[10] * scalar;
    result.mV[11] = mV[11] * scalar;
    result.mV[12] = mV[12] * scalar;
    result.mV[13] = mV[13] * scalar;
    result.mV[14] = mV[14] * scalar;
    result.mV[15] = mV[15] * scalar;

    return result;

}  // End: Matrix44::operator*()