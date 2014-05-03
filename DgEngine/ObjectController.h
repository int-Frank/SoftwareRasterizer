/*!
* @file ObjectController.h
*
* @author Frank Hart
* @date 3/01/2014
*
* Class header: ObjectController
*/

#ifndef OBJECTCONTROLLER_H
#define OBJECTCONTROLLER_H

#include "Vector4.h"
#include "Quaternion.h"
#include "VQS.h"
#include "BasisR3.h"
#include "Tuple.h"
#include "DgTypes.h"


//--------------------------------------------------------------------------------
//	@	ObjectController
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_controls
 *
 * @class ObjectController
 *
 * @brief This class is used to log various movement cues for a game object.
 *
 * From these cues, a final VQS transformation can be extracted. A typical 
 * example of use is is recording mouse/keyboard input over a time iteration to
 * control the player character. Reset() must be called after each iteration to 
 * erase all cues.
 *
 * @todo This is essentially a hack to get the camera moving in the game world. It
 * will no doubt evolve or even become redundant as the method for moving game 
 * objects evolves.
 * 
 * @author Frank Hart
 * @date 3/01/2014
 */
class ObjectController
{
public:
	//Constructor
	ObjectController();

	//! Reset all cues to zero
	void Reset();
	
	//! Log a movement in the +x direction
	void Increment_Left()			{ dv -= Vector4::xAxis; }

	//! Log a movement in the -x direction
	void Increment_Right()			{ dv += Vector4::xAxis; }

	//! Log a movement in the +y direction
	void Increment_Up()				{ dv += Vector4::yAxis; }

	//! Log a movement in the -y direction
	void Increment_Down()			{ dv -= Vector4::yAxis; }

	//! Log a movement in the +z direction
	void Increment_Forward()		{ dv -= Vector4::zAxis; }

	//! Log a movement in the -z direction
	void Increment_Back()			{dv += Vector4::zAxis;}



	//! Log a roll rotation by phi radians
	void Adjust_roll(float phi)		{droll_continuous += phi;}

	//! Log a pitch rotation by phi radians
	void Adjust_pitch(float phi)	{dpitch_continuous += phi;}

	//! Log a yaw rotation by phi radians
	void Adjust_yaw(float phi)		{dyaw_continuous += phi;}



	//! Log a clockwise roll rotation.
	void Increment_Roll_cw()		{droll_discrete -= 1.0f;}

	//! Log an upwards pitch rotation.
	void Increment_Pitch_up()		{dpitch_discrete -= 1.0f;}

	//! Log a left yaw rotation.
	void Increment_Yaw_left()		{dyaw_discrete += 1.0f;}

	//! Log a counter-clockwise roll rotation.
	void Increment_Roll_acw()		{droll_discrete += 1.0f;}

	//! Log a downwards pitch rotation.
	void Increment_Pitch_down()		{dpitch_discrete += 1.0f;}

	//! Log a right yaw rotation.
	void Increment_Yaw_right()		{dyaw_discrete -= 1.0f;}

	//! Read movements from the keyboard state.
	void ReadMovements();

	/*! From the current logged movements cues, get the change in orientation as a Quaternion.
	 * @param dt 
	 */
	Quaternion	Get_do(float dt) const;

	//! From the current logged movements cues, get the change in position.
	Vector4		Get_dp(float dt) const;

	/*! From the current logged movements cues, get the change oriantation and
	 * position as a VQS object.
	 */
	VQS			GetVQS(float dt) const;

	//! From the current logged movements cues, get the change in orientation as euler angles.
	void GetRPY(float& r, float& p, float& y, float dt) const;

	//Set speeds
	void SetTranslationSpeed(float m_s)		{v_speed = m_s;}
	void SetYawSpeed(float deg_s)			{yaw_speed = deg_s;}
	void SetPitchSpeed(float deg_s)			{pitch_speed = deg_s;}
	void SetRollSpeed(float deg_s)			{roll_speed = deg_s;}

private:
	//Movement data
	
	//Translation data
	Vector4 dv;

	//Translation speed
	float v_speed;	//	m/s

	//Orientation data
	float droll_discrete;		//change in roll	: incremented in discrete units (eg pressing a key)
	float dpitch_discrete;		//change in pitch
	float dyaw_discrete;		//change in yaw
	float droll_continuous;		//change in roll	: continuous increment (eg mouse movement)
	float dpitch_continuous;	//change in pitch
	float dyaw_continuous;		//change in yaw

	//Orientation speeds
	float roll_speed;			//	rad/s
	float pitch_speed;			//	rad/s
	float yaw_speed;			//	rad/s

};

#endif