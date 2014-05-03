/*!
* @file Tuple.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: Tuple
*/

#ifndef TUPLE_H
#define TUPLE_H

#include "DgTypes.h"
#include "Dg_io.h"
#include "DgError.h"
#include "CommonMath.h"

//--------------------------------------------------------------------------------
//		Declare template friend functions
//--------------------------------------------------------------------------------
template<class T> class Tuple;

//! Output
template<class T> DgWriter& operator<<(DgWriter& out, const Tuple<T>& source);
//! Input
template<class T> DgReader& operator>> (DgReader& in, Tuple<T>& dest);

/*!
 * @ingroup utility_container
 *
 * @class Tuple
 *
 * @brief Templated 3 tuple.
 *
 * A 3-tuple. Similar to a Vector3 but more general with a focus on
 * element-wise arithmetic operations.
 *
 * @author Frank Hart
 * @date 2/01/2014
 */
template<class T>
class Tuple
{
public:
	//Constructor/Destructor
	Tuple() {}
	Tuple(T a, T b, T c) {mV[0]=a; mV[1]=b; mV[2]=c; }
	~Tuple() {}

	//Copy operations
	Tuple(const Tuple&);
	Tuple& operator=(const Tuple&);

	//! Access by index
	T& operator[](uint8 a)		{ return mV[a]; }
	//! Access by index
	const T& operator[](uint8 a) const {return mV[a];}

	//Input/output
	friend DgWriter& operator<< <>(DgWriter& out, const Tuple<T>& source);
	friend DgReader& operator>> <>(DgReader& in, Tuple<T>& dest);

	//! Set all elements to t
	inline void Set(T t) {mV[0] = mV[1] = mV[2] = t;}
	inline void Set(T a, T b, T c);

	//Min/Max element
	T Min() const;
	T Max() const;
	
	//! Element-wise addition
	Tuple operator+ (const Tuple&) const;

	//! Element-wise subtraction
	Tuple operator- (const Tuple&) const;

	//! Element-wise multiplication
	Tuple operator* (const Tuple&) const;

	//! Element-wise division
	Tuple operator/ (const Tuple&) const;


	//! Addition with single value
	Tuple operator+ (T) const;

	//! Subtraction with single value
	Tuple operator- (T) const;

	//! Multiplication with single value
	Tuple operator* (T) const;

	//! Division with single value
	Tuple operator/ (T) const;


	//! Element-wise addition update
	Tuple& operator+= (const Tuple& rhs);

	//! Element-wise subtraction update
	Tuple& operator-= (const Tuple& rhs);

	//! Element-wise multiplication update
	Tuple& operator*= (const Tuple& rhs);

	//! Element-wise division update
	Tuple& operator/= (const Tuple& rhs);


	//! Addition update with single value
	Tuple& operator+= (T);

	//! Subtraction update with single value
	Tuple& operator-= (T);

	//! Multiplication update with single value
	Tuple& operator*= (T);

	//! Division update with single value
	Tuple& operator/= (T);

private:
	//Data members
	T mV[3];

};

//--------------------------------------------------------------------------------
//	@	Tuple<T>::Tuple()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>::Tuple(const Tuple<T>& t)
{
	mV[0] = t.mV[0];
	mV[1] = t.mV[1];
	mV[2] = t.mV[2];

}	//End: Tuple::Tuple()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Input
//--------------------------------------------------------------------------------
template <class T>
DgReader& operator>> (DgReader& in, Tuple<T>& dest)
{
	in >> dest.mV[0] >> dest.mV[1] >> dest.mV[2];

	if (!in)
	{
		ERROR_EXIT("@operator>>(Tuple) -> Bad read.");
	}

	return in;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
template <class T>
DgWriter& operator<<(DgWriter& out, const Tuple<T>& source)
{
	out << '<' << source.mV[0] << ' ' 
		<< source.mV[1] << ' ' 
		<< source.mV[2] << '>';

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator=(const Tuple<T>& t)
{
	if (this == &t)
		return *this;

	mV[0] = t.mV[0];
	mV[1] = t.mV[1];
	mV[2] = t.mV[2];

	return *this;

}	//End: Tuple::Tuple()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::Set()
//--------------------------------------------------------------------------------
//		Set elements
//--------------------------------------------------------------------------------
template <class T>
void Tuple<T>::Set(T a, T b, T c)
{
	mV[0] = a;
	mV[1] = b;
	mV[2] = c;

}	//End: Tuple<T>::Set()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::Max()
//--------------------------------------------------------------------------------
//		Returns maximum element
//--------------------------------------------------------------------------------
template <class T>
T Tuple<T>::Max() const
{
	uint8 i = 0;

	if (mV[1] > mV[0])
		i = 1;
	if (mV[2] > mV[i])
		i = 2;

	return mV[i];

}	//End: Tuple<T>::Max()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::Min()
//--------------------------------------------------------------------------------
//		Returns mainimum element
//--------------------------------------------------------------------------------
template <class T>
T Tuple<T>::Min() const
{
	uint8 i = 0;

	if (mV[1] < mV[0])
		i = 1;
	if (mV[2] < mV[i])
		i = 2;

	return mV[i];

}	//End: Tuple<T>::Min()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator+()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Addition
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator+(const Tuple<T>& t) const
{
	return Tuple<T>(mV[0] + t.mV[0], 
					mV[1] + t.mV[1], 
					mV[2] + t.mV[2]);

}	//End: Tuple<T>::operator+()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator-()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Subtraction
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator-(const Tuple<T>& t) const
{
	return Tuple<T>(mV[0] - t.mV[0], 
					mV[1] - t.mV[1], 
					mV[2] - t.mV[2]);

}	//End: Tuple<T>::operator-()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator*()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Multiplication
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator*(const Tuple<T>& t) const
{
	return Tuple<T>(mV[0] * t.mV[0], 
					mV[1] * t.mV[1], 
					mV[2] * t.mV[2]);

}	//End: Tuple<T>::operator*()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator/()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Division
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator/(const Tuple<T>& t) const
{
	return Tuple<T>(mV[0] / t.mV[0], 
					mV[1] / t.mV[1], 
					mV[2] / t.mV[2]);

}	//End: Tuple<T>::operator/()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator+()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Addition
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator+(T t) const
{
	return Tuple<T>(mV[0] + t, 
					mV[1] + t, 
					mV[2] + t);

}	//End: Tuple<T>::operator+()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator-()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Subtraction
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator-(T t) const
{
	return Tuple<T>(mV[0] - t, 
					mV[1] - t, 
					mV[2] - t);

}	//End: Tuple<T>::operator-()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator*()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Multiplication
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator*(T t) const
{
	return Tuple<T>(mV[0] * t, 
					mV[1] * t, 
					mV[2] * t);

}	//End: Tuple<T>::operator*()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator/()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Division
//--------------------------------------------------------------------------------
template <class T>
Tuple<T> Tuple<T>::operator/(T t) const
{
	return Tuple<T>(mV[0] / t, 
					mV[1] / t, 
					mV[2] / t);

}	//End: Tuple<T>::operator/()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator+=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Addition Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator+= (const Tuple<T>& rhs)
{
	mV[0] += rhs.mV[0];
	mV[1] += rhs.mV[1];
	mV[2] += rhs.mV[2];

	return *this;

}	//End: Tuple<T>::operator+=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator-=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Subtraction Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator-= (const Tuple<T>& rhs)
{
	mV[0] -= rhs.mV[0];
	mV[1] -= rhs.mV[1];
	mV[2] -= rhs.mV[2];

	return *this;

}	//End: Tuple<T>::operator-=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator*=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Multiplication Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator*= (const Tuple<T>& rhs)
{
	mV[0] *= rhs.mV[0];
	mV[1] *= rhs.mV[1];
	mV[2] *= rhs.mV[2];

	return *this;

}	//End: Tuple<T>::operator*=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator/=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/Tuple Division Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator/= (const Tuple<T>& rhs)
{
	mV[0] /= rhs.mV[0];
	mV[1] /= rhs.mV[1];
	mV[2] /= rhs.mV[2];

	return *this;

}	//End: Tuple<T>::operator/=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator+=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Addition Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator+=(T t)
{
	mV[0] += t;
	mV[1] += t;
	mV[2] += t;

	return *this;

}	//End: Tuple<T>::operator+=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator-=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Subtraction Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator-=(T t)
{
	mV[0] -= t;
	mV[1] -= t;
	mV[2] -= t;

	return *this;

}	//End: Tuple<T>::operator-=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator*=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Multiplication Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator*=(T t)
{
	mV[0] *= t;
	mV[1] *= t;
	mV[2] *= t;

	return *this;

}	//End: Tuple<T>::operator*=()


//--------------------------------------------------------------------------------
//	@	Tuple<T>::operator/=()
//--------------------------------------------------------------------------------
//		Element-wise Tuple/T Division Update
//--------------------------------------------------------------------------------
template <class T>
Tuple<T>& Tuple<T>::operator/=(T t)
{
	mV[0] /= t;
	mV[1] /= t;
	mV[2] /= t;

	return *this;

}	//End: Tuple<T>::operator/=()


#endif