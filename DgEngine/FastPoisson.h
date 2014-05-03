/*!
 * @file FastPoisson.h
 *
 * @author Frank Hart
 * @date 14/01/2014
 *
 * class : FastPoisson.h
 */

#ifndef FASTPOISSON_H
#define FASTPOISSON_H

#include "NormalDistributionBounded.h"

/*!
 * @ingroup math
 *
 * @class FastPoisson
 *
 * @brief Returns number of events in a given time interval
 *
 * Given a rate of occurance and a time interval, this class
 * returns a number of events following the poisson distribution.
 *
 * @author Frank Hart
 * @date 14/01/2014
 */
template<typename T>
class FastPoisson
{
public:

	FastPoisson(){}
	~FastPoisson() {}

	FastPoisson(const FastPoisson&);
	FastPoisson& operator= (const FastPoisson&);

	//! Generate number of events.
	uint32 Get(T dt);

	//! Get the rate
	T GetRate() const { return rate; }

	//! Set the rate and number of stddevs above
	//! the rate to limit results.
	void Set(T _rate);

private:

	NormalDistributionBounded<T> distribution;

	T rate;

};


//--------------------------------------------------------------------------------
//	@	FastPoisson::FastPoisson()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<typename T>
FastPoisson<T>::FastPoisson(const FastPoisson& other): 
distribution(other.distribution), rate(other.rate)
{
}	//End: FastPoisson::FastPoisson()


//--------------------------------------------------------------------------------
//	@	FastPoisson::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<typename T>
FastPoisson<T>& FastPoisson<T>::operator=(
	const FastPoisson& other)
{
	if (this == &other)
		return *this;

	distribution = other.distribution;
	rate = other.rate;

	return *this;

}	//End: FastPoisson::operator=()


//--------------------------------------------------------------------------------
//	@	FastPoisson::Set()
//--------------------------------------------------------------------------------
//		Set up the distribution
//--------------------------------------------------------------------------------
template<typename T>
void FastPoisson<T>::Set(T _rate)
{
	if (_rate < T(0.0))
		rate = T(1.0);
	else
		rate = _rate;

}	//End: FastPoisson::Set()


//--------------------------------------------------------------------------------
//	@	FastPoisson::Set()
//--------------------------------------------------------------------------------
//		Set up the distribution
//--------------------------------------------------------------------------------
template<typename T>
uint32 FastPoisson<T>::Get(T dt)
{
	//Set the distribution values
	T mean = rate * dt;
	T stddev = DgSqrt(mean);

	T lower = T(0.0);
	T upper = mean + T(3.0) * stddev;

	//Set the bounds
	distribution.Set(mean, stddev, lower, upper);

	//Increase upperbound to max
	distribution.upperBound = (NORMAL_DISTRIBUTION_TABLE_SIZE - 1);

	//Increment lower bound, else a value less then 0 migh be returned
	distribution.lowerBound++;
	
	return uint32(distribution.Get());

}	//End: FastPoisson::FastPoisson()

#endif