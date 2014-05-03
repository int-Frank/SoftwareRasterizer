/*!
* @file BaseWrapper.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: BaseWrapper<T>
*/

#ifndef BASEWRAPPER_H
#define BASEWRAPPER_H


/*!
 * @ingroup utility_container
 *
 * @class BaseWrapper
 *
 * @brief Wraps base pointers for automatic deep copying and deletion.
 *
 * Type T should be type BaseClass<T>. If not, the type should 
 * contain the method clone().
 *
 * @author Frank Hart
 * @date 2/01/2014
 */
template<class T>
class BaseWrapper
{
public:
	/*!
	 * @brief Build internal resource from input.
	 *
	 * @pre input type must contain the clone() method.
	 * @post Pointer to new resource stored.
	 */
	BaseWrapper(const T&);

	/*!
	 * @brief Build a new object on the heap from a pointer.
	 *
	 * @param[in] t Pointer to the object to clone.
	 * @param[in] dealloc Flag to determine if the input resource 't' should be freed.
	 *
	 * @pre 't' must be a valid pointer to an object contained the clone() method.
	 * @post Pointer to new resource stored.
	 */
	BaseWrapper(T* t, bool dealloc = true);

	/*!
	 * @brief Internal pointer set to NULL
	 */
	BaseWrapper(): ptr(NULL){}


	BaseWrapper(const BaseWrapper& other);
	BaseWrapper& BaseWrapper::operator=(const BaseWrapper& other);


	/*!
	 * @brief Internal resource cleared
	 */
	~BaseWrapper();

	/*!
	* @brief Clears current resource and build a new from input.
	*/
	void Set(const T&);

	/*!
	* @brief Clears current resource and build a new from input.
	*
	* @param[in] t Pointer to the object to clone.
	* @param[in] dealloc Flag to determine if the input resource 't' should be cleared.
	*
	* @pre 't' must be a valid pointer to an object containing the clone() method.
	* @post New derived class created, reference stored.
	*/
	void Set(const T* t, bool dealloc = true);

	/*!
	* @brief Get pointer to resource.
	* @return Pointer to the internal resource.
	*/
	operator T*()		      {return ptr;}

	/*!
	* @brief Get pointer to resource.
	* @return Pointer to the internal resource.
	*/
	operator const T*() const {return ptr;}

	/*!
	* @brief Get pointer to resource.
	* @return Pointer to the internal resource.
	*/
	T* operator->()			  {return ptr;}


private:
	//Data members
	T* ptr;

};


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::BaseWrapper()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
BaseWrapper<T>::BaseWrapper(const T& t)
{
	ptr = t.clone();

}	//End: BaseWrapper::BaseWrapper()


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::BaseWrapper()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
template<class T>
BaseWrapper<T>::BaseWrapper(T* t, bool dealloc)
{
	ptr = t->clone();

	if (dealloc)
		delete t;

}	//End: BaseWrapper:BaseWrapper()


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::~BaseWrapper()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
template<class T>
BaseWrapper<T>::~BaseWrapper()
{
	delete ptr;

}	//End: BaseWrapper::~BaseWrapper()


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::BaseWrapper()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
template<class T>
BaseWrapper<T>::BaseWrapper(const BaseWrapper& other)
{
	ptr = other.ptr->clone();

}	//End: BaseWrapper::BaseWrapper()


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class T>
BaseWrapper<T>& BaseWrapper<T>::operator=(const BaseWrapper& other)
{
	delete ptr;

	if (other.ptr != NULL)
	{
		T* temp_ptr(other.ptr->clone());
		ptr = temp_ptr;
	}

	return *this;

}	//End: BaseWrapper::operator=()


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::Set()
//--------------------------------------------------------------------------------
//		Set data from other
//--------------------------------------------------------------------------------
template<class T>
void BaseWrapper<T>::Set(const T& t)
{
	//Free memory
	delete ptr;

	//Assign new object
	ptr = t.clone();

}	//End: BaseWrapper:Set()


//--------------------------------------------------------------------------------
//	@	BaseWrapper<T>::Set()
//--------------------------------------------------------------------------------
//		Set from other. Frees memory of original pointer if dealloc is true.
//--------------------------------------------------------------------------------
template<class T>
void BaseWrapper<T>::Set(const T* t, bool dealloc)
{
	//Free memory
	delete ptr;

	//Assign new object
	ptr = t->clone();

	if (dealloc)
		delete t;

}	//End: BaseWrapper:Set()

#endif