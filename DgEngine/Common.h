/*!
 * @file Common.h
 *
 * @author Frank Hart
 * @date 2/01/2014
 *
 * This file contains a list of helper functions that don't really belong
 * anywhere else.
 */


#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <list>
#include "DgTypes.h"


/*!
 * @ingroup utility_other
 *
 * Find an element in an std::list. If the element is not found, it is added.
 *
 * @param[in] input_list The list to be searched.
 * @param[in] val The element to find/add.
 * @return Pointer to the found element.
 */
template<class T>
T* FindAdd(std::list<T>& input_list, const T& val)
{
	std::list<T>::iterator it;

	it = std::find(input_list.begin(), input_list.end(), val);

	if (it == input_list.end())
	{
		input_list.push_back(val);
		return &input_list.back();
	}

	return &(*it);

}	//End:FindAdd()


/*!
 * @ingroup utility_other
 *
 * Use a binary search method to find an element in an array.
 *
 * @param[in] data The array to search
 * @param[in] lower Lower limit, inclusive
 * @param[in] upper Upper limit, inclusive
 * @param[in] item The element to find.
 * @param[out] index The index of the item, or one less then
 *                   where the item should be.
 * @return True if the item was found.
 */
template<class T>
bool DgBinarySearch(const T* data, int32 lower, int32 upper, T item, int32& index)
{
	while (lower <= upper)
	{
		// calculate the midpoint for roughly equal partition
		index = ((upper + lower) >> 1);

		// determine which subarray to search
		if (data[index] < item)
			// change min index to search upper subarray
			lower = index + 1;
		else if (data[index] > item)
			// change max index to search lower subarray
			upper = index - 1;
		else
			// key found at index index
			return true;
	}

	//Set index closest (but lower) to key
	index = lower - 1;
	return false;

}	//End: DgBinarySearch()


#endif