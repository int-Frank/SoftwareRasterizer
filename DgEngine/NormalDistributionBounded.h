/*!
 * @file NormalDistributionBounded
 *
 * @author Frank Hart
 * @date 10/01/2014
 *
 * Class declaration: NormalDistributionBounded
 */

#ifndef NORMAL_DISTRIBUTION_BOUNDED_H
#define NORMAL_DISTRIBUTION_BOUNDED_H

#include <random>
#include "DgTypes.h"
#include "Dg_io.h"
#include "Common.h"
#include "NormalDistributionTable.inl"
#include "SimpleRNG.h"

template<typename T>
class FastPoisson;

/*!
 * @ingroup math
 *
 * @class NormalDistributionBounded
 *
 * @brief Generates normal distributed random values with bounds
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date 10/01/2014
 */
template<typename T>
class NormalDistributionBounded
{
	friend class FastPoisson<T>;

public:

	struct param
	{
		friend DgReader& operator>>(DgReader& in, param& dest)
		{
			//Error check
            if ((in >> dest.mean >> dest.stddev >> dest.lowerBound >> dest.upperBound).fail())
			{
                std::cerr << "@operator>>(NormalDistributionBounded<T>::param&) -> Bad read.") <<
                std:endl;
			}

			return in;

		}	//End: operator>>()

		param(){}
		param(T m, T s, T l, T u): mean(m), stddev(s),
			lowerBound(l), upperBound(u) {}

		T mean;
		T stddev;
		T lowerBound;
		T upperBound;
	};

public:

	NormalDistributionBounded(){}
	NormalDistributionBounded(const typename NormalDistributionBounded::param&);
	NormalDistributionBounded(T _mean, T _stddev, T lower, T upper);
	~NormalDistributionBounded() {}

	NormalDistributionBounded(const NormalDistributionBounded&);
	NormalDistributionBounded& operator= (const NormalDistributionBounded&);

	//! Get the mean
	T Mean() const { return mean; }

	//! Get the standard deviation
	T Stddev() const { return stddev; }

	//! Get the lower bound
	T min() const { return lowerBound; }

	//! Get the upper bound
	T max() const { return upperBound; }

	//! Set all parameters.
	void Set(T _mean, T _stddev, T lower, T upper);
	void Set(const typename NormalDistributionBounded::param&);

	//! get a random number
	T Get();

private:

	//--------------------------------------------------------------------------------
	//	 	Data
	//--------------------------------------------------------------------------------

	//Statistical data
	T mean;
	T stddev;

	//Bounds
	T lowerBound;
	T upperBound;

	//The range in the table to choose numbers from
	uint32 lowerRef;
	uint32 upperRef;

	//Random integer distribution
	SimpleRNG generator;

	//--------------------------------------------------------------------------------
	//	 	Functions
	//--------------------------------------------------------------------------------

	void init(const NormalDistributionBounded& other);

	// Bounds will be closest values in the table to the input
	void SetBounds(T lower, T upper);

	//--------------------------------------------------------------------------------
	//	 	Statics
	//--------------------------------------------------------------------------------

	//A table of normal distribution values.
	static const T distributionValues[NORMAL_DISTRIBUTION_TABLE_SIZE];
};


//--------------------------------------------------------------------------------
//	@	Typedefs
//--------------------------------------------------------------------------------
//		Common typedefs
//--------------------------------------------------------------------------------
typedef NormalDistributionBounded<float>			ndb_f;
typedef NormalDistributionBounded<float>::param		ndb_f_param;


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded<T>::distributionValues[]
//--------------------------------------------------------------------------------
//		The distribution table
//--------------------------------------------------------------------------------
template<typename T>
const T NormalDistributionBounded<T>::distributionValues[NORMAL_DISTRIBUTION_TABLE_SIZE] =
NORMAL_DISTRIBUTION_TABLE;


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::NormalDistributionBounded()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<typename T>
NormalDistributionBounded<T>::NormalDistributionBounded(
	const typename NormalDistributionBounded::param& values)
{
	Set(values);

}	//End: NormalDistributionBounded::NormalDistributionBounded()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::NormalDistributionBounded()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<typename T>
NormalDistributionBounded<T>::NormalDistributionBounded(
	T _mean, T _stddev, T lower, T upper)
{
	Set(_mean, _stddev, lower, upper);

}	//End: NormalDistributionBounded::NormalDistributionBounded()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<typename T>
void NormalDistributionBounded<T>::init(const NormalDistributionBounded& other)
{
	mean = other.mean;
	stddev = other.stddev;
	upperBound = other.upperBound;
	lowerBound = other.lowerBound;
	lowerRef = other.lowerRef;
	upperRef = other.upperRef;

	generator = other.generator;

}	//End: NormalDistributionBounded::operator=()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::NormalDistributionBounded()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<typename T>
NormalDistributionBounded<T>::NormalDistributionBounded(
	const NormalDistributionBounded& other)
{
	init(other);

}	//End: NormalDistributionBounded::NormalDistributionBounded()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<typename T>
NormalDistributionBounded<T>& NormalDistributionBounded<T>::operator=(
	const NormalDistributionBounded& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;

}	//End: NormalDistributionBounded::operator=()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::()
//--------------------------------------------------------------------------------
//		Set the mean of the distribution
//--------------------------------------------------------------------------------
template<typename T>
void NormalDistributionBounded<T>::SetBounds(T lower, T upper)
{
	if (stddev == T(0.0))
	{
		//Set the lower and upper to the mean.
		lowerBound = mean;
		upperBound = mean;

		lowerRef = (NORMAL_DISTRIBUTION_TABLE_SIZE / 2) + 1;
		upperRef = lowerRef;

		return;
	}

	//Convert the bounds
	T lb_norm = (lower - mean) / stddev;
	T ub_norm = (upper - mean) / stddev;

	// Ensure lb_norm < table[0] and ub_norm < table[end]
	if (lb_norm < distributionValues[0])
	{
		lb_norm = distributionValues[0];
	}
	if (ub_norm > distributionValues[NORMAL_DISTRIBUTION_TABLE_SIZE - 1])
	{
		ub_norm = distributionValues[NORMAL_DISTRIBUTION_TABLE_SIZE - 1];
	}

	int32 l_int, u_int;

	//Check for valid range
	if (lb_norm > ub_norm)
	{
        std::cerr << "@NormalDistributionBounded::SetBounds() -> invalid bounds." << std::endl;

		//Set the lower and upper to the mean.
		lowerBound = mean;
		upperBound = mean;

		lowerRef = (NORMAL_DISTRIBUTION_TABLE_SIZE / 2) + 1;
		upperRef = lowerRef;
	}
	else
	{
		DgBinarySearch(distributionValues, 0, (NORMAL_DISTRIBUTION_TABLE_SIZE - 1), lb_norm, l_int);
		lowerRef = uint32(l_int);
		lowerBound = distributionValues[lowerRef] * stddev + mean;

		DgBinarySearch(distributionValues, 0, (NORMAL_DISTRIBUTION_TABLE_SIZE - 1), ub_norm, u_int);
		upperRef = uint32(u_int);
		upperBound = distributionValues[upperRef] * stddev + mean;
	}

}	//End: NormalDistributionBounded::SetBounds()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::Set()
//--------------------------------------------------------------------------------
//		Set up the distribution
//--------------------------------------------------------------------------------
template<typename T>
void NormalDistributionBounded<T>::Set(T _mean, T _stddev, T lower, T upper)
{
	//Set the distribution values
	mean = _mean;

	if (_stddev < T(0.0))
		stddev = T(1.0);
	else
		stddev = _stddev;

	//Set the bounds
	SetBounds(lower, upper);

}	//End: NormalDistributionBounded::NormalDistributionBounded()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::Set()
//--------------------------------------------------------------------------------
//		Set up the distribution
//--------------------------------------------------------------------------------
template<typename T>
void NormalDistributionBounded<T>::Set(const typename NormalDistributionBounded::param& input)
{
	//Set the distribution values
	mean = input.mean;
	stddev = input.stddev;

	//Set the bounds
	SetBounds(input.lowerBound, input.upperBound);

}	//End: NormalDistributionBounded::NormalDistributionBounded()


//--------------------------------------------------------------------------------
//	@	NormalDistributionBounded::()
//--------------------------------------------------------------------------------
//		Get a random number
//--------------------------------------------------------------------------------
template<typename T>
T NormalDistributionBounded<T>::Get()
{
	//try this many times to generate a number in range
	uint32 ref = generator.GetUint(lowerRef, upperRef);
	T result = distributionValues[ref];
	return (result * stddev) + mean;

}	//End: NormalDistributionBounded::Get()




#endif