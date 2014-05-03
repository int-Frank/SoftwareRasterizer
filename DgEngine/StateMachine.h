#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "SDL.h"

class WindowManager;

//--------------------------------------------------------------------------------
//		Game states
//--------------------------------------------------------------------------------
enum GameStates
{
	STATE_NULL,
	STATE_TITLE,
	STATE_OVERWORLD,
	STATE_EXIT
};

//Class declarations
class GameState;
struct StateInfo;

//extern GameState* currentstate;

//Function Declarations
void SetNextState(StateInfo&, int newstate);
void ChangeState(StateInfo&, GameState*&);

//--------------------------------------------------------------------------------
//		Mother game state class
//--------------------------------------------------------------------------------
class GameState
{
public:
	virtual void HandleEvents(StateInfo&, SDL_Event&) = 0;
	virtual void Logic() = 0;
	virtual void Render() = 0;
	virtual ~GameState(){};

};


//--------------------------------------------------------------------------------
//		Structure holding State information
//--------------------------------------------------------------------------------
struct StateInfo
{
	//Constructor/Destructor
	StateInfo();
	StateInfo(int ID);
	~StateInfo();

	//State Variables
	int stateID;
	int nextstate;

};


#endif