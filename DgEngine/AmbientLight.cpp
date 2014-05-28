/*!
* @file AmbientLight.cpp
*
* @author Frank Hart
* @date 7/01/2014
*
* Class definitions: AmbientLight
*/

#include "AmbientLight.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Mesh.h"
#include "VQS.h"
#include "pugixml.hpp"


//--------------------------------------------------------------------------------
//	@	AmbientLight::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
AmbientLight& AmbientLight::operator=(const AmbientLight& other)
{
	if (this == &other)
		return *this;

	Light::operator=(other);

	return *this;

}	//End: Light::operator=()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Read from XML node
//--------------------------------------------------------------------------------
/*!
 * AmbientLight is built from the following tags:
 * | Tag       | Description         | Example                          |
 * | :-------: | :---------------:   | :------------------------------: |
 * | color     | An ARGB Color value | `<color>255 255 255 255</color>` |
 * | intensity | float [0, inf]      | `<intensity>1.2</intensity>`     |
 *
 * Example XML document:
 *
 *     <ambientlight>
 *       <color>255 255 255 255</color>
 *       <intensity>1.2</intensity>
 *     </ambientlight>
 *
 * Default values are used if a tag is not found.
 */
pugi::xml_node& operator>>(pugi::xml_node& node, AmbientLight& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "color")
		{
            Color clr;
            if (StringToNumber(clr.i, it->child_value(), std::hex))
            {
                dest.SetColor(clr);
            }
		}
		else if (tag == "intensity")
		{
			float temp;
			DgString(it->child_value()) >> temp;
			dest.SetIntensity(temp);
		}
	}

	return node;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	AmbientLight::Transform()
//--------------------------------------------------------------------------------
//		Transforms the light, only adjusts intensity based on the scale.
//--------------------------------------------------------------------------------
void AmbientLight::Transform(const VQS& vqs)
{
	UpdateIntensity(vqs.S());
}	//End: AmbientLight::Transform()


//--------------------------------------------------------------------------------
//	@	AmbientLight::TransformQuick()
//--------------------------------------------------------------------------------
//		Transforms the light, only adjusts intensity based on the scale.
//--------------------------------------------------------------------------------
void AmbientLight::TransformQuick(const VQS& vqs)
{
	UpdateIntensity(vqs.S());
}	//End: AmbientLight::TransformQuick()


//--------------------------------------------------------------------------------
//	@	AmbientLight::AddToMesh()
//--------------------------------------------------------------------------------
//		Add light to Mesh
//--------------------------------------------------------------------------------
void AmbientLight::AddToMesh(Mesh& mesh, const VQS& vqs, const Materials& mat) const
{
	DgArray<Vertex>& VList = mesh.GetVertices();

	for (uint32 i = 0; i < VList.size(); ++i)
	{
		if (VList[i].state == 'x')
			continue;

		VList[i].clr += color*intensity;
	}

}	//End: AmbientLight::AddToMesh()
