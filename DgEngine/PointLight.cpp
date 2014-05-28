/*!
* @file PointLight.h
*
* @author Frank Hart
* @date 8/01/2014
*
* Class definitions: PointLight
*/

#include "PointLight.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Mesh.h"
#include "Vector4.h"
#include "VQS.h"
#include "pugixml.hpp"
#include "Sphere.h"
#include "Materials.h"
#include "Dg_io.h"


//--------------------------------------------------------------------------------
//		PointLight::PointLight()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
PointLight::PointLight()
{
	sphere.Set(Point4::origin, Radius());

}	//End: PointLight::PointLight()


//--------------------------------------------------------------------------------
//		PointLight::PointLight()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
PointLight& PointLight::operator=(const PointLight& other)
{
	PointLightBASE::operator=(other);
	sphere = other.sphere;

	return *this;

}	//End: Light::operator=()


//--------------------------------------------------------------------------------
//		PointLight::SetPosition()
//--------------------------------------------------------------------------------
//		Sets position of the light
//--------------------------------------------------------------------------------
void PointLight::SetPosition(const Point4& input)
{
	sphere.SetCenter(input);

}	//End: PointLight::SetPosition()


//--------------------------------------------------------------------------------
//		PointLight::Transform()
//--------------------------------------------------------------------------------
//		Transform light position
//--------------------------------------------------------------------------------
void PointLight::Transform(const VQS& vqs)
{
	//Update Intensity
	UpdateIntensity(vqs.S() * vqs.S());

	//Update sphere
	sphere.Transform(vqs);
	sphere.SetRadius(Radius());

}	//End: PointLight::Transform()


//--------------------------------------------------------------------------------
//		PointLight::TransformQuick()
//--------------------------------------------------------------------------------
//		Transform light position
//--------------------------------------------------------------------------------
void PointLight::TransformQuick(const VQS& vqs)
{
	//Update Intensity
	UpdateIntensity(vqs.S() * vqs.S());

	//Update sphere
	sphere.TransformQuick(vqs);
	sphere.SetRadius(Radius());

}	//End: PointLight::Transform()


//--------------------------------------------------------------------------------
//		operator>>()
//--------------------------------------------------------------------------------
//		BUild light from an xml node
//--------------------------------------------------------------------------------
/*!
 * PointLight is built from the following tags:
 * | Tag       | Description         | Example                          |
 * | :-------: | :---------------:   | :------------------------------: |
 * | color     | An ARGB Color value | `<color>255 255 255 255</color>` |
 * | intensity | float [0, inf]      | `<intensity>1.2</intensity>`     |
 * | position  | Point4              | `<position>10 2 -4.2</position>` |
 *
 * Example XML document:
 *
 *     <pointlight>
 *       <color>255 255 255 255</color>
 *       <intensity>1.2</intensity>
 *       <position>10 2 -4.2</position>
 *     </pointlight>
 *
 * Default values are used if a tag is not found.
 */
pugi::xml_node& operator>>(pugi::xml_node& node, PointLight& dest)
{
	//iterate through all child nodes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "position")
		{
			Point4 p;
			DgString(it->child_value()) >> p;
			dest.sphere.SetCenter(p);
		}
		else if (tag == "intensity")
		{
			float inten;
			DgString(it->child_value()) >> inten;
			dest.SetIntensity(inten);
			dest.sphere.SetRadius(dest.Radius());

		}
		else if (tag == "color")
		{
			Color clr;
            if (StringToNumber(clr.i, it->child_value(), std::hex))
            {
                dest.SetColor(clr);
            }
		}

    }

	return node;

}	//End: PointLight::Build(pugi::xml_node&)


//--------------------------------------------------------------------------------
//		PointLight::AddToMesh()
//--------------------------------------------------------------------------------
//		Temporarily transform the light, then add to a Mesh.
//--------------------------------------------------------------------------------
void PointLight::AddToMesh(Mesh& obj, 
						   const VQS& T_OBJ_WLD,
						   const Materials& mat) const
{
	//Create transformed point
	Point4 temp_p(sphere.Center());
	temp_p *= T_OBJ_WLD;

	//Create transformed intensity
	float temp_int = Intensity() * T_OBJ_WLD.S() * T_OBJ_WLD.S();

	DgArray<Vertex>& VList = obj.GetVertices();

	if (mat.IsDoubleSided())
	{
		for (uint32 i = 0; i < VList.size(); ++i)
		{
			if (VList[i].state == 'x')
				continue;

			//Find vector from source to vertex
			Vector4 v_SV = temp_p - VList[i].position;

			//Find distance to vertex squared
			float d2 = v_SV.LengthSquared();

			//Normalise vector
			v_SV.Normalize();

			//Find the fraction of light hitting the vertex
			float frac = Dot(v_SV, VList[i].normal);

			//Find the intensity of light at the vertex
			float I = temp_int * DgAbs(frac) / d2;

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

			//Find vector from source to vertex
			Vector4 v_SV = temp_p - VList[i].position;

			//Find distance to vertex squared
			float d2 = v_SV.LengthSquared();

			//Normalise vector
			v_SV.Normalize();

			//Find the fraction of light hitting the vertex
			float frac = Dot(v_SV, VList[i].normal);

			//Check if any light reaches vertex
			if (frac <= 0.0f)
				continue;

			//Find the intensity of light at the vertex
			float I = temp_int * frac / d2;

			//Add light to vertex
			VList[i].clr += (color*I);
		}
	}
	

}	//End: PointLight::AddToVertex()


//--------------------------------------------------------------------------------
//		PointLight::Test()
//--------------------------------------------------------------------------------
//		Test if light touches a sphere
//--------------------------------------------------------------------------------
/*! 
 * @return
 *        - 0: No collision
 *        - 1: Collision
 */
uint8 PointLight::Test(const Sphere& s) const
{
	float dummy;
	return TestSphereSphere(sphere, s, dummy);

}	//End: PointLight::Test()