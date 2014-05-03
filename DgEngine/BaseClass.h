/*!
 * @file BaseClass.h
 *
 * @author Frank Hart
 * @date 2/01/2014
 *
 * Class header: BaseClass<T>
 */

#ifndef BASECLASS_H
#define BASECLASS_H

/*!
 * @ingroup utility_other
 *
 * @class BaseClass
 *
 * @brief This class contains methods that are useful to any derived classes.
 *
 * @author Frank Hart
 * @date 1/01/2014
 */
template<class T>
class BaseClass
{
public:
	
	BaseClass(){}
	virtual ~BaseClass(){}

	/*!
	 * @brief Creates a deep copy of a derived object.
	 *
	 * @return Pointer to the newly created object.
	 */
	virtual T* clone() const =0;

};


#endif