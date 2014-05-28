/*!
* @file SpotLight.h
*
* @author Frank Hart
* @date 8/01/2014
*
* Class definitions: SpotLight
*/

#include "SpotLight.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Mesh.h"
#include "Vector4.h"
#include "CommonMath.h"
#include "VQS.h"
#include "pugixml.hpp"
#include "Materials.h"

//--------------------------------------------------------------------------------
//		Defaults
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//		Constructor, default angles:
//		inner:		(15 deg)
//		outer:		(30 deg)
//--------------------------------------------------------------------------------
SpotLight::SpotLight() : cosInner(0.9659258262890682867497431997289f),
cone(Point4::origin, Vector4::xAxis, Radius(), 0.52359877559829887307710723054658f)
{
}	//End: SpotLight::SpotLight()


//--------------------------------------------------------------------------------
//	@	SpotLight::Spotlight()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
SpotLight::SpotLight(const SpotLight& other) : PointLightBASE(other),
cone(other.cone), cosInner(other.cosInner)
{

}	//End: SpotLight::Spotlight()


//--------------------------------------------------------------------------------
//	@	SpotLight::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
SpotLight& SpotLight::operator=(const SpotLight& other)
{
	Light::operator=(other);
	cosInner = other.cosInner;
	cone = other.cone;

	return *this;

}	//End: Light::operator=()


//--------------------------------------------------------------------------------
//	@	SpotLight::SetRay()
//--------------------------------------------------------------------------------
//		Sets position of the light
//--------------------------------------------------------------------------------
void SpotLight::SetRay(const Point4& p, const Vector4& a)
{
	cone.SetOrigin(p);
	cone.SetAxisSafe(a);

}	//End: SpotLight::SetPosition()


//--------------------------------------------------------------------------------
//	@	SpotLight::SetSpread()
//--------------------------------------------------------------------------------
//		Set angles
//--------------------------------------------------------------------------------
void SpotLight::SetSpread(float inner, float outer)
{
	inner = DgMax(0.0f, inner);
	inner = DgMin(PI, inner);
	outer = DgMax(inner, outer);
	outer = DgMin(PI, outer);

	cosInner = DgCos(inner);

	cone.SetAngle(outer);

}	//End: SpotLight::SetSpread()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		BUild light from an xml node
//--------------------------------------------------------------------------------
/*!
 * PointLight is built from the following tags:
 * | Tag       | Description                                                 | Example                          |
 * | :-------: | :-------------------------------------------------------:   | :------------------------------: |
 * | color     | An ARGB Color value                                         | `<color>255 255 255 255</color>` |
 * | intensity | float [0, inf]                                              | `<intensity>1.2</intensity>`     |
 * | axis      | Vector4, direction of the light                             | `<axis>10 2 -4.2</axis>`         |
 * | origin    | Point4, the light source                                    | `<origin>-8.21 7 0</origin>`     |
 * | angles    | float inner, float outer: angle of deviation from the axis. | `<angles>0.2 0.9</angles>`       |
 *
 * Example XML document:
 *
 *     <pointlight>
 *       <color>255 255 255 255</color>
 *       <intensity>1.2</intensity>
 *       <origin>10 2 -4.2</origin>
 *       <axis>10 2 -4.2</axis>
 *       <angles>0.2 0.9</angles>
 *     </pointlight>
 *
 * Default values are used if a tag is not found.
 */
pugi::xml_node& operator>>(pugi::xml_node& node, SpotLight& dest)
{
	//Defaults
	Point4 origin(0.0f, 0.0f, 0.0f);
	Vector4 axis(1.0f, 0.0f, 0.0f);
	float inner = 0.1f;
	float outer = 0.4f;
	float inten = 1;
	Color clr = Color::WHITE;

	//iterate through all child nodes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "origin")
		{
			DgString(it->child_value()) >> origin;
		}
		else if (tag == "direction")
		{
			DgString(it->child_value()) >> axis;
		}
		else if (tag == "angles")
		{
			DgString(it->child_value()) >> inner >> outer;
		}
		else if (tag == "intensity")
		{
			DgString(it->child_value()) >> inten;
		}
		else if (tag == "color")
		{
            StringToNumber(clr.i, it->child_value(), std::hex);
		}

    }

	//Set Data
	dest.SetIntensity(inten);
	dest.SetColor(clr);
	dest.SetRay(origin, axis);
	dest.SetSpread(inner, outer);
	dest.cone.SetRadius(dest.Radius());

	return node;
}	//End: SpotLight::Build(pugi::xml_node&)


//--------------------------------------------------------------------------------
//	@	SpotLight::Transform()
//--------------------------------------------------------------------------------
//		Transform light position
//--------------------------------------------------------------------------------
void SpotLight::Transform(const VQS& vqs)
{
	//Update Intensity
	UpdateIntensity(vqs.S() * vqs.S());

	//Transform cone
	cone.Transform(vqs);

	//Set cone radius
	cone.SetRadius(Radius());

}	//End: SpotLight::Transform()


//--------------------------------------------------------------------------------
//	@	SpotLight::TransformQuick()
//--------------------------------------------------------------------------------
//		Transform light position
//--------------------------------------------------------------------------------
void SpotLight::TransformQuick(const VQS& vqs)
{
	//Update Intensity
	UpdateIntensity(vqs.S() * vqs.S());

	//Transform cone
	cone.TransformQuick(vqs);

	//Set cone radius
	cone.SetRadius(Radius());

}	//End: SpotLight::Transform()


//--------------------------------------------------------------------------------
//	@	SpotLight::AddToMesh()
//--------------------------------------------------------------------------------
//		Add light to Mesh
//--------------------------------------------------------------------------------
void SpotLight::AddToMesh(Mesh& mesh, 
						  const VQS& vqs,
						  const Materials& mat) const
{
	//Create transformed point
	Point4 o(cone.Origin());
	o *= vqs;
	Vector4 a(cone.Axis());
	vqs.RotateSelf(a);

	//Create transformed intensity
	float new_int = Intensity() * vqs.S() * vqs.S();

	DgArray<Vertex>& VList = mesh.GetVertices();

	if (mat.IsDoubleSided())
	{
		for (uint32 i = 0; i < VList.size(); ++i)
		{
			if (VList[i].state == 'x')
				continue;

			//Find vector from vertex to the source
			Vector4 v_VS = o - VList[i].position;

			//Find distance to vertex squared
			float d2 = v_VS.LengthSquared();

			//Normalise vector
			v_VS.Normalize();

			//Find the fraction of light hitting the vertex (cos x)
			float frac = Dot(v_VS, VList[i].normal);

			//The vertex in relation to the cone
			float cos_phi = Dot(-v_VS, a);

			//Is the vertex is outside the designated cone?
			if (cos_phi < cone.CosTheta())
				continue;

			//Intensity modifier for corona
			float modifier = 1.0f;

			//If vertex is in the corona, adjust modifier.
			if (cos_phi < cosInner)
			{
				modifier = (cos_phi - cone.CosTheta()) / (cosInner - cone.CosTheta());
				modifier *= modifier;
			}

			//Find intensity
			float I = modifier*new_int * DgAbs(frac) / d2;

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

			//Find vector from vertex to the source
			Vector4 v_VS = o - VList[i].position;

			//Find distance to vertex squared
			float d2 = v_VS.LengthSquared();

			//Normalise vector
			v_VS.Normalize();

			//Find the fraction of light hitting the vertex (cos x)
			float frac = Dot(v_VS, VList[i].normal);

			//Is the vertex pointing away from the ray?
			if (frac <= 0.0f)
				continue;

			//The vertex in relation to the cone
			float cos_phi = Dot(-v_VS, a);

			//Is the vertex is outside the designated cone?
			if (cos_phi < cone.CosTheta())
				continue;

			//Intensity modifier for corona
			float modifier = 1.0f;

			//If vertex is in the corona, adjust modifier.
			if (cos_phi < cosInner)
			{
				modifier = (cos_phi - cone.CosTheta()) / (cosInner - cone.CosTheta());
				modifier *= modifier;
			}

			//Find intensity
			float I = modifier*new_int * frac / d2;

			//Add light to vertex
			VList[i].clr += (color*I);
		}
	}
	
}	//End: SpotLight::AddToMesh()