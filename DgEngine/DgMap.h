//================================================================================
// @ DgMap.h
// 
// Class: DgMap
//
// Ordered mapped list. Objects are inserted (in order) with some key. 
// Internally, data is stored separatly in a pre-allocated linked list to avoid
// excessive copying. Only unique keys are allowed.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DGMAP_H
#define DGMAP_H

#include <assert.h>
#include "DgTypes.h"

//--------------------------------------------------------------------------------
//	@	DgMap<U, T>:	U: key
//						T: data
//--------------------------------------------------------------------------------
template<class U, class T>
class DgMap
{
	//Internal container which stores the data
	struct DataContainer
	{
		DataContainer(): next(NULL), prev(NULL){}

		DataContainer* next;
		DataContainer* prev;
		T item;
	};

	//Internal container which stores key and pointer to data
	struct KeyContainer
	{
		KeyContainer(): item_ptr(NULL) {}

		U key;
		DataContainer* item_ptr;
	};


public:
	//Constructor / destructor
	DgMap();
	DgMap(uint32);
	~DgMap();

	//Copy operations
	DgMap(const DgMap&);
	DgMap& operator= (const DgMap&);

	//Accessors

	//Get item at position
	T& operator[](uint32 i)				{return keys[i].item_ptr->item;} 
	const T& operator[](uint32 i) const {return keys[i].item_ptr->item;}
	int size()			const			{return current_size;}
	bool empty()		const			{return current_size == 0;}
	int max_size()		const			{return array_size;}
	U ID(int i)			const			{return keys[i].key;}

	//Returns false if not found, however, index will be set to
	//the lower index just before where input key would be.
	bool find(U key, int& index, int lower = 0) const;		
	bool find(U key, int& index, int lower, int upper) const;	
	
	//Manipulators
	bool insert(const T&, U key);
	bool set(U key, const T&);
	bool erase(U key);
	void clear();	
	void resize(uint32);
	void extend();
	void reset();				//Reset data array to size 1.

private:
	//Data members
	DataContainer* data;
	KeyContainer* keys;

	int array_size;
	int current_size;

	//Next free container to fill
	DataContainer* next_ptr;

private:
	void init(const DgMap&);
};


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::DgMap()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMap<U, T>::DgMap()
	: keys(NULL), data(NULL), array_size(0), current_size(0)
{
	resize(1);

}	//End: DgMap::DgMap()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::DgMap()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMap<U, T>::DgMap(uint32 new_size)
	: keys(NULL), data(NULL), array_size(0), current_size(0)
{
	assert(new_size>0);

	resize(new_size);

}	//End: DgMap::DgMap()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::~DgMap()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMap<U, T>::~DgMap()
{
	//Free memory
	delete[array_size] keys;
	delete[array_size+2] data;

}	//End: DgMap::~DgMap()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::init()
//--------------------------------------------------------------------------------
//		Initialise DgMap to another.
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMap<U, T>::init(const DgMap& other)
{
	//Resize lists
	int sze = (other.current_size>0)?other.current_size:1;
	resize(sze);

	for (int i = 0; i < other.current_size; ++i)
	{
		insert(other.keys[i].item_ptr->item, other.keys[i].key);
	}

}	//End: DgMap::init()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::DgMap()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<class U, class T>
DgMap<U, T>::DgMap(const DgMap& other): 
	keys(NULL), array_size(0), current_size(0)
{
	init(other);

}	//End: DgMap::DgMap()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class U, class T>
DgMap<U, T>& DgMap<U, T>::operator=(const DgMap& other)
{
	if (this == &other)
		return *this;
	
	init(other);

	return *this;

}	//End: DgMap::operator=()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::resize()
//--------------------------------------------------------------------------------
//		Resize map, erases all data before resize.
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMap<U, T>::resize(uint32 val)
{
	//Delete old data
	delete[array_size+2] data;
	delete[array_size] keys;
	
	//Adjust sizes
	array_size = val;
	current_size = 0;

	//Create new arrays
	data = new DataContainer[array_size+2];
	keys = new KeyContainer[array_size];

	//Set pointers
	next_ptr = &data[1];
	for (int i = 1; i < array_size; ++i)
		data[i].next = &data[i+1];
	
	data[0].next = &data[array_size+1];
	data[array_size+1].prev = &data[0];
	
}	//End: DgMap::resize()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::find()
//--------------------------------------------------------------------------------
//		Find a value in the map, uses a binary search algorithm
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMap<U, T>::find(U key, int& index, int lower) const
{
	return find(key, index, lower, (current_size - 1));
	
}	//End: DgMap::find()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::find()
//--------------------------------------------------------------------------------
//		Find a value in the map within a range, uses a binary search algorithm
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMap<U, T>::find(U key, int& index, int lower, int upper) const
{
	//Check bounds
	lower = (lower>0)?lower:0;
	upper = (upper<current_size-1)?upper:current_size-1;

	while (lower <= upper)
	{
		// calculate the midpoint for roughly equal partition
		index = ( (upper + lower) >> 1 );
 
		// determine which subarray to search
		if (keys[index].key < key)
			// change min index to search upper subarray
			lower = index + 1;
		else if (keys[index].key > key)
			// change max index to search lower subarray
			upper = index - 1;
		else
			// key found at index index
			return true;
	}
	
	//Set index closest (but lower) to key
	index = lower - 1;
	return false;

}	//End: DgMap::find()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::extend()
//--------------------------------------------------------------------------------
//		Extend the map by a factor of 1.5, keeps all data.
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMap<U, T>::extend()
{
	//Calculate new size (1.5 * array_size + 1)
	int new_size = ( (array_size * 3) >> 1) + 1;
	
	//Create new arrays
	DataContainer* new_data = new DataContainer[new_size+2];
	KeyContainer* new_keys = new KeyContainer[new_size];

	//Copy over data
	for (int i = 0; i < current_size; ++i)
	{
		//Add in data
		DataContainer* ptr = keys[i].item_ptr;
		new_data[i+1].item = keys[i].item_ptr->item;
		new_data[i+1].prev = &new_data[i];
		new_data[i+1].next = &new_data[i+2];

		//Add in key
		new_keys[i].key = keys[i].key;
		new_keys[i].item_ptr = &new_data[i+1];
	}

	//Set next pointers
	for (int i = current_size+1; i < new_size + 1; ++i)
		new_data[i].next = &new_data[i+1];
	
	//Set end pointers
	new_data[0].next = &new_data[1];
	new_data[new_size + 1].prev = &new_data[current_size];
	new_data[current_size].next = &new_data[new_size + 1];

	//Set special pointers
	next_ptr = &new_data[current_size+1];

	//Swap data
	delete[array_size+2] data;
	delete[array_size] keys;
	data = new_data;
	keys = new_keys;

	//Adjust sizes
	array_size = new_size;

}	//End: DgMap::extend()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::insert()
//--------------------------------------------------------------------------------
//		Insert an element into the map
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMap<U, T>::insert(const T& val, U key)
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
		keys[i + 1] = keys[i];
	
	//Increment the index to the newly made free space in keys
	index++;
	
	//Get the data node to work on
	DataContainer *new_element = next_ptr;
	
	//Move next_free pointer to the next data_container
	next_ptr = next_ptr->next;

	//Assign data item
	new_element->item = val;

	//Insert value into the linked list
	data[array_size+1].prev->next = new_element;
	new_element->prev = data[array_size+1].prev;
	new_element->next = &data[array_size+1];
	data[array_size+1].prev = new_element;

	//Create KeyContainer
	keys[index].key = key;
	keys[index].item_ptr = new_element;

	//Adjust current size
	current_size++;

	return true;

}	//End: DgMap::insert()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::erase()
//--------------------------------------------------------------------------------
//		Remove an element from the map
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMap<U, T>::erase(U key)
{
	//Find the index
	int index;
	if (!find(key, index))
		return false;	//element not found
	
	//Remove item from data
	keys[index].item_ptr->next->prev = keys[index].item_ptr->prev;
	keys[index].item_ptr->prev->next = keys[index].item_ptr->next;
	
	//Add this broken item to the begining of the free list
	keys[index].item_ptr->next = next_ptr;				//put item in between next_free and end of the list
	
	//Reset next_free
	next_ptr = keys[index].item_ptr;
	
	//shift all RHS objects to the left one.
	for (int i = index; i < current_size-1; ++i)
		keys[i] = keys[i + 1];

	//Set current size
	current_size--;

	return true;

}	//End: DgMap::erase()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::set()
//--------------------------------------------------------------------------------
//		Sets an element to a new value
//--------------------------------------------------------------------------------
template<class U, class T>
bool DgMap<U, T>::set(U key, const T& val)
{
	//Find the index to insert to
	int index;
	if (!find(key, index))
		return false;	//element does not exist

	*keys[index].item_ptr = val;

	return true;

}	//End: DgMap::set()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::reset()
//--------------------------------------------------------------------------------
//		Reset size to 1
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMap<U, T>::reset()
{
	resize(1);

}	//End: DgMap::reset()


//--------------------------------------------------------------------------------
//	@	DgMap<U,T>::clear()
//--------------------------------------------------------------------------------
//		Set the number of elements to zero
//--------------------------------------------------------------------------------
template<class U, class T>
void DgMap<U, T>::clear()
{
	//Set pointers
	next_ptr = &data[1];
	for (int i = 1; i < array_size; ++i)
		data[i].next = &data[i+1];
	
	data[0].next = &data[array_size+1];
	data[array_size+1].prev = &data[0];

	current_size = 0;

}	//End: DgMap::clear()


#endif