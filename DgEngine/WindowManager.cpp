//================================================================================
// @ WindowManager.cpp
// 
// Description: This file defines WindowManager methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "WindowManager.h"
#include "Dg_io.h"
#include "CommonGraphics.h"
#include "DgRect.h"

//--------------------------------------------------------------------------------
//		Statics
//--------------------------------------------------------------------------------
const uint32 WindowManager::wDefault = 640;
const uint32 WindowManager::hDefault = 480;


//--------------------------------------------------------------------------------
//	@	WindowManager::init()
//--------------------------------------------------------------------------------
//		General constructor
//--------------------------------------------------------------------------------
void WindowManager::init(uint32 w, uint32 h, bool _fullScreen, std::string _title)
{
	//Set title
	title = _title;

	//Set flags
	fullScreen = _fullScreen;

	//Set up the screen
	if (fullScreen)
	{
		// make the scaled rendering look smoother.
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  

		sdlScreen = SDL_CreateWindow(title.c_str(),
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          w, h,
                          SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
		sdlScreen = SDL_CreateWindow(title.c_str(),
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          w, h,
                          SDL_WINDOW_OPENGL);

    //If there was an error in setting up the screen
    if( sdlScreen == NULL )
    {
        std::cerr << "@WindowManager::WindowManager(...) -> Could not create screen" << std::endl;
    }

	sdlRenderer = SDL_CreateRenderer(sdlScreen, -1, 0);

	sdlTexture = SDL_CreateTexture(sdlRenderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               w, h);

	//Letterbox full screenf
	SDL_RenderSetLogicalSize(sdlRenderer, w, h);

	//Set data
	ww = w;
	wh = h;

}	//End: WindowManager::init()


//--------------------------------------------------------------------------------
//	@	WindowManager::WindowManager()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
WindowManager::WindowManager(): sdlScreen(NULL), sdlRenderer(NULL), 
	sdlTexture(NULL), ww(0), wh(0)
{
	init(wDefault, hDefault, false, "Default");

}	//End: WindowManager::WindowManager()


//--------------------------------------------------------------------------------
//	@	WindowManager::WindowManager()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
WindowManager::WindowManager(uint32 w, uint32 h, bool _fullScreen, std::string _title)
	: sdlScreen(NULL), sdlRenderer(NULL), sdlTexture(NULL), ww(0), wh(0)
{
	init(w, h, _fullScreen, _title);

}	//End: WindowManager::WindowManager()


//--------------------------------------------------------------------------------
//	@	WindowManager::WindowManager()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
WindowManager::WindowManager(const WindowManager& other)
	: sdlScreen(NULL), sdlRenderer(NULL), sdlTexture(NULL), ww(0), wh(0)
{
	init(other.ww, other.wh, other.fullScreen, other.title);

}	//End: WindowManager::WindowManager()


//--------------------------------------------------------------------------------
//	@	WindowManager::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
WindowManager& WindowManager::operator=(const WindowManager& other)
{
	if (this == &other)
		return *this;

	Set(other.ww, other.wh, other.fullScreen, other.title);

	return *this;
}	//End: WindowManager::operator=()


//--------------------------------------------------------------------------------
//	@	WindowManager::Set()
//--------------------------------------------------------------------------------
//		Destroys current elements and creates a new window
//--------------------------------------------------------------------------------
void WindowManager::Set(uint32 w, uint32 h, bool _fullScreen, std::string _title)
{
	
	SDL_DestroyTexture(sdlTexture);
    SDL_RenderClear(sdlRenderer);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlScreen);

	sdlScreen = NULL;
	sdlRenderer = NULL;
	sdlTexture = NULL;

	init(w, h, _fullScreen, _title);

}	//End: WindowManager::Set()


//--------------------------------------------------------------------------------
//	@	WindowManager::Resize()
//--------------------------------------------------------------------------------
//		Resizes window
//--------------------------------------------------------------------------------
void WindowManager::Resize(uint32 w, uint32 h)
{
	Set(w, h, fullScreen, title);

}	//End: WindowManager::Resize()


//--------------------------------------------------------------------------------
//	@	WindowManager::FullScreen()
//--------------------------------------------------------------------------------
//		Toggles Fullscreen
//--------------------------------------------------------------------------------
void WindowManager::ToggleFullScreen()
{
	fullScreen = !fullScreen;
	Set(ww, wh, fullScreen, title);

}	//End: WindowManager::FullScreen()


//--------------------------------------------------------------------------------
//	@	WindowManager::FlipScreen()
//--------------------------------------------------------------------------------
//		Display buffer
//--------------------------------------------------------------------------------
void WindowManager::FlipScreen()
{
	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent(sdlRenderer);

}	//End: WindowManager::FlipScreen()


//--------------------------------------------------------------------------------
//	@	WindowManager::~WindowManager()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
WindowManager::~WindowManager()
{
	//Clear GPU texture
	SDL_DestroyTexture(sdlTexture);

	//Destroy renderer
    SDL_RenderClear(sdlRenderer);
    SDL_DestroyRenderer(sdlRenderer);

	//Destroy window
    SDL_DestroyWindow(sdlScreen);

}	//End: WindowManager::~WindowManager()


//--------------------------------------------------------------------------------
//	@	WindowManager::UpdateBuffer()
//--------------------------------------------------------------------------------
//		Add and Image to the buffer
//--------------------------------------------------------------------------------
void WindowManager::UpdateBuffer(const Image& img, uint32 x, uint32 y)
{
	//Get portion of the screen to render to
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = img.h();
	rect.w = img.w();

	//Clip if necessary
	if (!ClipRect(rect, ww, wh))
		return;

	//Update texture
	SDL_UpdateTexture(sdlTexture, &rect, img.pixels(),
		img.pitch() );

}	//End: WindowManager::ApplyToBuffer()