/*!
* @file PointLightBASE.cpp
*
* @author Frank Hart
* @date 8/01/2014
*
* Class definitions: PointLightBASE
*/

#include "PointLightBASE.h"
#include "CommonMath.h"

//--------------------------------------------------------------------------------
//		Statics
//--------------------------------------------------------------------------------
const float PointLightBASE::cutOff = 0.02f;


//--------------------------------------------------------------------------------
//		PointLightBASE::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
PointLightBASE& PointLightBASE::operator=(const PointLightBASE& other)
{
	Light::operator=(other);
	return *this;

}	//End: Light::operator=()


//--------------------------------------------------------------------------------
//		PointLightBASE::Radius()
//--------------------------------------------------------------------------------
//		Get the cut off distance for the light
//--------------------------------------------------------------------------------
/*!
 * The intensity of a point light attenuates at a rate proportional
 * to the inverse square of the distance away from the source.
 * Theoretically the light will continue on to infinity, however past
 * a certain distance, the intensity will be to low to have any real
 * impact on the scene. The `Radius()` of a pointlight is the distance
 * away from the source where the intensity drops below the `cutOff`.
 */
float PointLightBASE::Radius() const
{
	return DgSqrt(intensity/cutOff);

}	//End: PointLightBASE::Radius()


//--------------------------------------------------------------------------------
//		PointLightBASE::SqRadius()
//--------------------------------------------------------------------------------
//		Get the squared cut off distance for the light
//--------------------------------------------------------------------------------
float PointLightBASE::SqRadius() const
{
	return intensity/cutOff;

}	//End: PointLightBASE::SqRadius()