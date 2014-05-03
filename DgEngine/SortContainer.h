/*!
* @file SortContainer.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: SortContainer<>
*/
#ifndef SORTCONTAINER_H
#define SORTCONTAINER_H


/*!
 * @ingroup utility_container
 *
 * @struct SortContainer
 *
 * @brief Couples a pointer to an object and a value used for sorting.
 *
 * Sorting arrays can sometimes be inefficient due to expensive copy operations.
 * This class attempts to circumvent this by using a pointer
 * to the actual data. The original data is not moved during the sort, only 
 * the pointer.
 *
 * @tparam T Data type
 * @tparam U Sort value type
 *
 * @author Frank Hart
 * @date 2/01/2014
 */
template<class T, class U>
struct SortContainer
{
	//Constructor/destructor
	SortContainer(): ptr(NULL) {}
	SortContainer(T& tptr, U uval): ptr(&tptr), val(uval) {}
	~SortContainer() {}

	//Copy operations
	SortContainer(const SortContainer<T,U>& other):
		ptr(other.ptr), val(other.val) {}
	SortContainer& operator=(const SortContainer&);

	//comparison
	inline bool operator== (const SortContainer<T,U>& other)
	{return val == other.val;}
	inline bool operator!= (const SortContainer<T,U>& other)
	{return val != other.val;}

	inline bool operator> (const SortContainer<T,U>& other)
	{return val > other.val;}
	inline bool operator< (const SortContainer<T,U>& other)
	{return val < other.val;}

	inline bool operator>= (const SortContainer<T,U>& other)
	{return val >= other.val;}
	inline bool operator<= (const SortContainer<T,U>& other)
	{return val <= other.val;}

	T* ptr;	//pointer to the data element
	U val;	//sorting value

};


//--------------------------------------------------------------------------------
//	@	SortContainer<T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class T, class U>
SortContainer<T, U>& SortContainer<T, U>::operator=(const SortContainer& other)
{
	ptr = other.ptr;
	val = other.val;

	return *this;
}	//End: SortContainer<T>::operator=()


#endif