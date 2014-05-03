/*!
* @file Light.cpp
*
* @author Frank Hart
* @date 7/01/2014
*
* Class definitions: Light
*/

#include "Light.h"
#include "Vertex.h"
#include "CommonMath.h"
#include "VQS.h"


//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Light::Light(): color(1.0f, 1.0f, 1.0f), intensity(1.0f)
{
}	//End: Light::Light()

//--------------------------------------------------------------------------------
//		Sets light color
//--------------------------------------------------------------------------------
void Light::SetColor(::Color new_color)
{
	color[0] = float(new_color.rgba.red)/255.0f;
	color[1] = float(new_color.rgba.green)/255.0f;
	color[2] = float(new_color.rgba.blue)/255.0f;

}	//End: Light::SetColor()


//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Light& Light::operator=(const Light& other)
{
	intensity = other.intensity;
	color = other.color;

	return *this;

}	//End: Light::operator=()


//--------------------------------------------------------------------------------
//		Set intensity
//--------------------------------------------------------------------------------
void Light::SetIntensity(float new_int)
{
	intensity = DgMax(0.0f, new_int);

}	//End: Light::SetIntensity()


//--------------------------------------------------------------------------------
//		Set intensity
//--------------------------------------------------------------------------------
//! @param multiplier The multiplier is clamped to the interval [EPSILON, inf].
void Light::UpdateIntensity(float multiplier)
{
	intensity *= DgMax(EPSILON, multiplier);

}	//End: Light::UpdateIntensity()