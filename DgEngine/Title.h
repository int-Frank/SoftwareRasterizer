#ifndef TITLE_H
#define TITLE_H

#include "StateMachine.h"
#include "Text.h"
#include "ViewportHandler.h"
#include "FPSTimer.h"

class Image;

//--------------------------------------------------------------------------------
//		Title screen class
//--------------------------------------------------------------------------------
class Title : public GameState
{
public:
	//Loads Title resources
	Title();

	//Frees Title resources
	~Title();

	//Main loop functions
	void HandleEvents(StateInfo&, SDL_Event&);
	void Logic();
	void Render();

private:

	//Some text
	Text menu;

	ViewportHandler view;

	FPSTimer fps;

};

#endif