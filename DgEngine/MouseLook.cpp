/*!
* @file MouseLook.cpp
*
* @author Frank Hart
* @date 2/01/2014
*
* Class definitions: MouseLook
*/

#include "MouseLook.h"
#include "WindowManager.h"
#include "CommonMath.h"

//--------------------------------------------------------------------------------
//	@	MouseLook::MouseLook()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
MouseLook::MouseLook()
{
	//Set default sensitivities
	x_sens = 0.1f;
	y_sens = 0.1f;

	sens_min = 0.0f;
	sens_max = 5.0f;

	//Assign movement directions
	x_flip = 1;
	y_flip = 1;

	//State is inactive
	state = 'r';

}	//End: MouseLook::MouseLook()


//--------------------------------------------------------------------------------
//	@	MouseLook::~MouseLook()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
MouseLook::~MouseLook()
{
}	//End: MouseLook::~MouseLook()


//--------------------------------------------------------------------------------
//	@	MouseLook::SetSensLimits()
//--------------------------------------------------------------------------------
//		Set sensitivity limits
//--------------------------------------------------------------------------------
//! Input wills be clamped to the range [0, inf].
void MouseLook::SetSensLimits(float smin, float smax)
{
	//Check min
	if (smin < 0.0f)
		sens_min = 0.0f;
	else
		sens_min = smin;

	//Check max
	if (smax < smin)
		sens_max = smin;
	else
		sens_max = smax;

}	//End: MouseLook::SetSensLimits()


//--------------------------------------------------------------------------------
//	@	MouseLook::FlipVertical()
//--------------------------------------------------------------------------------
//		Flip Vertical movement
//--------------------------------------------------------------------------------
void MouseLook::FlipVertical()
{
	y_flip *= -1;

}	//End: MouseLook::FlipVertical()


//--------------------------------------------------------------------------------
//	@	MouseLook::FlipHorizontal()
//--------------------------------------------------------------------------------
//		Flip Horizontal movement
//--------------------------------------------------------------------------------
void MouseLook::FlipHorizontal()
{
	x_flip *= -1;

}	//End: MouseLook::FlipHorizontal()


//--------------------------------------------------------------------------------
//	@	MouseLook::Grab()
//--------------------------------------------------------------------------------
//		Grab the mouse
//--------------------------------------------------------------------------------
void MouseLook::Grab(WindowManager* window)
{
	//If mouse if released
	if (state != 'a')
	{
		//Get window dimensions
		uint32 w = window->w();
		uint32 h = window->h();

		//Reset mouse
		SDL_WarpMouseInWindow(window->Get_SDL_Window(), w/2, h/2);
		SDL_SetWindowGrab(window->Get_SDL_Window(), SDL_TRUE);			//Restrict mouse to window
		SDL_ShowCursor(0);					//Hide cursor

		//i for initial
		state = 'i';
	}

}	//End: MouseLook::Grab()


//--------------------------------------------------------------------------------
//	@	MouseLook::Release()
//--------------------------------------------------------------------------------
//		Realease the mouse
//--------------------------------------------------------------------------------
void MouseLook::Release(WindowManager* window)
{
	//If mouse is grabbed
	if (state != 'r')
	{
		//Get window dimensions
		uint32 w = window->w();
		uint32 h = window->h();

		//Reset mouse
		SDL_WarpMouseInWindow(window->Get_SDL_Window(), w/2, h/2);				//Bring mouse to center
		SDL_SetWindowGrab(window->Get_SDL_Window(), SDL_FALSE);		//Release mouse
		SDL_ShowCursor(1);				//Show cursor

		//x for released
		state = 'r';
	}
}	//End: MouseLook::Release()


//--------------------------------------------------------------------------------
//	@	MouseLook::Toggle()
//--------------------------------------------------------------------------------
//		Grab/Release toggle
//--------------------------------------------------------------------------------
void MouseLook::Toggle(WindowManager* window)
{
	if (state == 'r')
		Grab(window);
	else if (state == 'a' || state == 'i')
		Release(window);

}	//End: MouseLook::Toggle()


//--------------------------------------------------------------------------------
//	@	MouseLook::SetSens()
//--------------------------------------------------------------------------------
//		Set Sensitivity for both x and y
//--------------------------------------------------------------------------------
//! @param val Input will be clamped to the min and max sensitivies.
void MouseLook::SetSens(float val)
{
	//Clamp value
	ClampNumber(sens_min, sens_max, val);

	//Set sensitivities
	x_sens = y_sens = val;

}	//End: MouseLook::SetSens(float val)


//--------------------------------------------------------------------------------
//	@	MouseLook::SetXSens()
//--------------------------------------------------------------------------------
//		Set x Sensitivity
//--------------------------------------------------------------------------------
//! @param val Input will be clamped to the x-min and x-max sensitivies.
void MouseLook::SetXSens(float val)
{
	//Clamp value
	ClampNumber(sens_min, sens_max, val);

	//Set sensitivities
	x_sens = val;

}	//End: MouseLook::SetXSens(float val)


//--------------------------------------------------------------------------------
//	@	MouseLook::SetYSens()
//--------------------------------------------------------------------------------
//		Set y Sensitivity
//--------------------------------------------------------------------------------
//! @param val Input will be clamped to the y-min and y-max sensitivies.
void MouseLook::SetYSens(float val)
{
	//Clamp value
	ClampNumber(sens_min, sens_max, val);

	//Set sensitivities
	y_sens = val;

}	//End: MouseLook::SetYSens(float val)


//--------------------------------------------------------------------------------
//	@	MouseLook::AdjustSens()
//--------------------------------------------------------------------------------
//		Incrememt sensitivities
//--------------------------------------------------------------------------------
void MouseLook::AdjustSens(float val)
{
	//Set sensitivities
	x_sens += val;
	y_sens += val;
	
	//Clamp values
	ClampNumber(sens_min, sens_max, x_sens);
	ClampNumber(sens_min, sens_max, y_sens);

}	//End: MouseLook::AdjustSens()


//--------------------------------------------------------------------------------
//	@	MouseLook::AdjustXSens()
//--------------------------------------------------------------------------------
//		Incrememt x sensitivity
//--------------------------------------------------------------------------------
void MouseLook::AdjustXSens(float val)
{
	//Set sensitivities
	x_sens += val;
	
	//Clamp values
	ClampNumber(sens_min, sens_max, x_sens);

}	//End: MouseLook::AdjustXSens()


//--------------------------------------------------------------------------------
//	@	MouseLook::AdjustYSens()
//--------------------------------------------------------------------------------
//		Incrememt y sensitivity
//--------------------------------------------------------------------------------
void MouseLook::AdjustYSens(float val)
{
	//Set sensitivities
	y_sens += val;
	
	//Clamp values
	ClampNumber(sens_min, sens_max, y_sens);

}	//End: MouseLook::AdjustYSens()


//--------------------------------------------------------------------------------
//	@	MouseLook::GetRotations()
//--------------------------------------------------------------------------------
//		Get angles
//--------------------------------------------------------------------------------
void MouseLook::GetRotations(WindowManager* window, float& x, float& y)
{
	//If state is inactive, no rotations
	switch (state)
	{
	case 'r':	//Released: no mouse input
		{
			//Set default rotations
			x = y = 0.0f;

			return;
		}
	case 'i':	//Mouse has just been grabbed: return no input for this call
		{
			//Set default rotations
			x = y = 0.0f;

			//Activate mouse
			state = 'a';
			
			//Get window dimensions
			uint32 w = window->w();
			uint32 h = window->h();

			//Bring mouse to center
			SDL_WarpMouseInWindow(window->Get_SDL_Window(), w/2, h/2);				//Bring mouse to center

			return;
		}
	}

	//Get mouse coordinates
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	
	//Get window half dimensions
	uint32 wd2 = window->w()/2;
	uint32 hd2 = window->h()/2;

	//Calculate x angle
	x = float( (int(wd2) - mx) * x_flip ) * x_sens;

	//Calculate y angle
	y = float( (my - int(hd2)) * y_flip ) * y_sens;
	
	//Bring mouse to center
	SDL_WarpMouseInWindow(window->Get_SDL_Window(), wd2, hd2);

}	//End: MouseLook::GetRotations()