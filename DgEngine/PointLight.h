/*!
 * @file PointLight.h
 *
 * @author Frank Hart
 * @date 8/01/2014
 *
 * Class header: PointLight
 */

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Point4.h"
#include "PointLightBASE.h"
#include "Sphere.h"

struct Vertex;
struct Polygon;
class Mesh;
class VQS;

/*!
 * @ingroup lights
 *
 * @class PointLight
 *
 * @brief Light emminating from a source in all directions.
 *
 * @author Frank Hart
 * @date 8/01/2014
 */
class PointLight : public PointLightBASE
{
public:

	//Constructor / destructor
	PointLight();
	~PointLight() {}

	//Copy operations
	PointLight(const PointLight& other): PointLightBASE(other), sphere(other.sphere) {}
	PointLight& operator= (const PointLight&);

	/*!
	* @brief Creates a deep copy of a derived object.
	*
	* @return Pointer to the newly created object.
	*/
	PointLight* clone() const {return new PointLight(*this);}

	//! @brief Build light from pugi::xml_node.
	friend pugi::xml_node& operator>>(pugi::xml_node&, PointLight& dest);

	//! Set the position of the light
	void PointLight::SetPosition(const Point4&);

	//! Adjusts intensity.
	void Transform(const VQS&);

	//! Adjusts intensity.
	void TransformQuick(const VQS&);

	//! Temporarily transform the light, then add to a Mesh.
	void AddToMesh(Mesh&, const VQS&, const Materials&) const;

	//! @brief Does the light touch the sphere?
	uint8 Test(const Sphere&) const;

private:
	//Data members

	Sphere sphere;

};

#endif