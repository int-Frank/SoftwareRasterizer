#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "Title.h"
#include "StateMachine.h"
#include "CommonGraphics.h"
#include "Utility.h"
#include "DgTypes.h"
#include "Dg_io.h"
#include <time.h>
#include "Image.h"
#include "ImageManager.h"
#include "WindowManager.h"

//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Title::Title()
{
	//Set text
	menu.SetString("DgEngine 2.0\n\nPress 's' to start and 'h' to bring up\nthe help screen in the game world");
	menu.SetSize(30);
	menu.SetColor(Color::WHITE);
	menu.SetPosition(100, 50);

	//Attach viewport and activate
	view.Attach(0);
	view.Activate();
	
	fps.SetPosition(10, 10);

}	//End: Title::Title()


//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Title::~Title()
{
}


//--------------------------------------------------------------------------------
//		Handle Events
//--------------------------------------------------------------------------------
void Title::HandleEvents(StateInfo& stateinfo, SDL_Event& event)
{
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
						SetNextState( stateinfo, STATE_EXIT );
						break;
					}
				case SDLK_s :
					{
						//Enter the game
						SetNextState(stateinfo, STATE_OVERWORLD);
						break;
					}
				case SDLK_f :
					{
						//Enter the game
						WINDOW->ToggleFullScreen();
						break;
					}
				}
				break;

			} //End: case SDL_KEYDOWN:

		} //End: switch(event.type)

	} //End: while( SDL_PollEvent( &event ) )

} //End: Title::HandleEvents()


//--------------------------------------------------------------------------------
//		Logic
//--------------------------------------------------------------------------------
void Title::Logic()
{
}	//End: Title::Logic()


//--------------------------------------------------------------------------------
//		Render
//--------------------------------------------------------------------------------
void Title::Render()
{
	Viewport* vp = view.GetViewport();
	if (vp == NULL)
		return;

	//Draw text
	menu.Draw(*vp);
	fps.Draw(*vp);

}	//End: Title::Render()