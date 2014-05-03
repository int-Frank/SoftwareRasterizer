/*!
* @file PointLightBASE.h
*
* @author Frank Hart
* @date 8/01/2014
*
* Class header: PointLightBASE
*/

#ifndef POINTLIGHTBASE_H
#define POINTLIGHTBASE_H

#include "Light.h"
#include "DgTypes.h"

class Sphere;

/*!
* @ingroup lights
*
* @class PointLightBASE
*
* @brief Base class for PointLight and SpotLight.
*
* @author Frank Hart
* @date 8/01/2014
*/
class PointLightBASE : public Light
{
public:
	//Constructor / destructor
	PointLightBASE(){}
	virtual ~PointLightBASE() {}

	//Copy operations
	PointLightBASE(const PointLightBASE& other): Light(other) {}
	PointLightBASE& operator= (const PointLightBASE& other);

	//! @brief The radius of which the light can light objects.
	float Radius() const;

	//! The squared radius of which the light can light objects.
	float SqRadius() const;

	/*!
	* @brief Creates a deep copy of a derived object.
	*
	* @return Pointer to the newly created object.
	*/
	virtual PointLightBASE* clone() const {return new PointLightBASE(*this);}

	//! Virtual overrider (allows an instance of this class)
	virtual void Transform(const VQS&) {}

	//! Virtual overrider (allows an instance of this class)
	virtual void TransformQuick(const VQS&) {}

	//! Virtual overrider (allows an instance of this class)
	virtual void AddToMesh(Mesh&, const VQS&, const Materials&) const {}

	//! Virtual overrider (allows an instance of this class)
	virtual uint8 Test(const Sphere&) const {return 0;}

private:
	//Data members

	//! Used to determine how far a point light extends.
	static const float cutOff;

};

#endif