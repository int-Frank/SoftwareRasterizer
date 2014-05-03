/*!
 * @file SimpleRNG.h
 *
 * @author Frank Hart
 * @date 11/01/2014
 *
 * Class declaration: SimpleRNG
 */

#ifndef RANDOMBASE_H
#define RANDOMBASE_H

#include <random>
#include "DgTypes.h"
#include "CommonMath.h"


/*!
 * @ingroup
 *
 * @class SimpleRNG
 *
 * @brief Base class for all random number classes
 *
 * [detailed description]
 *
 * @author John D. Cook 
 * @date 11/01/2014
 */
class SimpleRNG
{
public:

	SimpleRNG() {}
	~SimpleRNG() {}

	SimpleRNG(const SimpleRNG&) {}
	SimpleRNG& operator= (const SimpleRNG&) { return *this; }

	//! Seed the internal generator
	static void SetSeed(uint32 seed);

	//! Seed the internal generator
	static void SetSeed(uint32 seed1, uint32 seed2);

	//! Seeds the generator with a value taken from the current time.
	static void SetSeedFromSystemTime();

	//! Produce a uniform random sample from the open interval (0, 1).
	template<class T>
	T GetUniform();

	//! Produce a uniform random sample from the open interval (a, b).
	template<class T>
	T GetUniform(T a, T b);

	//! Get random unsigned integer within the interval [a, b]
	uint32 GetUint(uint32 a, uint32 b);

	//! Get random unsigned integer.
	uint32 GetUint();

	//! Get a Gaussian random sample with mean 0 and std 1.
	template<class T>
	T GetNormal();

	//! Get a Gaussian random sample with specified mean and standard deviation
	template<class T>
	T GetNormal(T mean, T std);

	template<class T>
	T GetGamma(T shape, T scale);


private:

	static uint32 m_w;
	static uint32 m_z;

};


//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetUniform()
//--------------------------------------------------------------------------------
//		The method will not return either end point.
//--------------------------------------------------------------------------------
template<class T>
T SimpleRNG::GetUniform()
{
	// 0 <= u < 2^32
	uint32 u = GetUint();
	// The magic number below is 1/(2^32 + 2).
	// The result is strictly between 0 and 1.
	return T(u + 1) * T(2.328306435454494e-10);

}	//End: SimpleRNG::GetUniform()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetUniform()
//--------------------------------------------------------------------------------
//		The method will not return either end point.
//--------------------------------------------------------------------------------
template<class T>
T SimpleRNG::GetUniform(T a, T b)
{
	if (b < a)
	{
		return a;
	}

	T range = b - a;
	return GetUniform<T>() * range + a;

}	//End: SimpleRNG::GetUniform()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetNormal()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
template<class T>
T SimpleRNG::GetNormal()
{
	// Use Box-Muller algorithm
	T u1 = GetUniform<T>();
	T u2 = GetUniform<T>();
	T r = DgSqrt(T(-2.0)*DgLog(u1));
	T theta = T(6.283185307179586476925286766559)*u2;
	return r*DgSin(theta);
}	//End: SimpleRNG::GetNormal()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetNormal()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
template<class T>
T SimpleRNG::GetNormal(T mean, T std)
{
	if (standardDeviation <= T(0.0))
	{
		return mean;
	}
	return mean + std*GetNormal<T>();

}	//End: SimpleRNG::GetNormal()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetGamma()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
template<class T>
T SimpleRNG::GetGamma(T shape, T scale)
{
	// Implementation based on "A Simple Method for Generating Gamma Variables"
	// by George Marsaglia and Wai Wan Tsang.  ACM Transactions on Mathematical Software
	// Vol 26, No 3, September 2000, pages 363-372.

	T d, c, x, xsquared, v, u;

	if (shape >= T(1.0))
	{
		d = shape - T(0.33333333333333333333333333333333);
		c = T(1.0) / DgSqrt(T(9.0)*d);
		for (;;)
		{
			do
			{
				x = GetNormal<T>();
				v = T(1.0) + c*x;
			} while (v <= T(0.0));
			v = v*v*v;
			u = GetUniform<T>();
			xsquared = x*x;
			if (u < T(1.0) - T(0.0331)*xsquared*xsquared || DgLog(u) < T(0.5)*xsquared + d*(T(1.0) - v + DgLog(v)))
				return scale*d*v;
		}
	}
	else if (shape <= T(0.0))
	{
		/*string msg = string.Format("Shape must be positive. Received {0}.", shape);
		throw new ArgumentOutOfRangeException(msg)*/
		return T(-1.0);

	}
	else
	{
		T g = GetGamma(shape + T(1.0), T(1.0));
		T w = GetUniform<T>();
		return scale*g*std::pow(w, T(1.0) / shape);
	}
}	//End: SimpleRNG::GetGamma()

#endif