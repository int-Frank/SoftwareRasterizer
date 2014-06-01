//================================================================================
// @ map.h
// 
// Class: map
//
// Ordered mapped list. Objects are inserted (in order) with some key. 
// Similar to a DgMap however data dynamically allocated on the heap. 
// Only unique keys are allowed.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DG_MAP_H
#define DG_MAP_H

#include "DgTypes.h"


namespace Dg
{
  //--------------------------------------------------------------------------------
  //	@	map<U, T>:	U: key
  //						T: data
  //--------------------------------------------------------------------------------
  template<class U, class T>
  class map
  {
    //Internal container which stores the data
    struct Container
    {
      Container() : ptr(NULL) {}
      ~Container() {} //Don't delete pointer
      U key;
      T* ptr;
    };

  public:
    //Constructor / destructor
    map();
    ~map();

    //Copy operations
    map(const map&);
    map& operator= (const map&);

    //Accessors

    //Get ptr at position
    T& operator[](uint32 i)				{ return *data[i].ptr; }
    const T& operator[](uint32 i) const { return *data[i].ptr; }
    uint32 size()			const			{ return current_size; }
    bool empty()		const			{ return current_size == 0; }
    uint32 max_size()		const			{ return array_size; }
    U key(uint32 i)			const			{ return data[i].key; }

    //Returns false if not found, however, index will be set to
    //the lower index just before where input key would be.
    bool find(U key, uint32& index) const;			//Uses binary search

    //Manipulators
    uint32 insert(U key, const T&);		//Use find
    bool erase(U key);					//Use find
    void clear();
    void extend();
    void reset();				//Reset data array to size 1.

  private:
    //Data members
    Container* data;

    uint32 array_size;
    uint32 current_size;

  private:
    void init(const map&);
  };


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::map()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map<U, T>::map()
  : data(NULL), array_size(0), current_size(0)
  {
  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::~map()
  //--------------------------------------------------------------------------------
  //		Destructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map<U, T>::~map()
  {
    //Free memory
    for (uint32 i = 0; i < current_size; ++i)
    {
      delete data[i].ptr;
    }

    delete[] data;

  }	//End: map::~map()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::init()
  //--------------------------------------------------------------------------------
  //		Initialise map to another.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map<U, T>::init(const map& other)
  {
    //Delete all items
    for (uint32 i = 0; i < current_size; ++i)
    {
      delete data[i].ptr;
    }

    delete[] data;
    data = NULL;

    current_size = other.current_size;
    array_size = current_size;

    data = new Container[current_size];

    for (uint32 i = 0; i < current_size; ++i)
    {
      data[i].key = other.data[i].key;
      data[i].ptr = new T(*other.data[i].ptr);
    }

  }	//End: map::init()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::map()
  //--------------------------------------------------------------------------------
  //		Copy constructor
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map<U, T>::map(const map& other) :
  data(NULL), array_size(0), current_size(0)
  {
    init(other);

  }	//End: map::map()

  //--------------------------------------------------------------------------------
  //	@	map<U,T>::operator=()
  //--------------------------------------------------------------------------------
  //		Assignment
  //--------------------------------------------------------------------------------
  template<class U, class T>
  map<U, T>& map<U, T>::operator=(const map& other)
  {
    if (this == &other)
      return *this;

    init(other);

    return *this;

  }	//End: map::operator=()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::find()
  //--------------------------------------------------------------------------------
  //		Find a value in the map within a range, uses a binary search algorithm
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map<U, T>::find(U key, uint32& index) const
  {
    uint32 lower(1);
    uint32 upper(current_size);

    while (lower <= upper)
    {
      // calculate the midpoint for roughly equal partition
      index = ((upper + lower) >> 1) - 1;

      // determine which subarray to search
      if (data[index].key < key)
        // change min index to search upper subarray
        lower = index + 2;
      else if (data[index].key > key)
        // change max index to search lower subarray
        upper = index;
      else
        // key found at index index
        return true;
    }

    //Set index closest (but lower) to key
    index = lower - 1;
    return false;

  }	//End: map::find()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::extend()
  //--------------------------------------------------------------------------------
  //		Extend the map by a factor of 1.5, keeps all data.
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map<U, T>::extend()
  {
    //Calculate new size
    int new_size = array_size * 2;
    if (new_size == 0)
      new_size = 1;

    //Create new arrays
    Container* new_data = new Container[new_size];

    for (uint32 i = 0; i < current_size; ++i)
    {
      new_data[i].key = data[i].key;
      new_data[i].ptr = data[i].ptr;
    }

    delete[] data;
    data = new_data;
    array_size = new_size;

  }	//End: map::extend()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::insert()
  //--------------------------------------------------------------------------------
  //		Insert an element into the map
  //--------------------------------------------------------------------------------
  template<class U, class T>
  uint32 map<U, T>::insert(U key, const T& val)
  {
    //Find the index to insert to
    uint32 index;
    if (find(key, index))
      return index;	//element already exists

    //Range check
    if (current_size == array_size)
      extend();

    //shift all RHS objects to the right by one.
    for (uint32 i = current_size; i > index; --i)
    {
      data[i].key = data[i - 1].key;
      data[i].ptr = data[i - 1].ptr;
    }

    data[index].key = key;
    data[index].ptr = new T(val);

    current_size++;

    return index;

  }	//End: map::insert()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::erase()
  //--------------------------------------------------------------------------------
  //		Remove an element from the map
  //--------------------------------------------------------------------------------
  template<class U, class T>
  bool map<U, T>::erase(U key)
  {
    //Find the index
    uint32 index;
    if (!find(key, index))
      return false;	//element not found

    delete data[index].ptr;

    for (uint32 i = index; i < current_size - 1; ++i)
      data[i] = data[i + 1];

    data[current_size].ptr = NULL;

    current_size--;

    return true;

  }	//End: map::erase()


  //--------------------------------------------------------------------------------
  //	@	DgMap<U,T>::reset()
  //--------------------------------------------------------------------------------
  //		Reset size to 1
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map<U, T>::reset()
  {
    resize(1);

  }	//End: map::reset()


  //--------------------------------------------------------------------------------
  //	@	DgMap<U,T>::clear()
  //--------------------------------------------------------------------------------
  //		Set the number of elements to zero
  //--------------------------------------------------------------------------------
  template<class U, class T>
  void map<U, T>::clear()
  {
    for (uint32 i = 0; i < current_size; ++i)
    {
      delete data[i].ptr;
      data[i].ptr = NULL;
    }
    current_size = 0;

  }	//End: map::clear()

};
#endif