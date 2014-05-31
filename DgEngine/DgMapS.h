//================================================================================
// @ map_s.h
// 
// Class: map_s
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

namespace Dg
{

  //--------------------------------------------------------------------------------
  //	@	map_s<U, T>:	U: key
  //						T: data
  //--------------------------------------------------------------------------------
  template<class U, class T>
  class map_s
  {
	  //Internal container which stores the data
	  struct Container
	  {
		  U key;
		  T item;
	  };

  public:
	  //Constructor / destructor
	  map_s();
	  map_s(uint32);
	  ~map_s();

	  //Copy operations
	  map_s(const map_s&);
	  map_s& operator= (const map_s&);

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
    bool erase_c(U key);				//Calls Clear() on object before erasing
    void clear();
    void clear_c();
	  void resize(uint32);
	  void extend();
	  void reset();				//Reset data array to size 1.

  private:
	  //Data members
	  Container* data;

	  int array_size;
	  int current_size;

  private:
	  void init(const map_s&);
  };


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::map_s()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map_s<U, T>::map_s()
	  : data(NULL), array_size(1), current_size(0)
  {
	  resize(1);

  }	//End: map_s::map_s()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::map_s()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map_s<U, T>::map_s(uint32 new_size)
	  : data(NULL), array_size(0), current_size(0)
  {
	  resize(new_size);

  }	//End: map_s::map_s()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::~map_s()
  //--------------------------------------------------------------------------------
  //		Destructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map_s<U, T>::~map_s()
  {
	  //Free memory
	  delete[] data;

  }	//End: map_s::~map_s()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::init()
  //--------------------------------------------------------------------------------
  //		Initialise map_s to another.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map_s<U, T>::init(const map_s& other)
  {
	  //Resize lists
	  int sze = (other.current_size>0)?other.current_size:1;
	  resize(sze);

	  for (int i = 0; i < other.current_size; ++i)
	  {
		  data[i] = other.data[i];
	  }

  }	//End: map_s::init()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::map_s()
  //--------------------------------------------------------------------------------
  //		Copy constructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map_s<U, T>::map_s(const map_s& other): 
	  data(NULL), array_size(0), current_size(0)
  {
	  init(other);

  }	//End: map_s::map_s()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::operator=()
  //--------------------------------------------------------------------------------
  //		Assignment
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map_s<U, T>& map_s<U, T>::operator=(const map_s& other)
  {
	  if (this == &other)
		  return *this;
	
	  init(other);

	  return *this;

  }	//End: map_s::operator=()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::resize()
  //--------------------------------------------------------------------------------
  //		Resize map, erases all data before resize.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map_s<U, T>::resize(uint32 val)
  {
	  //Delete old data
	  delete[] data;
	  data = new Container[val];

	  array_size = val;
	  current_size = 0;
	
  }	//End: map_s::resize()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::find()
  //--------------------------------------------------------------------------------
  //		Find a value in the map, uses a binary search algorithm
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map_s<U, T>::find(U key, int& index, int lower) const
  {
	  return find(key, index, lower, (current_size - 1));
	
  }	//End: map_s::find()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::find()
  //--------------------------------------------------------------------------------
  //		Find a value in the map within a range, uses a binary search algorithm
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map_s<U, T>::find(U key, int& index, int lower, int upper) const
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

  }	//End: map_s::find()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::extend()
  //--------------------------------------------------------------------------------
  //		Extend the map by a factor of 1.5, keeps all data.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map_s<U, T>::extend()
  {
	  //Calculate new size (1.5 * array_size + 1)
	  int new_size = ( (array_size * 3) >> 1) + 1;
	
	  //Create new arrays
	  Container* new_data = new Container[new_size];

	  for (int i = 0; i < current_size; ++i)
		  new_data[i] = data[i];

	  delete[] data;
	  data = new_data;
	  array_size = new_size;

  }	//End: map_s::extend()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::insert()
  //--------------------------------------------------------------------------------
  //		Insert an element into the map
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map_s<U, T>::insert(const T& val, U key)
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

  }	//End: map_s::insert()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::erase()
  //--------------------------------------------------------------------------------
  //		Remove an element from the map
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map_s<U, T>::erase(U key)
  {
	  //Find the index
	  int index;
	  if (!find(key, index))
		  return false;	//element not found
	
	  for (int i = index; i < current_size - 1; ++i)
		  data[i] = data[i+1];

	  current_size--;

	  return true;

  }	//End: map_s::erase()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::erase_c()
  //--------------------------------------------------------------------------------
  //		Remove an element from the map, Calls Clear() on target.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map_s<U, T>::erase_c(U key)
  {
      //Find the index
      int index;
      if (!find(key, index))
          return false;	//element not found

      data[index].Clear();

      for (int i = index; i < current_size - 1; ++i)
          data[i] = data[i + 1];

      current_size--;

      return true;

  }	//End: map_s::erase_c()


  //--------------------------------------------------------------------------------
  //	@	map_s<U,T>::set()
  //--------------------------------------------------------------------------------
  //		Sets an element to a new value
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map_s<U, T>::set(U key, const T& val)
  {
	  //Find the index to insert to
	  int index;
	  if (!find(key, index))
		  return false;	//element does not exist

	  data[index].item = val;

	  return true;

  }	//End: map_s::set()


  //--------------------------------------------------------------------------------
  //	@	DgMap<U,T>::reset()
  //--------------------------------------------------------------------------------
  //		Reset size to 1
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map_s<U, T>::reset()
  {
	  resize(1);

  }	//End: map_s::reset()


  //--------------------------------------------------------------------------------
  //	@	DgMap<U,T>::clear()
  //--------------------------------------------------------------------------------
  //		Set the number of elements to zero
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map_s<U, T>::clear()
  {
	  current_size = 0;

  }	//End: map_s::clear()


  //--------------------------------------------------------------------------------
  //	@	DgMap<U,T>::clear_c()
  //--------------------------------------------------------------------------------
  //		Set the number of elements to zero, Calls Clear() on all active elements.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map_s<U, T>::clear_c()
  {
      for (int i = 0; i < current_size; ++i)
          data[i].Clear();

      current_size = 0;

  }	//End: map_s::clear_c()

};

#endif