/*!
 * @file RNG.cpp
 *
 * @author Frank Hart
 * @date 11/01/2014
 *
 * class definitions: RNG
 */

#include "SimpleRNG.h"
#include "DgTypes.h"
#include "DgError.h"
#include <ctime>

//--------------------------------------------------------------------------------
//	@	Statics
//--------------------------------------------------------------------------------
uint32 SimpleRNG::m_w = 521288629;
uint32 SimpleRNG::m_z = 362436069;


//--------------------------------------------------------------------------------
//	@	SimpleRNG::SetSeed()
//--------------------------------------------------------------------------------
//		Use one value to set the seed
//--------------------------------------------------------------------------------
void SimpleRNG::SetSeed(uint32 seed)
{
	m_w = seed;

}	//End: SimpleRNG::SetSeed()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::SetSeed()
//--------------------------------------------------------------------------------
//		Use two values to set the seed
//--------------------------------------------------------------------------------
void SimpleRNG::SetSeed(uint32 u, uint32 v)
{
	if (u != 0) m_w = u;
	if (v != 0) m_z = v;

}	//End: SimpleRNG::SetSeed()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::SetSeedFromSystemTime()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void SimpleRNG::SetSeedFromSystemTime()
{
	SetSeed((uint32)time(0));

}	//End: SimpleRNG::SetSeedFromSystemTime()





//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetUint()
//--------------------------------------------------------------------------------
//		This is the heart of the generator.
//--------------------------------------------------------------------------------
uint32 SimpleRNG::GetUint()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;

}	//End: SimpleRNG::GetUint()


//--------------------------------------------------------------------------------
//	@	SimpleRNG::GetUint()
//--------------------------------------------------------------------------------
//		This is the heart of the generator.
//--------------------------------------------------------------------------------
uint32 SimpleRNG::GetUint(uint32 a, uint32 b)
{
	if (a > b)
	{
		ERROR_OUT("SimpleRNG::GetUnit(uint32 a, uint32 b) -> lower > upper");
		return a;
	}
	else if (a == b)
	{
		return a;
	}
	

	uint32 val = GetUint();
	uint32 range = b - a;
	return a + (val % range);

}	//End: SimpleRNG::GetUint()


