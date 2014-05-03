/*!
* @file DirectionalLight.h
*
* @author Frank Hart
* @date 8/01/2014
*
* Class header: DirectionalLight
*/

#ifndef DirectionalLight_H
#define DirectionalLight_H

#include "Light.h"
#include "Vector4.h"

struct Vertex;
struct Polygon;
class Mesh;
class VQS;

/*!
 * @ingroup lights
 *
 * @class DirectionalLight
 *
 * @brief A directional light has color and direction only. 
 *
 * It is a parallel light travelling throught the scene in 
 * the same direction and intensity.
 *
 * @author Frank Hart
 * @date 8/01/2014
 */
class DirectionalLight : public Light
{
public:
	//Constructor / destructor
	DirectionalLight();
	~DirectionalLight(){}

	//Copy operations
	DirectionalLight(const DirectionalLight& other): 
		Light(other), direction(other.direction) {}
	DirectionalLight& operator= (const DirectionalLight&);

	/*!
	* @brief Creates a deep copy of a derived object.
	*
	* @return Pointer to the newly created object.
	*/
	DirectionalLight* clone() const {return new DirectionalLight(*this);}
	
	//! @brief Build light from pugi::xml_node.
	friend pugi::xml_node& operator>>(pugi::xml_node&, DirectionalLight& dest);

	//! Adjusts intensity and direction.
	void Transform(const VQS&);

	//! Adjusts intensity and direction.
	void TransformQuick(const VQS&);

	//! Temporarily transform the light, then add to a Mesh.
	void AddToMesh(Mesh&, const VQS&, const Materials&) const;

	//Set parameters
	void SetDirection(const Vector4&);

	/*! Does the light touch the sphere?
	*
	* @return
	*        - 0: No collision
	*        - 1: Collision
	*/
	uint8 Test(const Sphere& r) const {return 1;}

private:
	//Data members
	Vector4 direction;

private:

};

#endif