/*!
* @file DgArray.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: DgArray<>
*/

#ifndef DGARRAY_H
#define DGARRAY_H

#include <assert.h>
#include "DgTypes.h"

//--------------------------------------------------------------------------------
//	@	DgArray<T>:	T: data
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_container
 *
 * @class DgArray
 *
 * @brief Contiguous array, similar to std::vector.
 * 
 * http://www.cplusplus.com/reference/vector/vector/ 
 *
 * DgArrays are sequence containers representing arrays that can change in size.
 * Just like arrays, DgArrays use contiguous storage locations for their elements, 
 * which means that their elements can also be accessed using offsets on regular 
 * pointers to its elements, and just as efficiently as in arrays. But unlike arrays, 
 * their size can change dynamically, with their storage being handled automatically by 
 * the container.
 *
 * Internally, DgArrays use a dynamically allocated array to store their elements. 
 * This array may need to be reallocated in order to grow in size when new elements 
 * are inserted, which implies allocating a new array and moving all elements to it. 
 * This is a relatively expensive task in terms of processing time, and thus, DgArrays 
 * do not reallocate each time an element is added to the container.
 *
 * Instead, DgArray containers may allocate some extra storage to accommodate for 
 * possible growth, and thus the container may have an actual capacity greater than 
 * the storage strictly needed to contain its elements (i.e., its size). 
 * 
 *
 * @author Frank Hart
 * @date 7/01/2014
 */
template<class T>
class DgArray
{
public:
	//Constructor / destructor
	DgArray();

	//! Construct with a set size
	DgArray(uint32);
	~DgArray();

	DgArray(const DgArray&);
	DgArray& operator= (const DgArray&);

	//! Copy both the current elements and the elements in the reserved memory.
	void CopyAll(const DgArray& other);

	//! Access element
	T& operator[](uint32 i)				{return data[i];}

	//! Accessor, no range check.
	const T& operator[](uint32 i) const	{return data[i];}

	//! Accessor with range check.
	T& at(uint32);

	//! Accessor with range check.
	const T& at(uint32) const;

	//! Current size of the array
	uint32 size()		const			{return current_size;}

	//! Is the array empty
	bool empty()		const			{return current_size == 0;}

	//! Size of the reserved memory.
	uint32 max_size()	const			{return array_size;}

	//! Get pointer to first element.
	T* Data()							{return data;}

	//! Get pointer to first element.
	const T* Data()		const			{return data;}

	//! Add element to the back of the array.
	void push_back(const T&);

	//! Remove element from the back of the array.
	void pop_back();

	//! Current size is flagged as 0. Elements are NOT destroyed.
	void clear();

	//! Set the current size to 0 and the reserve to new_size
	void resize(uint32 new_size);

	//! Exteneds the total size of the array (current + reserve) by a factor of 1.5
	void extend();

private:
	//Data members
	T* data;
	uint32 array_size;
	uint32 current_size;

private:
	void init(const DgArray&);
};


//--------------------------------------------------------------------------------
//	@	DgArray<T>::DgArray()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
DgArray<T>::DgArray(): data(NULL), array_size(0), current_size(0)
{
}	//End: DgArray::DgArray()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::DgArray()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
DgArray<T>::DgArray(uint32 new_size): data(NULL), current_size(0), array_size(new_size)
{
	//Initialise pointers
	data = new T[new_size];

}	//End: DgArray::DgArray()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::~DgArray()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
template<class T>
DgArray<T>::~DgArray()
{
	//Free memory
	delete[] data;

}	//End: DgArray::~DgArray()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::init()
//--------------------------------------------------------------------------------
//		Initialise array to another DgArray
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::init(const DgArray& other)
{
	//Delete current array
	delete[] data;

	//Set sizes
	array_size = other.array_size;
	current_size = other.current_size;

	//Set data
	data = new T[array_size];

	//Copy contents
	for (uint32 i = 0; i < current_size; i++)
	{
		data[i] = other.data[i];
	}
}	//End: DgArray<T>::init()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::DgArray()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<class T>
DgArray<T>::DgArray(const DgArray& other): data(NULL)
{
	init(other);

}	//End: DgArray::DgArray()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class T>
DgArray<T>& DgArray<T>::operator=(const DgArray& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;
}	//End: DgArray::operator=()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::CopyAll()
//--------------------------------------------------------------------------------
//		Copies entire array
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::CopyAll(const DgArray<T>& other)
{
	if (array_size != other.array_size)
		resize(other.array_size);

	//Copy contents
	for (uint32 i = 0; i < array_size; i++)
	{
		data[i] = other.data[i];
	}

}	//End: DgArray<T>::CopyAll()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::at()
//--------------------------------------------------------------------------------
//		Accessor with range check
//--------------------------------------------------------------------------------
template<class T>
T& DgArray<T>::at(uint32 index)
{
	if (index >= current_size)
		throw std::out_of_range("DgArray: range error");

	return data[index];

}	//End: DgArray<T>::at()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::at()
//--------------------------------------------------------------------------------
//		const accessor with range check
//--------------------------------------------------------------------------------
template<class T>
const T& DgArray<T>::at(uint32 index) const
{
	if (index >= current_size)
		throw std::out_of_range("DgArray: range error");

	return data[index];

}	//End: DgArray<T>::at()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::push_back()
//--------------------------------------------------------------------------------
//		Add element to the back of the array
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::push_back(const T& val)
{
	//Range check
	if (current_size ==  array_size)
	{
		extend();
	}

	//Set element
	data[current_size] = val;

	//increment current size
	++current_size;

}	//End: DgArray<T>::push_back()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::pop_back
//--------------------------------------------------------------------------------
//		Pop an element from the back of the array
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::pop_back()
{
	//Range check
	if (current_size == 0)
		return;
	
	//Deincrement current size
	--current_size;

}	//End: DgArray<T>::pop_back()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::clear()
//--------------------------------------------------------------------------------
//		Clear array
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::clear()
{
	//Set current size to 0
	current_size = 0;

}	//End: DgArray::clear()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::resize()
//--------------------------------------------------------------------------------
//		Resize array, erases all data before resize.
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::resize(uint32 new_size)
{
	//Delete current array
	delete[] data;

	//Set sizes
	array_size = new_size;
	current_size = 0;

	//Set data
	data = new T[array_size];

}	//End: DgArray::resize()


//--------------------------------------------------------------------------------
//	@	DgArray<T>::extend()
//--------------------------------------------------------------------------------
//		Extend the array by a factor of 1.5, keeps all data.
//--------------------------------------------------------------------------------
template<class T>
void DgArray<T>::extend()
{
	//Calculate new size (1.5 * array_size + 1)
	int new_size = ( (array_size * 3) >> 1) + 1;
	
	//Create new arrays
	T* new_data = new T[new_size];

	//Copy data
	for (uint32 i = 0; i < current_size; ++i)
		new_data[i] = data[i];

	//Assign new values
	delete[array_size] data;
	data = new_data;
	array_size = new_size;

}	//End: DgArray::extend()



//--------------------------------------------------------------------------------
//		Helpful functions
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	find()
//--------------------------------------------------------------------------------
//		Find a value in the list, returns reference
//--------------------------------------------------------------------------------
template<class T>
T* find (DgArray<T>& container, const T& val)
{
	for (uint32 i = 0; i < container.size(); ++i) 
	{
		if (container[i] == val) 
			return &container[i];
	}

	return NULL;

}	//End find()


//--------------------------------------------------------------------------------
//	@	fill()
//--------------------------------------------------------------------------------
//		Fill array with value
//--------------------------------------------------------------------------------
template<class T>
void fill(DgArray<T>& container, const T& val)
{
	for (uint32 i = 0; i < container.size(); ++i) 
	{
		container[i] = val;
	}

}	//End: fill()

#endif