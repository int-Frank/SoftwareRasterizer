/*!
* @file AmbientLight.h
*
* @author Frank Hart
* @date 7/01/2014
*
* Class header: AmbientLight
*/

#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "Light.h"

struct Vertex;
struct Polygon;
class Mesh;
class VQS;
namespace pugi{ class xml_node; }

/*!
 * @ingroup lights
 *
 * @class AmbientLight
 *
 * @brief An ambient light source represents a fixed-intensity and 
 * fixed-color light source that affects all objects in the scene equally
 *
 * @author Frank Hart
 * @date 7/01/2014
 */
class AmbientLight : public Light
{
public:
	//Constructor / destructor
	AmbientLight(){}
	~AmbientLight(){}

	//Copy operations
	AmbientLight(const AmbientLight& other): Light(other) {}
	AmbientLight& operator= (const AmbientLight&);

	/*!
	* @brief Creates a deep copy of a derived object.
	*
	* @return Pointer to the newly created object.
	*/
	AmbientLight* clone() const {return new AmbientLight(*this);}
	
	//! @brief Build light from pugi::xml_node.
	friend pugi::xml_node& operator>>(pugi::xml_node&, AmbientLight& dest);

	//! Adjusts intensity.
	void Transform(const VQS&);

	//! Adjusts intensity.
	void TransformQuick(const VQS&);

	//! Temporarily transform the light, then add to a Mesh.
	void AddToMesh(Mesh&, const VQS&, const Materials&) const;

	/*! Does the light touch the sphere?
	 *
     * @return 
     *        - 0: No collision
     *        - 1: Collision
     */
	uint8 Test(const Sphere& r) const {return 1;}

protected:
	//Data members
};


#endif