//================================================================================
// @ CommonMath.h
// 
// Description: 
//
// This files contains a variety of useful math functions and constants.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef COMMONMATH_H
#define COMMONMATH_H

#include <math.h>
#include <cstdlib>
#include "DgTypes.h"


//--------------------------------------------------------------------------------
//		Global Constants
//--------------------------------------------------------------------------------

extern const float PI;
extern const float TwoPI;
extern const float HalfPI;
extern const float OneOverPI;
extern const float EPSILON;
extern const float RAND_MAX_FLOAT;

//--------------------------------------------------------------------------------
//		Trig functions
//--------------------------------------------------------------------------------
#define DgSin(x) sin(x)
#define DgCos(x) cos(x)
#define DgTan(x) tan(x)
#define DgASin(x) asin(x)
#define DgACos(x) acos(x)
#define DgATan(x) atan(x)
#define DgATan2(y,x) atan2(y,x)
#define DgSinCos(a, sina, cosa) sina = DgSin(a); cosa = DgCos(a)	
#define DgLog(x) log(x)

//--------------------------------------------------------------------------------
//		Other useful functions
//--------------------------------------------------------------------------------
#define DgAbs(val) abs(val)
#define DgSqrt(x) sqrt(x)
#define DgInvSqrt(x) 1.0f/DgSqrt(x)
#define DgFloor(val) floor(val)
#define DgCeil(val) ceil(val)
#define DgRound(val) floor(val+0.5f)
#define DgMin(a,b) (a<b)?a:b
#define DgMax(a,b) (a<b)?b:a
#define DgPow(a,exp) pow(a,exp)


//--------------------------------------------------------------------------------
//	@	DgLog2()
//--------------------------------------------------------------------------------
//		Finds log2 of a number
//--------------------------------------------------------------------------------
uint32 DgLog2(uint32 input);


//--------------------------------------------------------------------------------
//	@	FloorPower2()
//--------------------------------------------------------------------------------
//		Floors a number to the nearest power of 2
//--------------------------------------------------------------------------------
uint32 FloorPower2(uint32 input);


//--------------------------------------------------------------------------------
//	@	DgSwap()
//--------------------------------------------------------------------------------
//		Swap function
//--------------------------------------------------------------------------------
template<typename T>
inline void DgSwap(T& a, T& b)
{
	T t = a;
	a = b;
	b = t;

}	//End: DgSwap()


//--------------------------------------------------------------------------------
//	@	rand_int()
//--------------------------------------------------------------------------------
//		Generates a random integer between a and b ( inclusive)
//--------------------------------------------------------------------------------
inline int rand_int(int a, int b)
{
	int range = b-a+1;
	return(a + std::rand()%range);

}	//End: rand_int()


//--------------------------------------------------------------------------------
//	@	rand_float()
//--------------------------------------------------------------------------------
//		Generates a random float between a and b (inclusive)
//--------------------------------------------------------------------------------
inline float rand_float(float a, float b)
{
	float range = b-a;
	return (a + (float)std::rand() / RAND_MAX_FLOAT * range);

}	//End: rand_float()


//--------------------------------------------------------------------------------
//	@	rand_float()
//--------------------------------------------------------------------------------
//		Generates a random float between 0 and 1 (inclusive)
//--------------------------------------------------------------------------------
inline float rand_float()
{
	return ((float)std::rand() / RAND_MAX_FLOAT);

}	//End: rand_float()


//--------------------------------------------------------------------------------
//	@	WrapNumber()
//--------------------------------------------------------------------------------
//		Wrap a number to a range
//--------------------------------------------------------------------------------
inline void WrapNumber(float lower, float upper, float& val)
{
	if (val < lower || val > upper)
	{
		float range = upper - lower;
		val = val - range*floor((val - lower) / range);
	}

}	//End: WrapNum()


//--------------------------------------------------------------------------------
//	@	WrapAngle()
//--------------------------------------------------------------------------------
//		Wraps val to [-PI, PI]
//--------------------------------------------------------------------------------
inline void WrapAngle(float& val)
{
	if (val < -PI || val > PI)
	{
		val = val - TwoPI*DgFloor(val/TwoPI)+PI;
	}

}	//End: WrapAngle()


//--------------------------------------------------------------------------------
//	@	ClampNumber()
//--------------------------------------------------------------------------------
//		Clamp a number to a range
//--------------------------------------------------------------------------------
inline void ClampNumber(float lower, float upper, float& val)
{
	if (val < lower)
		val = lower;
	else if (val > upper)
		val = upper;

}	//End: ClampNumber()


//--------------------------------------------------------------------------------
//	@	Twof_exp()
//--------------------------------------------------------------------------------
//		Returns float(2^input)
//--------------------------------------------------------------------------------
inline float Twof_exp(uint32 a)
{
	return float(1 << a);

}	//End: Twof_exp()


//--------------------------------------------------------------------------------
//	@	Twoi_exp()
//--------------------------------------------------------------------------------
//		Returns int(2^input)
//--------------------------------------------------------------------------------
inline int32 Twoi_exp(uint32 a)
{
	return (1 << a);

}	//End: Twoi_exp()


//--------------------------------------------------------------------------------
//	@	IsZero()
//--------------------------------------------------------------------------------
//		Is the argument close to zero?
//--------------------------------------------------------------------------------
inline bool IsZero(float a)
{
	return ( DgAbs(a) < EPSILON );

}	//End: IsZero()


//--------------------------------------------------------------------------------
//	@	DgAreEqual()
//--------------------------------------------------------------------------------
//		Are a and b close to being equal?
//--------------------------------------------------------------------------------
inline bool DgAreEqual(float a , float b)
{
	return ( ::IsZero(a-b) );

}	//End: DgAreEqual()

#endif