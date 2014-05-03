/*!
 * @file SpotLight.h
 *
 * @author Frank Hart
 * @date 8/01/2014
 *
 * Class header: SpotLight
 */

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "PointLightBASE.h"
#include "Point4.h"
#include "Vector4.h"
#include "Cone.h"

struct Vertex;
struct Polygon;
class Mesh;
class VQS;

/*!
 * @ingroup lights
 *
 * @class SpotLight
 *
 * @brief Light emminating from a source, focused in one direction.
 *
 * A spotlight is defined by two cones of light, the inner and outer cones.
 * The inner cone of light follows the same attenuation rules as the point light.
 * Light dimishes between the inner and outer cones.
 *
 * @author Frank Hart
 * @date 8/01/2014
 */
class SpotLight : public PointLightBASE
{
public:
	//Constructor / destructor
	SpotLight();
	~SpotLight(){}

	//Copy operations
	SpotLight(const SpotLight& other);
	SpotLight& operator= (const SpotLight&);

	/*!
	* @brief Creates a deep copy of a derived object.
	*
	* @return Pointer to the newly created object.
	*/
	SpotLight* clone() const {return new SpotLight(*this);}
		
	//! @brief Build light from pugi::xml_node.
	friend pugi::xml_node& operator>>(pugi::xml_node&, SpotLight& dest);

	//! Adjusts intensity.
	void Transform(const VQS&);

	//! Adjusts intensity.
	void TransformQuick(const VQS&);

	//! Temporarily transform the light, then add to a Mesh.
	void AddToMesh(Mesh&, const VQS&, const Materials&) const;
	
	//! Set the origin and the axis of the spotlight
	void SetRay(const Point4&, const Vector4&);

	//! Set the angles of deviation for the inner and outer cones of light
	void SetSpread(float inner, float outer); 
	
	//! Return the outer cone
	const Cone& GetCone() const {return cone;}
	
	/*! Does the light touch the sphere?
	*
	* @return
	*        - 0: No collision
	*        - 1: Collision
	*/
	uint8 Test(const Sphere& s) const {return TestConeSphere(cone, s);}

private:
	//! The spread of light per unit of distance
	float cosInner;	//Central beam

	//! Bounding geometry
	Cone cone;

};

#endif