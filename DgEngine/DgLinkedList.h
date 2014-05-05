//================================================================================
// @ DgLinkedList.h
// 
// Class: DgLinkedList
//
// Pre-allocated Linked list. Similar to std::list with similarly named methods
// and functionality. The underlying arrays are preallocated and only change in
// size if extending list past that allocated, or manually resizing. This makes
// for fast insertion/erasing of elements.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DGLINKEDLIST
#define DGLINKEDLIST

#include <assert.h>
typedef unsigned int uint32;

//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>:	T: data type
//--------------------------------------------------------------------------------
template<class T>
class DgLinkedList
{
private:

	//--------------------------------------------------------------------------------
	//		Container to hold the object and pointers
	//--------------------------------------------------------------------------------
	struct data_container
	{
		data_container(): next(NULL), previous(NULL) {}
		
		data_container* next;
		data_container* previous;
		T element;
	};

public:

	//--------------------------------------------------------------------------------
	//		Constant Iterator
	//--------------------------------------------------------------------------------
	class const_iterator
	{
	private:
		friend class DgLinkedList;
		
	private:
		//Special constructor, not for external use
		const_iterator(const data_container* _ptr) {ptr = _ptr;}

	public:
		//Constructor / destructor
		const_iterator(){}
		~const_iterator(){}

		//Copy operations
		const_iterator(const const_iterator& it): ptr(it.ptr){}
		const_iterator& operator= (const const_iterator&);
		
		//Comparison
		bool operator==(const const_iterator& it) const {return ptr == it.ptr;}
		bool operator!=(const const_iterator& it) const {return ptr != it.ptr;}

		//operators
		const_iterator& operator++();
		const_iterator operator++(int);
		const_iterator& operator--();
		const_iterator operator--(int);

		//Conversion
		const T* operator->() const {return &(ptr->element);}
		const T& operator*() const {return ptr->element;}

	private:
		//Data members
		const data_container *ptr;

	};


	//--------------------------------------------------------------------------------
	//		Iterator
	//--------------------------------------------------------------------------------
	class iterator
	{
		friend class DgLinkedList;

	private:
		//Special constructor, not for external use
		iterator(data_container* _ptr) {ptr = _ptr;}

	public:
		//Constructor / destructor
		iterator(){}
		~iterator(){}

		//Copy operations
		iterator(const iterator& it): ptr(it.ptr){}
		iterator& operator= (const iterator&);

		//Comparison
		bool operator==(const iterator& it) const {return ptr == it.ptr;}
		bool operator!=(const iterator& it) const {return ptr != it.ptr;}

		//operators
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);

		//Conversion
		operator const_iterator() const {return const_iterator(ptr);}
		T* operator->() const {return &(ptr->element);}
		T& operator*() const {return ptr->element;}

	private:
		//Data members
		data_container *ptr;

	};


public:
	//Constructor / destructor
	DgLinkedList();
	DgLinkedList(uint32);
	~DgLinkedList();

	//Copy operations
	DgLinkedList(const DgLinkedList&);
	DgLinkedList& operator= (const DgLinkedList&);

	//Accessors
	inline iterator			begin()		const {return iterator(root_container.next);}
    inline iterator			end()		const { return iterator(const_cast<data_container*>(&end_container)); }
	inline const_iterator	cbegin()	const {return const_iterator(root_container.next);}
    inline const_iterator	cend()		const { return const_iterator(const_cast<data_container*>(&end_container)); }
	inline uint32			size()		const {return current_size;}
	inline uint32 			max_size()	const {return maximum_size;}
	inline bool 			empty()		const {return current_size == 0;}
	inline T&				back()		const {return end_container.previous->element;}
	inline T&				front()		const {return root_container.next->element;}

	//Manipulators
	void push_back(const T&);
	bool push_back_blank();
    void push_front(const T&);
    bool push_front_blank();
	void insert(const iterator&, const T&);
	void push_back_noresize(const T&);
	void push_front_noresize(const T&);
	void insert_noresize(const iterator&, const T&);
	void pop_back();
	void pop_front();
	void erase(iterator&);
	void clear();
	void resize(uint32);
	void extend();

private:
	//Data members

	//Pre-allocated block of memory to hold elements
	data_container *data;

	//Root and end objects, and pointers
	data_container root_container;
	data_container end_container;

	//Next free element in the list;
	data_container* next_free;		

	//Sizes
	uint32 current_size;
	uint32 maximum_size;

private:
	//Functions
	void init(uint32 new_size);

};


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::iterator::operator=()
//--------------------------------------------------------------------------------
//		iterator Assignment
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::iterator& DgLinkedList<T>::iterator::operator=
	(const typename DgLinkedList<T>::iterator& other)
{
	ptr = other.ptr;

	return *this;
}	//End:: DgLinkedList<T>::iterator::operator=()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::iterator::operator++()
//--------------------------------------------------------------------------------
//		iterator pre increment
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::iterator& DgLinkedList<T>::iterator::operator++()
{
	ptr = ptr->next;

	return *this;
}	//End: DgLinkedList<T>::iterator::operator++()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::iterator::operator++()
//--------------------------------------------------------------------------------
//		iterator post increment
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::iterator DgLinkedList<T>::iterator::operator++(int)
{
	iterator result(*this);	// make a copy for result
    ++(*this);              // Now use the prefix version to do the work
    return result;			// return the copy (the old) value.

}	//End: DgLinkedList<T>::iterator::operator++()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::iterator::operator--()
//--------------------------------------------------------------------------------
//		iterator pre decrement
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::iterator& DgLinkedList<T>::iterator::operator--()
{
	ptr = ptr->previous;

	return *this;

}	//End: DgLinkedList<T>::iterator::operator++()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::iterator::operator--()
//--------------------------------------------------------------------------------
//		iterator post decrement
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::iterator DgLinkedList<T>::iterator::operator--(int)
{
	iterator result(*this);	// make a copy for result
    --(*this);              // Now use the prefix version to do the work
    return result;			// return the copy (the old) value.

}	//End: DgLinkedList<T>::iterator::operator--()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::const_iterator::operator=()
//--------------------------------------------------------------------------------
//		const_iterator Assignment
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::const_iterator& DgLinkedList<T>::const_iterator::operator=
	(const typename DgLinkedList<T>::const_iterator& other)
{
	ptr = other.ptr;

	return *this;

}	//End::DgLinkedList<T>::const_iterator::operator=()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::const_iterator::operator++()
//--------------------------------------------------------------------------------
//		const_iterator pre increment
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::const_iterator& DgLinkedList<T>::const_iterator::operator++()
{
	ptr = ptr->next;

	return *this;

}	//End: DgLinkedList<T>::const_iterator::operator++()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::const_iterator::operator=()
//--------------------------------------------------------------------------------
//		const_iterator post increment
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::const_iterator DgLinkedList<T>::const_iterator::operator++(int)
{
	const_iterator result(*this);	// make a copy for result
    ++(*this);              // Now use the prefix version to do the work
    return result;			// return the copy (the old) value.

}	//End: DgLinkedList<T>::const_iterator::operator++()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::const_iterator::operator--()
//--------------------------------------------------------------------------------
//		const_iterator pre decrement
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::const_iterator& DgLinkedList<T>::const_iterator::operator--()
{
	ptr = ptr->previous;

	return *this;

}	//End: DgLinkedList<T>::const_iterator::operator--()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::const_iterator::operator--()
//--------------------------------------------------------------------------------
//		const_iterator post decrement
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::const_iterator DgLinkedList<T>::const_iterator::operator--(int)
{
	const_iterator result(*this);	// make a copy for result
    --(*this);              // Now use the prefix version to do the work
    return result;			// return the copy (the old) value.

}	//End: DgLinkedList<T>::const_iterator::operator--()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::init()
//--------------------------------------------------------------------------------
//		General initialise function
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::init(uint32 new_size)
{
	//Assign sizes
	maximum_size = new_size;
	current_size = 0;

	//Initialise data
	data = new data_container[maximum_size];
	next_free = &data[0];

	//Set outer container pointers
    root_container.next = &end_container;
    end_container.previous = &root_container;

	//Only need to assign forward pointers
	for (uint32 i = 0; i < maximum_size-1; i++)
	{
		data[i].next = &data[i+1];
	}

}	//End: DgLinkedList::init()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::DgLinkedList<T>()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
DgLinkedList<T>::DgLinkedList() : data(NULL), next_free(NULL)
{
	//Set data
	init(1);

}	//End: DgLinkedList::DgLingedList()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::DgLinkedList<T>()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
DgLinkedList<T>::DgLinkedList(uint32 new_size): data(NULL), next_free(NULL)
{
	//Size must be at least 1
	assert(new_size > 0);

	//Set up the list
	init(new_size);

}	//End: DgLinkedList::DgLingedList()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::~DgLinkedList<T>()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
template<class T>
DgLinkedList<T>::~DgLinkedList()
{
	//Free data
	delete[] data;

}	//End: DgLinkedList::~DgLingedList()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::DgLinkedList<T>()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<class T>
DgLinkedList<T>::DgLinkedList(const DgLinkedList& other)
{
	//Initialise data
	init(other.maximum_size);

	//Assign data
	DgLinkedList<T>::const_iterator it = other.begin();
	for (it; it != other.end(); ++it)
	{
		push_back(*it);
	}

}	//End: DgLinkedList::DgLinkedList()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::operator()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class T>
DgLinkedList<T>& DgLinkedList<T>::operator=(const DgLinkedList& other)
{
	if (this == &other)
		return *this;

	//resize array
	resize(other.maximum_size);

	//Assign data
	DgLinkedList<T>::const_iterator it = other.begin();
	for (it; it != other.end(); ++it)
	{
		push_back(*it);
	}

	return *this;
}	//End: DgLinkedList::operator=()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::clear()
//--------------------------------------------------------------------------------
//		Clear the list, retains allocated memory.
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::clear()
{
	//Reset next free
	next_free = &data[0];

	//Set outer container pointers
    root_container.next = &end_container;
    end_container.previous = &root_container;

	//close the last element in the list
	data[maximum_size-1].next = NULL;

	//Assign pointers
	for (uint32 i = 0; i < maximum_size-1; i++)
	{
		data[i].next = &data[i+1];
	}

	current_size = 0;

}	//End: DgLinkedList<T>::clear()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::resize()
//--------------------------------------------------------------------------------
//		Resize the list, wipes all data.
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::resize(uint32 new_size)
{
	//Size must be at least 1
	assert(new_size > 0);

	delete[] data;
	data = NULL;

	//Initialise data
	init(new_size);

}	//End: DgLinkedList<T>::clear()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::push_back()
//--------------------------------------------------------------------------------
//		Add an element to the back of the list
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::push_back(const T& val)
{
	//Is the list full?
	if (current_size == maximum_size)
		extend();

	//Get the list node to work on
	data_container *new_element = next_free;
	
	//Move next_free pointer to the next data_container
	next_free = next_free->next;

	//Assign the element
	new_element->element = val;

	//Add the current element to the back of the active list
	end_container.previous->next = new_element;
	new_element->previous = end_container.previous;
    new_element->next = &end_container;
	end_container.previous = new_element;

	//Increment current_size
	current_size++;

}	//End: DgLinkedList::push_back()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::push_back_blank()
//--------------------------------------------------------------------------------
//		Add an element to the back of the list, but does not assign, nor
//		resize the array.
//--------------------------------------------------------------------------------
template<class T>
bool DgLinkedList<T>::push_back_blank()
{
	//Is the list full?
	if (current_size == maximum_size)
		return false;

	//Get the list node to work on
	data_container *new_element = next_free;

	//Move next_free pointer to the next data_container
	next_free = next_free->next;

	//Add the current element to the back of the active list
	end_container.previous->next = new_element;
	new_element->previous = end_container.previous;
    new_element->next = &end_container;
	end_container.previous = new_element;

	//Increment current_size
	current_size++;

	return true;

}	//End: DgLinkedList::push_back_blank()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::push_front_blank()
//--------------------------------------------------------------------------------
//		Add an element to the front of the list, but does not assign, nor
//		resize the array.
//--------------------------------------------------------------------------------
template<class T>
bool DgLinkedList<T>::push_front_blank()
{
    //Is the list full?
    if (current_size == maximum_size)
        return false;

    //Get the list node to work on
    data_container *new_element = next_free;

    //Move next_free pointer to the next data_container
    next_free = next_free->next;

    //Add the current element to the back of the active list
    root_container.next->previous = new_element;
    new_element->previous = &root_container;
    new_element->next = root_container.next;
    root_container.next = new_element;

    //Increment current_size
    current_size++;

    return true;

}	//End: DgLinkedList::push_back_blank()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::push_back_noresize()
//--------------------------------------------------------------------------------
//		Add an element to the back of the list. Return if full
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::push_back_noresize(const T& val)
{
	//Is the list full?
	if (current_size < maximum_size)
		push_back(val);

}	//End: DgLinkedList::push_back_noresize()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::push_front()
//--------------------------------------------------------------------------------
//		Add an element to the front of the list
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::push_front(const T& val)
{
	//Is the list full?
	if (current_size == maximum_size)
		extend();

	//Get the list node to work on
	data_container *new_element = next_free;
	
	//Move next_free pointer to the next data_container
	next_free = next_free->next;

	//Assign the element
	new_element->element = val;

	//Add the current element to the back of the active list
	root_container.next->previous = new_element;
	new_element->previous = &root_container;
	new_element->next = root_container.next;
	root_container.next = new_element;

	//Increment current_size
	current_size++;

}	//End: DgLinkedList::push_front()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::push_front_noresize()
//--------------------------------------------------------------------------------
//		Add an element to the front of the list. Return if full
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::push_front_noresize(const T& val)
{
	//Is the list full?
	if (current_size < maximum_size)
		push_front(val);

}	//End: DgLinkedList::push_front_noresize()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::pop_back()
//--------------------------------------------------------------------------------
//		Erase last element
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::pop_back()
{
	//Range check
	assert(current_size != 0);
	
	//Get new last element
    data_container* last = end_container.previous->previous;

	//Assign next free
	end_container.previous->next = next_free;
	next_free = end_container.previous;

	//Break element from chain
    last->next = &end_container;			//prev points to next
	end_container.previous = last;	//next points to previous

	//Deincrement current_size
	current_size--;

}	//End: DgLinkedList::pop_back()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::pop_front()
//--------------------------------------------------------------------------------
//		Erase first element
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::pop_front()
{
	//Range check
	assert(current_size != 0);
	
	//Get new first element
    data_container* first = root_container.next->next;

	//Assign next free
	root_container.next->next = next_free;
	next_free = root_container.next;

	//Break element from chain
    first->previous = &root_container;		//prev points to next
	root_container.next = first;	//next points to previous

	//Deincrement current_size
	current_size--;

}	//End: DgLinkedList::pop_front()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::insert()
//--------------------------------------------------------------------------------
//		Add an element to the list at position. 
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::insert(const iterator& it, const T& val)
{
    assert(it.ptr != &root_container);

	//Is the list full?
	if (current_size == maximum_size)
		extend();
	
	//Get the list node to work on
	data_container *new_element = next_free;
	
	//Move next_free pointer to the next data_container
	next_free = next_free->next;

	//Insert next free
	it.ptr->previous->next = new_element;
	new_element->previous = it.ptr->previous;
	it.ptr->previous = new_element;
	new_element->next = it.ptr;

	//Set the element
	new_element->element = val;

	//Increment current_size
	current_size++;

}	//End: DgLinkedList::insert()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::insert_noresize()
//--------------------------------------------------------------------------------
//		Add an element to the list at position. 
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::insert_noresize(const iterator& it, const T& val)
{
	//Is the list full?
	if (current_size < maximum_size)
		insert(it, val);

}	//End: DgLinkedList::insert_noresize()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::erase()
//--------------------------------------------------------------------------------
//		Erase an element from the list
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::erase(iterator& it)
{
	//Remember previous element
	data_container* next = it.ptr->next;

	//Break element from chain
	it.ptr->previous->next = it.ptr->next;		//prev points to next
	it.ptr->next->previous = it.ptr->previous;	//next points to previous

	//Add this broken item to the begining of the free list
	it.ptr->next = next_free;				//put item in between next_free and end of the list
	
	//Reset next_free
	next_free = it.ptr;
	
	//Deincrement current_size
	current_size--;

	//Return iterator to the next container
	it = iterator(next);

}	//End: DgLinkedList::erase()


//--------------------------------------------------------------------------------
//	@	DgLinkedList<T>::extend()
//--------------------------------------------------------------------------------
//		Increases the size of the underlying arrays by a factor of 1.5
//--------------------------------------------------------------------------------
template<class T>
void DgLinkedList<T>::extend()
{
	//Calculate new size
	uint32 new_size = ( (maximum_size * 3) >> 1) + 1;
	
	//Create new array
	data_container* new_data = new data_container[new_size];

	//Assign pointers
	for (uint32 i = 0; i < new_size-1; i++)
	{
		new_data[i].next = &new_data[i+1];
	}
	for (uint32 i = 1; i < current_size; i++)
	{
		new_data[i].previous = &new_data[i-1];
	}

	//Assign values
	iterator it = begin();
	for (uint32 i = 0; it != end(); ++it, ++i)
	{
		new_data[i].element = *it;
	}

	//Assign data pointer
	delete[maximum_size] data;
	data = new_data;
	
	//Assign next free pointer
	next_free = &new_data[current_size];
	
	//Adjust sizes
	maximum_size = new_size;

	//Determine root and end pointers
	if (current_size == 0)
	{
        root_container.next = &end_container;
        end_container.previous = &root_container;
	}
	else
	{
		root_container.next = &data[0];
		end_container.previous = &data[current_size-1];
        new_data[0].previous = &root_container;
        new_data[current_size - 1].next = &end_container;
	}
	
}	//End: DgLinkedList<T>::extend()


//--------------------------------------------------------------------------------
//		Helpful functions
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	find()
//--------------------------------------------------------------------------------
//		Find a value in the list, returns iterator
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::iterator find (
	typename DgLinkedList<T>::iterator first, 
	typename DgLinkedList<T>::iterator last, 
	const T& val)
{
  while (first!=last) 
  {
    if (*first==val) 
		return first;

    ++first;
  }

  return last;

}	//End find()


//--------------------------------------------------------------------------------
//	@	find()
//--------------------------------------------------------------------------------
//		Find a value in the list, returns const_iterator
//--------------------------------------------------------------------------------
template<class T>
typename DgLinkedList<T>::const_iterator find (
	typename DgLinkedList<T>::const_iterator first, 
	typename DgLinkedList<T>::const_iterator last, 
	const T& val)
{
  while (first!=last) 
  {
    if (*first==val) 
		return first;

    ++first;
  }

  return last;

}	//End find()

#endif