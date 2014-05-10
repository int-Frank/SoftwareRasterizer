//================================================================================
// @ DgOrderedArray.h
// 
// Class: DgOrderedArray
//
// An ordered array. Elements are inserted in order. Uses contiguous memory to 
// store data, therefore is best used for very small data types (ie literals) 
// and types with cheap assignment operators.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DGORDEREDARRAY_H
#define DGORDEREDARRAY_H

#include <assert.h>
typedef unsigned int uint32;

//--------------------------------------------------------------------------------
//	@	DgOrderedArray<T>:	T: data type
//--------------------------------------------------------------------------------
template<class T>
class DgOrderedArray
{
public:
	//Constructor / destructor
	DgOrderedArray();
	DgOrderedArray(uint32);
	~DgOrderedArray();

	//Copy operations
	DgOrderedArray(const DgOrderedArray&);
	DgOrderedArray& operator= (const DgOrderedArray&);

	//Accessors

	//Get item at position
	T& operator[](uint32 i)				{return data[i];}
	const T& operator[](uint32 i) const {return data[i];}
	int size()			const			{return current_size;}
	bool empty()		const			{return current_size == 0;}
	int max_size()		const			{return array_size;}

	//Returns false if not found, however, index will be set to
	//the lower index just before where input key would be.
	bool find(const T&, int& index, int lower = 0) const;			//Use binary search
	bool find(const T&, int& index, int lower, int upper) const;	//Use binary search
	
	//Manipulators
	bool insert(const T&);
	bool insert_unique(const T&);
	bool erase(const T&);
	void clear();	
	void resize(uint32);
	void extend();
	void reset();				//Reset data array to size 1.

private:
	//Data members
	T* data;

	int array_size;
	int current_size;

private:
	void init(const DgOrderedArray&);
};


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::DgOrgeredArray()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
DgOrderedArray<T>::DgOrderedArray()
	: data(NULL), array_size(0), current_size(0)
{
}	//End: DgOrderedArray::DgOrderedArray()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::DgOrgeredArray()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
DgOrderedArray<T>::DgOrderedArray(uint32 new_size)
	: data(NULL), array_size(0), current_size(0)
{
	resize(new_size);

}	//End: DgOrderedArray::DgOrderedArray()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::~DgOrgeredArray()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
template<class T>
DgOrderedArray<T>::~DgOrderedArray()
{
	//Free memory
	delete[array_size] data;

}	//End: DgOrderedArray::~DgOrderedArray()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::DgOrgeredArray()
//--------------------------------------------------------------------------------
//		Initialise DgOrderedArray to another.
//--------------------------------------------------------------------------------
template<class T>
void DgOrderedArray<T>::init(const DgOrderedArray& other)
{
	//Resize lists
	resize(other.current_size);
	current_size = other.current_size;

	for (int i = 0; i < other.current_size; ++i)
	{
		data[i] = other.data[i];
	}

}	//End: DgOrderedArray::init()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::DgOrgeredArray()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<class T>
DgOrderedArray<T>::DgOrderedArray(const DgOrderedArray& other): 
	data(NULL), array_size(0), current_size(0)
{
	init(other);

}	//End: DgOrderedArray::DgOrderedArray()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class T>
DgOrderedArray<T>& DgOrderedArray<T>::operator=(const DgOrderedArray& other)
{
	if (this == &other)
		return *this;
	
	init(other);

	return *this;

}	//End: DgOrderedArray::operator=()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::resize()
//--------------------------------------------------------------------------------
//		Resize map, erases all data before resize.
//--------------------------------------------------------------------------------
template<class T>
void DgOrderedArray<T>::resize(uint32 val)
{
	//Delete old data
	delete[array_size] data;
	data = new T[val];

	array_size = val;
	current_size = 0;
	
}	//End: DgOrderedArray::resize()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::find()
//--------------------------------------------------------------------------------
//		Find a value in the list, uses a binary search algorithm
//--------------------------------------------------------------------------------
template<class T>
bool DgOrderedArray<T>::find(const T& item, int& index, int lower) const
{
	return find(item, index, lower, (current_size - 1));
	
}	//End: DgOrderedArray::find()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::find()
//--------------------------------------------------------------------------------
//		Find a value in the list within a range, uses a binary search algorithm
//--------------------------------------------------------------------------------
template<class T>
bool DgOrderedArray<T>::find(const T& item, int& index, int lower, int upper) const
{
	//Check bounds
	lower = (lower>0)?lower:0;
	upper = (upper<current_size-1)?upper:current_size-1;

	while (lower <= upper)
	{
		// calculate the midpoint for roughly equal partition
		index = ( (upper + lower) >> 1 );
 
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

}	//End: DgOrderedArray::find()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::extend()
//--------------------------------------------------------------------------------
//		Extend the array by a factor of 1.5, keeps all data.
//--------------------------------------------------------------------------------
template<class T>
void DgOrderedArray<T>::extend()
{
	//Calculate new size (1.5 * array_size + 1)
	int new_size = ( (array_size * 3) >> 1) + 1;
	
	//Create new arrays
	T* new_data = new T[new_size];

	for (int i = 0; i < current_size; ++i)
		new_data[i] = data[i];

	delete[array_size] data;
	data = new_data;
	array_size = new_size;

}	//End: DgOrderedArray::extend()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::insert()
//--------------------------------------------------------------------------------
//		Insert an element into the list
//--------------------------------------------------------------------------------
template<class T>
bool DgOrderedArray<T>::insert(const T& item)
{
	//Find the index to insert to
	int index;
	find(item, index);
	
	//Range check
	if (current_size ==  array_size)
		extend();

	//shift all RHS objects to the right by one.
	for (int i = current_size-1; i > index; --i)
		data[i + 1] = data[i];
	
	index++;

	data[index] = item;

	current_size++;

	return true;

}	//End: DgOrderedArray::insert()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::insert_unique()
//--------------------------------------------------------------------------------
//		Insert an element into the list, only if it does not yet exist
//--------------------------------------------------------------------------------
template<class T>
bool DgOrderedArray<T>::insert_unique(const T& item)
{
	//Find the index to insert to
	int index;
	if (find(item, index))
		return false;

	//Range check
	if (current_size == array_size)
		extend();

	//shift all RHS objects to the right by one.
	for (int i = current_size - 1; i > index; --i)
		data[i + 1] = data[i];

	index++;

	data[index] = item;

	current_size++;

	return true;

}	//End: DgOrderedArray::insert_unique()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::erase()
//--------------------------------------------------------------------------------
//		Find and removes all of this element from the list.
//--------------------------------------------------------------------------------
template<class T>
bool DgOrderedArray<T>::erase(const T& item)
{
	//Find the index
	int lower, upper;
	if (!find(item, lower))
		return false;	//element not found
	
	//Initial upper bounds
	upper = lower + 1;

	//Find lower bounds
	while (lower >= 0 && data[lower] == item)
		--lower;

	//Find upper bounds
	while (upper <= current_size && data[upper] == item)
		++upper;

	//Number of elements to remove
	lower++;
	int num = upper - lower;

	//Remove elements
	for (; lower < current_size - upper; ++lower)
	{
		data[lower] = data[lower + num];
	}

	//Adjust current_size
	current_size -= num;

	return true;

}	//End: DgOrderedArray::erase()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::reset()
//--------------------------------------------------------------------------------
//		Reset size to 1
//--------------------------------------------------------------------------------
template<class T>
void DgOrderedArray<T>::reset()
{
	//Delete old data
	delete[array_size] data;
	data = 0;

	array_size = 0;
	current_size = 0;
	
}	//End: DgOrderedArray::reset()


//--------------------------------------------------------------------------------
//	@	DgOrgeredArray<T>::clear()
//--------------------------------------------------------------------------------
//		Set the number of elements to zero
//--------------------------------------------------------------------------------
template<class T>
void DgOrderedArray<T>::clear()
{
	current_size = 0;

}	//End: DgOrderedArray::clear()


#endif