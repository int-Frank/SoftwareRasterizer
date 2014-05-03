//================================================================================
// @ DgMapS.h
// 
// Class: DgMapS
//
// Ordered mapped list. Objects are inserted (in order) with some key. 
// Similar to a DgMap however data is stored with the key. Best used for
// small objects and/or objects with cheap assignment operators. Else
// use a DgMap<U,T>. Only unique keys are allowed.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DGMAPS_H
#define DGMAPS_H

#include <assert.h>
#include "DgTypes.h"

//--------------------------------------------------------------------------------
//	@	DgMapS<U, T>:	U: key
//						T: data
//--------------------------------------------------------------------------------
template<class U, class T>
class DgMapS
{
	//Internal container which stores the data
	struct Container
	{
		U key;
		T item;
	};

public:
	//Constructor / destructor
	DgMapS();
	DgMapS(uint32);
	~DgMapS();

	//Copy operations
	DgMapS(const DgMapS&);
	DgMapS& operator= (const DgMapS&);

	//Accessors

	//Get item at position
	T& operator[](uint32 i)				{return data[i].item;}
	const T& operator[](uint32 i) const {return data[i].item;}
	int size()			const			{return current_size;}
	bool empty()		const			{return current_size == 0;}
	int max_size()		const			{return array_size;}
	U ID(int i)			const			{return data[i].key;}

	//Returns false if not found, however, index will be set to
	//the lower index just before where input key would be.
	bool find(U key, int& index, int lower = 0) const;			//Use binary search
	bool find(U key, int& index, int lower, int upper) const;	//Use binary search
	
	//Manipulators
	bool insert(const T&, U key);		//Use find
	bool set(U key, const T&);			//Use find
	bool erase(U key);					//Use find
	void clear();	
	void resize(uint32);
	void extend();
	void reset();				//Reset data array to size 1.

private:
	//Data members
	Container* data;

	int array_size;
	int current_size;

private:
	void init(const DgMapS&);
};


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::DgMapS()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMapS<U, T>::DgMapS()
	: data(NULL), array_size(1), current_size(0)
{
	resize(1);

}	//End: DgMapS::DgMapS()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::DgMapS()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMapS<U, T>::DgMapS(uint32 new_size)
	: data(NULL), array_size(0), current_size(0)
{
	resize(new_size);

}	//End: DgMapS::DgMapS()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::~DgMapS()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMapS<U, T>::~DgMapS()
{
	//Free memory
	delete[array_size] data;

}	//End: DgMapS::~DgMapS()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::init()
//--------------------------------------------------------------------------------
//		Initialise DgMapS to another.
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMapS<U, T>::init(const DgMapS& other)
{
	//Resize lists
	int sze = (other.current_size>0)?other.current_size:1;
	resize(sze);

	for (int i = 0; i < other.current_size; ++i)
	{
		data[i] = other.data[i];
	}

}	//End: DgMapS::init()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::DgMapS()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMapS<U, T>::DgMapS(const DgMapS& other): 
	data(NULL), array_size(0), current_size(0)
{
	init(other);

}	//End: DgMapS::DgMapS()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class U, class T>
DgMapS<U, T>& DgMapS<U, T>::operator=(const DgMapS& other)
{
	if (this == &other)
		return *this;
	
	init(other);

	return *this;

}	//End: DgMapS::operator=()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::resize()
//--------------------------------------------------------------------------------
//		Resize map, erases all data before resize.
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMapS<U, T>::resize(uint32 val)
{
	//Delete old data
	delete[array_size] data;
	data = new Container[val];

	array_size = val;
	current_size = 0;
	
}	//End: DgMapS::resize()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::find()
//--------------------------------------------------------------------------------
//		Find a value in the map, uses a binary search algorithm
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMapS<U, T>::find(U key, int& index, int lower) const
{
	return find(key, index, lower, (current_size - 1));
	
}	//End: DgMapS::find()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::find()
//--------------------------------------------------------------------------------
//		Find a value in the map within a range, uses a binary search algorithm
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMapS<U, T>::find(U key, int& index, int lower, int upper) const
{
	//Check bounds
	lower = (lower>0)?lower:0;
	upper = (upper<current_size-1)?upper:current_size-1;

	while (lower <= upper)
	{
		// calculate the midpoint for roughly equal partition
		index = ( (upper + lower) >> 1 );
 
		// determine which subarray to search
		if (data[index].key < key)
			// change min index to search upper subarray
			lower = index + 1;
		else if (data[index].key > key)
			// change max index to search lower subarray
			upper = index - 1;
		else
			// key found at index index
			return true;
	}
	
	//Set index closest (but lower) to key
	index = lower - 1;
	return false;

}	//End: DgMapS::find()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::extend()
//--------------------------------------------------------------------------------
//		Extend the map by a factor of 1.5, keeps all data.
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMapS<U, T>::extend()
{
	//Calculate new size (1.5 * array_size + 1)
	int new_size = ( (array_size * 3) >> 1) + 1;
	
	//Create new arrays
	Container* new_data = new Container[new_size];

	for (int i = 0; i < current_size; ++i)
		new_data[i] = data[i];

	delete[array_size] data;
	data = new_data;
	array_size = new_size;

}	//End: DgMapS::extend()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::insert()
//--------------------------------------------------------------------------------
//		Insert an element into the map
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMapS<U, T>::insert(const T& val, U key)
{
	//Find the index to insert to
	int index;
	if (find(key, index))
		return false;	//element already exists
	
	//Range check
	if (current_size ==  array_size)
		extend();

	//shift all RHS objects to the right by one.
	for (int i = current_size-1; i > index; --i)
		data[i + 1] = data[i];
	
	index++;

	data[index].key = key;
	data[index].item = val;

	current_size++;

	return true;

}	//End: DgMapS::insert()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::erase()
//--------------------------------------------------------------------------------
//		Remove an element from the map
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMapS<U, T>::erase(U key)
{
	//Find the index
	int index;
	if (!find(key, index))
		return false;	//element not found
	
	for (int i = index; i < current_size - 1; ++i)
		data[i] = data[i+1];

	current_size--;

	return true;

}	//End: DgMapS::erase()


//--------------------------------------------------------------------------------
//	@	DgMapS<U,T>::set()
//--------------------------------------------------------------------------------
//		Sets an element to a new value
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMapS<U, T>::set(U key, const T& val)
{
	//Find the index to insert to
	int index;
	if (!find(key, index))
		return false;	//element does not exist

	data[index].item = val;

	return true;

}	//End: DgMapS::set()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::reset()
//--------------------------------------------------------------------------------
//		Reset size to 1
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMapS<U, T>::reset()
{
	resize(1);

}	//End: DgMapS::reset()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::clear()
//--------------------------------------------------------------------------------
//		Set the number of elements to zero
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMapS<U, T>::clear()
{
	//Set pointers

	current_size = 0;

}	//End: DgMapS::clear()


#endif