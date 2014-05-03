/*!
* @file Utility.cpp
*
* @author Frank Hart
* @date 3/01/2014
*
* System functions
*/

#include "SDL.h"
#include "Utility.h"
#include "SimpleRNG.h"
#include "SDL_ttf.h"
#include "Dg_io.h"
#include "Text.h"
#include "Debugger.h"
#include "Rasterizer.h"
#include "WindowManager.h"
#include "TextureManager.h"
#include "ViewportHandler.h"
#include <string>

//--------------------------------------------------------------------------------
//		Functions to assist startup
//--------------------------------------------------------------------------------
bool InitializeWindow();


//--------------------------------------------------------------------------------
//	@	START()
//--------------------------------------------------------------------------------
//		Initiate all systems
//--------------------------------------------------------------------------------
/*!
 * - Clear error log file
 * - Set random seed
 * - Initialize all SDL systems
 * - Initialize SDL true type fonts
 * - Initialize the window
 * - Load viewports
 * - Load Texture file
 * - Load Debugger file
 */
bool START()
{
	//Clear errorlog.txt
	CLEAR_LOG();

	//Seed the random number generator
	SimpleRNG::SetSeedFromSystemTime();

	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	//Initialize window 
	if(!InitializeWindow())
		return false;

	//Load Viewports
	ViewportHandler::LoadResources(WINDOW->w(), WINDOW->h());
	
	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
        return false;

	//Load texture document
	TEXTURE_MANAGER.LoadDocument("Images.xml");

	//Load in debug file
	DEBUGGER.Load("debug_text.xml");

	return true;

}	//End: START()


//--------------------------------------------------------------------------------
//	@	InitializeWindow()
//--------------------------------------------------------------------------------
//		Initialize WINDOW
//--------------------------------------------------------------------------------
bool InitializeWindow()
{
	//Fullscreen?
	std::string str = "false";
	GetFromFile("setup.ini", "fullscreen", str);
	bool fullscreen = ToBool(str);

	//Dimensions
	uint32 h = 200, w = 200;
	GetFromFile("setup.ini", "screen_height", h);
	GetFromFile("setup.ini", "screen_width", w);

	WINDOW = new WindowManager(	w, h, fullscreen, "My Game");

    //If everything initialized fine
    return true;

}	//End: InitializeWindow()


//--------------------------------------------------------------------------------
//	@	SHUTDOWN()
//--------------------------------------------------------------------------------
//		Shut down all systems
//--------------------------------------------------------------------------------
/*!
 * - Clear all Text resources
 * - Quit SDL_TTF
 * - Close Window
 * - Shut dow all SDL systems
 */
void SHUTDOWN()
{
	//Clear all resources
	Text::ClearResources();

	//Quit SDL_ttf
    TTF_Quit();

	//Destroy window
	delete WINDOW;

    //Quit SDL
    SDL_Quit();

}	//End: ShutDownSDL()


//--------------------------------------------------------------------------------
//	@	RESIZE_WINDOW()
//--------------------------------------------------------------------------------
//		Resize the window, updates viewports.
//--------------------------------------------------------------------------------
void RESIZE_WINDOW(uint32 w, uint32 h)
{
	if (w == 0 || h == 0)
		return;

	WINDOW->Resize(w, h);
	ViewportHandler::SetParentDimensions(w, h);

}	//End: RESIZE_WINDOW()