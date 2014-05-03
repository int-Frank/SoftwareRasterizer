/*!
* @file Light.h
*
* @author Frank Hart
* @date 7/01/2014
*
* Class header: Light
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Tuple.h"
#include "DgTypes.h"
#include "Object.h"

class VQS;
struct Vertex;
class Mesh;
struct Polygon;
class Sphere;
class Materials;
namespace pugi{class xml_node;}

/*!
 * @ingroup lights
 *
 * @class Light
 *
 * @brief Base class for all lights.
 *
 * @author Frank Hart
 * @date 2/01/2014
 */
class Light : public Object
{
public:

	//Constructor / destructor
	Light();
	virtual ~Light(){}

	//Copy operations
	Light(const Light& light): intensity(light.intensity), 
		color(light.color){}
	Light& operator= (const Light&);

	//Manipulators

	//! Set a RGB color
	void SetColor(::Color new_color);

	//! Intensity will be clamped to the interval [0, inf]
	void SetIntensity(float new_intensity);

	//! Multiplies the intensity by the input. 
	void UpdateIntensity(float multiplier);

	//! Accessor
	float Intensity()			const {return intensity;}

	//! Accessor
	const Tuple<float>& Color() const {return color;}

	//! Temporarily transform the light, then add to a Mesh.
	virtual void AddToMesh(Mesh&, const VQS&, const Materials&) const =0;

	//! Determines if the light touches a sphere.
	virtual uint8 Test(const Sphere&) const =0;

protected:
	//Data members

	//RGB [0 - 1]
	float intensity;
	Tuple<float> color;

};

#endif