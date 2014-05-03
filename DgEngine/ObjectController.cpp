//================================================================================
// @ ObjectController.cpp
// 
// Description: ObjectController class definitions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "ObjectController.h"
#include "CommonMath.h"

//--------------------------------------------------------------------------------
//	@	ObjectController::ObjectController()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
ObjectController::ObjectController(): dv(Vector4::origin), v_speed(10.0f), 
	droll_discrete(0.0f), dpitch_discrete(0.0f), dyaw_discrete(0.0f),
	droll_continuous(0.0f), dpitch_continuous(0.0f), dyaw_continuous(0.0f),
	roll_speed(2.5f), pitch_speed(0.7f), yaw_speed(0.7f)
{

}	//End: ObjectController::ObjectController()


//--------------------------------------------------------------------------------
//	@	ObjectController::Reset()
//--------------------------------------------------------------------------------
//		Reset ObjectController data
//--------------------------------------------------------------------------------
void ObjectController::Reset()
{
	dv = Vector4::origin;
	droll_discrete		= 0;
	dpitch_discrete		= 0;
	dyaw_discrete		= 0;
	droll_continuous	= 0;
	dpitch_continuous	= 0;
	dyaw_continuous		= 0;

}	//End: ObjectController::Reset()


//--------------------------------------------------------------------------------
//	@	ObjectController::Get_dp()
//--------------------------------------------------------------------------------
//		Get translation vector
//--------------------------------------------------------------------------------
Vector4 ObjectController::Get_dp(float dt) const
{
	//Normalize
	Vector4 dp(dv);
	dp.Normalize();

	//Process
	return dp*(v_speed);

}	//End: ObjectController::Get_dp()


//--------------------------------------------------------------------------------
//	@	ObjectController::Get_do()
//--------------------------------------------------------------------------------
//		Get orientation quaternion
//--------------------------------------------------------------------------------
Quaternion ObjectController::Get_do(float dt) const
{
	Quaternion temp;

	float roll_total = droll_discrete  *  roll_speed + droll_continuous;
	float pitch_total = dpitch_discrete *  pitch_speed + dpitch_continuous;
	float yaw_total = dyaw_discrete   *  yaw_speed + dyaw_continuous;

	temp.Set(pitch_total, yaw_total, roll_total);

	return temp;

}	//End: ObjectController::Get_de()


//--------------------------------------------------------------------------------
//	@	ObjectController::GetVQS()
//--------------------------------------------------------------------------------
//		Get movement as a VQS structure
//--------------------------------------------------------------------------------
VQS ObjectController::GetVQS(float dt) const
{
	return VQS(Get_dp(dt), Get_do(dt), 1.0f);

}	//End: ObjectController::GetVQS()


//--------------------------------------------------------------------------------
//	@	ObjectController::GetRPY()
//--------------------------------------------------------------------------------
//		Get angles of rotation
//--------------------------------------------------------------------------------
void ObjectController::GetRPY(float& roll, float& pitch, float& yaw, float dt) const
{
	roll =  droll_discrete  *  roll_speed		+ droll_continuous;
	pitch = dpitch_discrete *  pitch_speed + dpitch_continuous;
	yaw = dyaw_discrete   *  yaw_speed + dyaw_continuous;

}	//End: ObjectController::GetRPY()