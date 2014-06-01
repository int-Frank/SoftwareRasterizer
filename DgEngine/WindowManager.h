//================================================================================
// @ WindowManager.h
// 
// Class: WindowManager
//
// The window manager is responsible for creating an application window,
// and providing functionality to update the screen buffer.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "SDL.h"
#include "DgTypes.h"
#include "Image.h"
#include <string>

namespace DgGraphics{enum BlendType;}


//--------------------------------------------------------------------------------
//	@	WindowManager
//--------------------------------------------------------------------------------
class WindowManager
{
	friend class Viewport;
public:
	//Constructor / destructor
	WindowManager();
	WindowManager(uint32 w, uint32 h, bool fullScreen, std::string title); 
	~WindowManager();

	//Copy operations
	WindowManager(const WindowManager&);
	WindowManager& operator= (const WindowManager&);

	//Modify size
	void Set(uint32 w, uint32 h, bool fullScreen, std::string _title);
	void Resize(uint32 w, uint32 h);
	void ToggleFullScreen();

	//Render the buffer
	void FlipScreen();

	//Add an Image to the buffer
	void UpdateBuffer(const Image&, uint32 x, uint32 y);

	//Return functions
	uint32 w() const {return ww;}
	uint32 h() const {return wh;}

	SDL_Window* Get_SDL_Window() {return sdlScreen;}

private:
	//Data members
	std::string title;
	bool fullScreen;

	SDL_Window*		sdlScreen;		//The window
	SDL_Renderer*	sdlRenderer;	//Viewport
	SDL_Texture*	sdlTexture;		//Stored on the GPU

	//Size of the window.
	uint32 ww;
	uint32 wh;

	//Default sizes.
	static const uint32 wDefault;
	static const uint32 hDefault;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------

	//Initialise screen
	void init(uint32 w, uint32 h, bool fullScreen, std::string title);
};

//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
namespace global
{
  extern WindowManager *WINDOW;
}

#endif