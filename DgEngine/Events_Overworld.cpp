#include "SDL.h"
#include "Overworld.h"
#include "Rasterizer.h"
#include "Component_POSITION.h"
#include "Dg_io.h"
#include "WindowManager.h"
#include "Utility.h"

//--------------------------------------------------------------------------------
//		Handle Events
//--------------------------------------------------------------------------------
void Overworld::HandleEvents(StateInfo& stateinfo, SDL_Event& event)
{
	//Get the keystates
	const uint8* keystates = SDL_GetKeyboardState( NULL );

	//Keys that can be depressed at the same time 

	//Reset input structure
	cameraControls.Reset();

	//Movement
	if (keystates[SDL_SCANCODE_W])
	{
		cameraControls.Increment_Forward();
	}
	if (keystates[SDL_SCANCODE_S])
	{
		cameraControls.Increment_Back();
	}
	if (keystates[SDL_SCANCODE_A])
	{
		cameraControls.Increment_Left();
	}
	if (keystates[SDL_SCANCODE_D])
	{
		cameraControls.Increment_Right();
	}
	if (keystates[SDL_SCANCODE_Q])
	{
		cameraControls.Increment_Down();
	}
	if (keystates[SDL_SCANCODE_E])
	{
		cameraControls.Increment_Up();
	}
	if (keystates[SDL_SCANCODE_Z])
	{
		cameraControls.Increment_Roll_acw();
	}
	if (keystates[SDL_SCANCODE_C])
	{
		cameraControls.Increment_Roll_cw();
	}

	//While there's events to handle
    while( SDL_PollEvent( &event ) )
    {
		switch(event.type)
		{
		//--------------------------------------------------------------------------------
		//		Windows events
		//--------------------------------------------------------------------------------
		case SDL_WINDOWEVENT:
			{
				switch(event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					{
						//Quit the program
						SetNextState( stateinfo, STATE_EXIT );
						break;
					}
				}
				break;

			}	//End: case SDL_WINDOWEVENT:

		//--------------------------------------------------------------------------------
		//		Keybourd events
		//--------------------------------------------------------------------------------
		case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE :
					{
						//Quit the program
						SetNextState( stateinfo, STATE_TITLE );
						break;
					}
				case SDLK_SPACE :
					{
						//Toggle mouse
						mousecontrol.Toggle(WINDOW);
						break;
					}
				case SDLK_b:
					{
						int ind = 0;
						if (gameData.Cameras.find(0xA1C00000, ind))
						{
							gameData.Cameras[ind].cameraSystem.ToggleActive();
						}
						break;
					}
				case SDLK_h:
					{
						helpBox.ToggleActive();
						break;
					}
				case SDLK_y:
					{
						mousecontrol.FlipVertical();
						break;
					}
				case SDLK_LEFTBRACKET:
					{
						mousecontrol.AdjustSens(-0.02f);
						break;
					}
				case SDLK_RIGHTBRACKET:
					{
						mousecontrol.AdjustSens(0.02f);
						break;
					}
				}
				break;

			} //End: case SDL_KEYDOWN:


		//--------------------------------------------------------------------------------
		//		Mouse events
		//--------------------------------------------------------------------------------
		case SDL_MOUSEMOTION:
			{
				//Get the angles
				float yaw, pitch;
				mousecontrol.GetRotations(WINDOW, yaw, pitch);
			
				//Record the angles
				cameraControls.Adjust_yaw(yaw);
				cameraControls.Adjust_pitch(pitch);
				break;

			} //End: case SDL_MOUSEMOTION:

		} //End: switch(event.type)

	} //End: while( SDL_PollEvent( &event ) )

}	//End: Overworld::HandleEvents()