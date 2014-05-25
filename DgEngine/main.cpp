#include "SDL.h" 
#include "Utility.h"
#include "StateMachine.h"
#include "Title.h"
#include "FPSTimer.h"
#include "Debugger.h"

#include "Timer.h"
#include "WindowManager.h"

int main( int argc, char* args[] ) 
{ 
    //Redirect std::cerr to file
    std::filebuf CERR_NEW_BUF;
    CERR_NEW_BUF.open("errorlog.txt", std::ios::out);;
    std::streambuf* CERR_OLD_BUF = std::cerr.rdbuf(&CERR_NEW_BUF);

	//Load resources, create screen
    if (!START())
    {
        return 1;
    }

	//Create StateInfo Object
	StateInfo stateinfo(STATE_TITLE);

	//Set the current GameState object
	GameState* currentstate = new Title();

	//Create SDL_Event object
	SDL_Event event;

	//Game loop
	while (stateinfo.stateID != STATE_EXIT)
	{
		//Do state event handling
		currentstate->HandleEvents(stateinfo, event);

		//Do state logic
		currentstate->Logic();

		//Do state rendering
		currentstate->Render();

		//Show debug text
		//DEBUGGER.Draw(0);

		//Compile viewports onto WINDOW and reset viewports.
		ViewportHandler::Compile(WINDOW);
		ViewportHandler::Reset(true, true);

		//New window system
		WINDOW->FlipScreen();

		//If Change state
		if (stateinfo.nextstate != STATE_NULL)
		{
			//Change state
			ChangeState(stateinfo, currentstate);
		}
	}

	//Delete Current state
	delete currentstate;

	//Quit SDL 
	SHUTDOWN();

    std::cerr.rdbuf(CERR_OLD_BUF);
	return 0; 
}