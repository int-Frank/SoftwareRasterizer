/*!
* @file DirectionalLight.cpp
*
* @author Frank Hart
* @date 8/01/2014
*
* Class definitions: DirectionalLight
*/

#include "DirectionalLight.h"
#include "VQS.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Mesh.h"
#include "Materials.h"


//--------------------------------------------------------------------------------
//	@	DirectionalLight::DirectionalLight()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
DirectionalLight::DirectionalLight()
{
	direction = Vector4::xAxis;
}	//End: DirectionalLight::DirectionalLight()


//--------------------------------------------------------------------------------
//	@	DirectionalLight::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
DirectionalLight& DirectionalLight::operator=
	(const DirectionalLight& other)
{
	if (this == &other)
		return *this;

	Light::operator=(other);
	direction = other.direction;

	return *this;

}	//End: Light::operator=()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Read from XML node
//--------------------------------------------------------------------------------
/*!
 * AmbientLight is built from the following tags:
 * | Tag       | Description         | Example                                 |
 * | :-------: | :---------------:   | :-------------------------------------: |
 * | color     | An ARGB Color value | `<color>255 255 255 255</color>`        |
 * | intensity | float [0, inf]      | `<intensity>1.2</intensity>`            |
 * | direction | Vector4             | `<direction>2.9 0 -0.5</direction>`     |
 *
 * Example XML document:
 *
 *     <directionallight>
 *       <color>255 255 255 255</color>
 *       <intensity>1.2</intensity>
 *       <direction>2.9 0 -0.5</direction>
 *     </directionallight>
 *
 * Default values are used if a tag is not found.
 */
pugi::xml_node& operator>>(pugi::xml_node& node, DirectionalLight& dest)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "color")
		{
			Color temp;
			DgString(it->child_value()) >> temp;
			dest.SetColor(temp);
		}
		else if (tag == "intensity")
		{
			float temp;
			DgString(it->child_value()) >> temp;
			dest.SetIntensity(temp);
		}
		else if (tag == "direction")
		{
			Vector4 temp;
			DgString(it->child_value()) >> temp;
			dest.SetDirection(temp);
		}
	}

	return node;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	DirectionalLight::SetDirection()
//--------------------------------------------------------------------------------
//		Set the direction of the light
//--------------------------------------------------------------------------------
void DirectionalLight::SetDirection(const Vector4& input)
{
	if (input.IsZero())
		direction = Vector4::xAxis;
	else
	{
		direction = input;
		direction.Normalize();
	}

}	//End: DirectionalLight::SetDirection()


//--------------------------------------------------------------------------------
//	@	DirectionalLight::Transform()
//--------------------------------------------------------------------------------
//		Rotate the directional light
//--------------------------------------------------------------------------------
void DirectionalLight::Transform(const VQS& vqs)
{
	//Rotate vector
	direction = vqs.Rotate(direction);
	direction.Normalize();

}	//End: DirectionalLight::Transform()


//--------------------------------------------------------------------------------
//	@	DirectionalLight::TransformQuick()
//--------------------------------------------------------------------------------
//		Rotate the directional light
//--------------------------------------------------------------------------------
void DirectionalLight::TransformQuick(const VQS& vqs)
{
	//Rotate vector
	direction = vqs.Rotate(direction);

}	//End: DirectionalLight::Transform()


//--------------------------------------------------------------------------------
//	@	DirectionalLight::AddToMesh()
//--------------------------------------------------------------------------------
//		Add light to Mesh
//--------------------------------------------------------------------------------
void DirectionalLight::AddToMesh(Mesh& obj, 
								 const VQS& T_OBJ_WLD, 
								 const Materials& mat) const
{
	//Create rotated vector
	Vector4 temp_v(-direction);
	T_OBJ_WLD.RotateSelf(temp_v);

	DgArray<Vertex>& VList = obj.GetVertices();

	if (mat.IsDoubleSided())
	{
		for (uint32 i = 0; i < VList.size(); ++i)
		{
			if (VList[i].state == 'x')
				continue;

			//Find the fraction of light hitting the vertex
			float frac = Dot(temp_v, VList[i].normal);

			//Find the intensity of light at the vertex
			float I = intensity * DgAbs(frac);

			//Add light to vertex
			VList[i].clr += (color*I);
		}
	}
	else
	{
		for (uint32 i = 0; i < VList.size(); ++i)
		{
			if (VList[i].state == 'x')
				continue;

			//Find the fraction of light hitting the vertex
			float frac = Dot(temp_v, VList[i].normal);

			//Check if any light reaches vertex
			if (frac <= 0.0f)
				continue;

			//Find the intensity of light at the vertex
			float I = intensity * frac;

			//Add light to vertex
			VList[i].clr += (color*I);
		}
	}

}	//End: DirectionalLight::AddToVertex()
