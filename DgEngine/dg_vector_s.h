/*!
* @file vector_s.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: vector_s<>
*/

#ifndef DG_VECTOR_S_H
#define DG_VECTOR_S_H

#include <assert.h>
#define NULL 0

//--------------------------------------------------------------------------------
//	@	vector_s<T>:	T: data
//--------------------------------------------------------------------------------
/*!
* @ingroup utility_container
*
* @class vector_s
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
* Instead, vector_s containers may allocate some extra storage to accommodate for
* possible growth, and thus the container may have an actual capacity greater than
* the storage strictly needed to contain its elements (i.e., its size).
*
*
* @author Frank Hart
* @date 7/01/2014
*/

typedef unsigned int uint32;

namespace Dg
{
  template<class T>
  class vector_s
  {
  public:
    //Constructor / destructor
    vector_s();

    //! Construct with a set size
    vector_s(uint32);
    ~vector_s();

    vector_s(const vector_s&);
    vector_s& operator= (const vector_s&);

    //! Copy both the current elements and the elements in the reserved memory.
    void CopyAll(const vector_s& other);

    //! Access element
    T& operator[](uint32 i)				{ return data[i]; }

    //! Accessor, no range check.
    const T& operator[](uint32 i) const	{ return data[i]; }

    //! Get last element
    /// Calling this function on an empty container causes undefined behavior.
    T& back() { return data[array_size - 1]; }

    //! Get last element
    /// Calling this function on an empty container causes undefined behavior.
    const T& back() const { return data[array_size - 1]; }

    //! Accessor with range check.
    T& at(uint32);

    //! Accessor with range check.
    const T& at(uint32) const;

    //! Current size of the array
    uint32 size()		const			{ return current_size; }

    //! Is the array empty
    bool empty()		const			{ return current_size == 0; }

    //! Size of the reserved memory.
    uint32 max_size()	const			{ return array_size; }

    //! Get pointer to first element.
    T* Data()							{ return data; }

    //! Get pointer to first element.
    const T* Data()		const			{ return data; }

    //! Add element to the back of the array.
    void push_back(const T&);

    //! Remove element from the back of the array.
    void pop_back();

    //! Add element to the back of the array.
    void push_front(const T&);

    //! Remove element from the back of the array.
    void pop_front();

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
    void init(const vector_s&);
  };


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::vector_s()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_s<T>::vector_s() : data(NULL), array_size(1), current_size(0)
  {
    data = new T[array_size];
  }	//End: vector_s::vector_s()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::vector_s()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_s<T>::vector_s(uint32 new_size) : data(NULL), current_size(0), array_size(new_size)
  {
    array_size = (array_size == 0) ? 1 : array_size;

    //Initialise pointers
    data = new T[array_size];

  }	//End: vector_s::vector_s()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::~vector_s()
  //--------------------------------------------------------------------------------
  //		Destructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_s<T>::~vector_s()
  {
    //Free memory
    delete[] data;

  }	//End: vector_s::~vector_s()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::init()
  //--------------------------------------------------------------------------------
  //		Initialise array to another vector_s
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::init(const vector_s& other)
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
  }	//End: vector_s<T>::init()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::vector_s()
  //--------------------------------------------------------------------------------
  //		Copy constructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_s<T>::vector_s(const vector_s& other) : data(NULL)
  {
    init(other);

  }	//End: vector_s::vector_s()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::operator=()
  //--------------------------------------------------------------------------------
  //		Assignment
  //--------------------------------------------------------------------------------
  template<class T>
  vector_s<T>& vector_s<T>::operator=(const vector_s& other)
  {
    if (this == &other)
      return *this;

    init(other);

    return *this;
  }	//End: vector_s::operator=()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::CopyAll()
  //--------------------------------------------------------------------------------
  //		Copies entire array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::CopyAll(const vector_s<T>& other)
  {
    if (array_size != other.array_size)
      resize(other.array_size);

    //Copy contents
    for (uint32 i = 0; i < array_size; i++)
    {
      data[i] = other.data[i];
    }

  }	//End: vector_s<T>::CopyAll()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::at()
  //--------------------------------------------------------------------------------
  //		Accessor with range check
  //--------------------------------------------------------------------------------
  template<class T>
  T& vector_s<T>::at(uint32 index)
  {
    if (index >= current_size)
      throw std::out_of_range("vector_s: range error");

    return data[index];

  }	//End: vector_s<T>::at()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::at()
  //--------------------------------------------------------------------------------
  //		const accessor with range check
  //--------------------------------------------------------------------------------
  template<class T>
  const T& vector_s<T>::at(uint32 index) const
  {
    if (index >= current_size)
      throw std::out_of_range("vector_s: range error");

    return data[index];

  }	//End: vector_s<T>::at()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::push_back()
  //--------------------------------------------------------------------------------
  //		Add element to the back of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::push_back(const T& val)
  {
    //Range check
    if (current_size == array_size)
    {
      extend();
    }

    //Set element
    data[current_size] = val;

    //increment current size
    ++current_size;

  }	//End: vector_s<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::pop_back
  //--------------------------------------------------------------------------------
  //		Pop an element from the back of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::pop_back()
  {
    //Range check
    if (current_size == 0)
      return;

    //Deincrement current size
    --current_size;

  }	//End: vector_s<T>::pop_back()



  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::push_front()
  //--------------------------------------------------------------------------------
  //		Add element to the front of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::push_front(const T& val)
  {
    //Range check
    if (current_size == array_size)
    {
      extend();
    }

    for (uint32 i = current_size; i >= 0; --i)
      data[i + 1] = data[i];

    //Set element
    data[0] = val;

    //increment current size
    ++current_size;

  }	//End: vector_s<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::pop_front
  //--------------------------------------------------------------------------------
  //		Pop an element from the front of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::pop_front()
  {
    //Range check
    if (current_size == 0)
      return;

    for (uint32 i = 0; i < current_size - 1; ++i)
      data[i] = data[i + 1];

    //Deincrement current size
    --current_size;

  }	//End: vector_s<T>::pop_front()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::clear()
  //--------------------------------------------------------------------------------
  //		Clear array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::clear()
  {
    //Set current size to 0
    current_size = 0;

  }	//End: vector_s::clear()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::resize()
  //--------------------------------------------------------------------------------
  //		Resize array, erases all data before resize.
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::resize(uint32 new_size)
  {
    //Delete current array
    delete[] data;

    //Set sizes
    array_size = (new_size == 0) ? 1 : new_size;
    current_size = 0;

    //Set data
    data = new T[array_size];

  }	//End: vector_s::resize()


  //--------------------------------------------------------------------------------
  //	@	vector_s<T>::extend()
  //--------------------------------------------------------------------------------
  //		Extend the array by a factor of 1.5, keeps all data.
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_s<T>::extend()
  {
    //Calculate new size 
    uint32 new_size = array_size * 2;

    //Create new arrays
    T* new_data = new T[new_size];

    //Copy data
    for (uint32 i = 0; i < current_size; ++i)
      new_data[i] = data[i];

    //Assign new values
    delete[] data;
    data = new_data;
    array_size = new_size;

  }	//End: vector_s::extend()



  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  //	@	find()
  //--------------------------------------------------------------------------------
  //		Find a value in the list, returns reference
  //--------------------------------------------------------------------------------
  template<class T>
  T* find(vector_s<T>& container, const T& val)
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
  void fill(vector_s<T>& container, const T& val)
  {
    for (uint32 i = 0; i < container.size(); ++i)
    {
      container[i] = val;
    }

  }	//End: fill()

};
#endif