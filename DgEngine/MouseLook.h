/*!
 * @file MouseLook.h
 *
 * @author Frank Hart
 * @date 2/01/2014
 *
 * Class header: MouseLook
 */

#ifndef MOUSELOOK_H
#define MOUSELOOK_H

class WindowManager;

//--------------------------------------------------------------------------------
//	@	MouseLook
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_controls
 *
 * @class MouseLook
 *
 * @brief A class to control mouse movement and events.
 *
 * This class processes mouse movement and converts it to angles of rotation. It also
 * contains methods to grab and release the mouse.
 *
 * @author Frank Hart
 * @date 3/01/2014
 */
class MouseLook
{
public:
	//Constructor/Destructor
	MouseLook();
	~MouseLook();

	//Copy operations
	MouseLook(const MouseLook&);
	MouseLook& operator=(const MouseLook&);

	//! @brief Sets both the x and y sensitivities to val.
	void SetSens(float val);

	//! @brief Sets the x sensitivity to val.
	void SetXSens(float val);

	//! @brief Sets the  y sensitivity to val.
	void SetYSens(float val);

	//! @brief Sets the sensitivity limits.
	void SetSensLimits(float smin, float smax);

	//! Increment both sensitivities by an amount.
	void AdjustSens(float val);

	//! Increment x sensitivity by an amount.
	void AdjustXSens(float val);

	//! Increment y sensitivity by an amount.
	void AdjustYSens(float val);

	//! Flip y rotation angle
	void FlipVertical();

	//! Flip x rotation angle
	void FlipHorizontal();

	/*!
	 * @brief Change state from mouselook to pointer.
	 *
	 * @param[in] window Release the mouse from this window
	 *
	 * @pre: window is the WindowManager currently owning the mouse.
	 * @post: Mouselook is temporarily disabled, mouse pointer is shown and 
	 *        is brought to the center of the window.
	 */
	void Release(WindowManager* window);

	/*!
	* @brief Change state from free pointer to mouselook.
	*
	* @param[in] window Grab the mouse to this window
	*
	* @pre: window is not NULL.
	* @post: Mouselook is enabled, mouse pointer is hidden.
	*/
	void Grab(WindowManager* window);

	/*!
	* @brief Toggles between free and mouselook.
	*
	* @param[in] window Grab the mouse to this window
	*
	* @pre: window is the WindowManager currently owning the mouse. 
	* @post: Toggles between free and mouselook.
	*/
	void Toggle(WindowManager* window);

	/*!
	* @brief Gives angles of rotations calculated from the last mouse movement.
	*
	* @param[in] window The mouse movement is calculated relative to this window.
	* @param[out] x Horizontal angle of rotation.
	* @param[out] y Verticle angle of rotation.
	*
	* @pre: window is the WindowManager currently owning the mouse.
	* @post: If the mouse is grabbed, set the angles of rotations, else
	*        sets x and y to zero.
	*/
	void GetRotations(WindowManager*, float& x, float& y);

private:
	//Data members
	
	//State of the mouse
	//	r: realeased
	//	i: mouse has just been grabbed
	//	a: mouse is grabbed (mouselook is active)
	char state;

	//current x,y sensitivies
	float x_sens;
	float y_sens;

	//sensitivitiy limits
	float sens_max, sens_min;

	//x,y flip
	int x_flip, y_flip;
};

#endif