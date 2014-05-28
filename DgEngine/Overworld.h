#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "StateMachine.h"
#include "Timer.h"
#include "Text.h"
#include "Dg_io.h"
#include "MouseLook.h"
#include "ObjectController.h"
#include "Debugger.h"
#include "Skybox.h"
#include "pugixml.hpp"
#include "GameDatabase.h"
#include <string>
#include "Viewport.h"
#include "FPSTimer.h"
#include "ViewportHandler.h"
#include "DgArray.h"
#include "ViewportEvent.h"
#include "MessageBox.h"


//--------------------------------------------------------------------------------
//		Overworld class
//--------------------------------------------------------------------------------
class Overworld : public GameState
{
public:
	//Loads Overworld resources
	Overworld();

	//Frees Overworld resources
	~Overworld();

	//Input
    bool Init();

	//Main loop functions
	void HandleEvents(StateInfo&, SDL_Event&);
	void Logic();
	void Render();

private:

	//Tag
	std::string id;
	
	//The help text
	MessageBox helpBox;

	//Timer
	Timer timer;

	//Skybox
	Skybox skybox;

	//Controllers
	MouseLook mousecontrol;
	ObjectController cameraControls;

	//Change in time (s)
	uint32 dt;

	//The game database
	GameDatabase gameData;

	//Viewport event list
	DgArray<ViewportEvent> viewport_events;
	
	FPSTimer fps;

};

#endif