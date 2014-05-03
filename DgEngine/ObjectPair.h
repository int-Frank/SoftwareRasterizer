/*!
* @file ObjectPair.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: ObjectPair<>
*/

#ifndef OBJECTPAIR_H
#define OBJECTPAIR_H

#include "Object.h"
#include "VQS.h"


/*!
 * @ingroup utility_container
 *
 * @class ObjectPair
 *
 * @brief Couples two Objects together for clean transformations.
 *
 * Wrapper class to simplify transforming objects. It essentially couples two 
 * objects together (base and current) and transforms the current from the base to elimate 
 * floating-point drift. Type T must contain a Transform method, ideally T is derived 
 * from class Object.
 *
 * @author Frank Hart
 * @date 2/01/2014
 */
template<class T>
class ObjectPair : public Object
{
public:
	//Constructor / destructor
	ObjectPair() {}

	//! Both the base and current objects are set to t.
	ObjectPair(const T& t): base(t), current(t) {}
	~ObjectPair() {}

	//Copy operations
	ObjectPair(const ObjectPair& o): 
		current(o.current), base(o.base) {}
	ObjectPair& operator= (const ObjectPair& o);

	/*!
	* @brief Creates a deep copy of the ObjectPair.
	*
	* @return Pointer to the newly created ObjectPair.
	*/
	ObjectPair<T>* clone() const {return new ObjectPair<T>(*this);}

	//Mutators

	//! Both the base and current objects are set to t.
	void Set(const T& t)	 {base = t; current = t;}
	void SetBase(const T& t) {base = t;}

	//! @brief Sets the current to the base.
	void Update()			 {current = base;}

	//Transformations

	//! @brief Transforms the current from the base.
	void Transform(const VQS& vqs)				
	{current = base; current.Transform(vqs);}

	//! @brief QuickTransforms the current from the base.
	void TransformQuick(const VQS& vqs)
	{current = base; current.TransformQuick(vqs);}

	//--------------------------------------------------------------------------------
	//		The data is public, but should be accessed through the interface
	//		as much as possible.
	//--------------------------------------------------------------------------------
	T base;
	T current;

};

//--------------------------------------------------------------------------------
//	@	ObjectPair<T>::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
template<class T>
ObjectPair<T>& ObjectPair<T>::operator=(const ObjectPair<T>& other)
{
	if (this == &other)
		return *this;

	base = other.base;
	current = other.current;

	return *this;
}	//End: ObjectPair<T>::operator=()


#endif