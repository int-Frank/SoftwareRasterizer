#include "StateMachine.h"
#include "Title.h"
#include "Overworld.h"
#include "SDL.h"

//GameState* currentstate = NULL;

//--------------------------------------------------------------------------------
//		Request a change in State
//--------------------------------------------------------------------------------
void SetNextState(StateInfo& stateinfo, int newstate)
{
	//If the user doesn't want to quit
	if (stateinfo.nextstate != STATE_EXIT)
	{
		//Set the next State
		stateinfo.nextstate = newstate;
	}

}	//End: SetNextState()


//--------------------------------------------------------------------------------
//		Change State
//--------------------------------------------------------------------------------
void ChangeState(StateInfo& stateinfo, GameState*& currentstate)
{
	//If the state needs to be changed
	if (stateinfo.nextstate != STATE_NULL)
	{
		//Delete current state
		if (stateinfo.nextstate != STATE_EXIT)
		{
			//Delete the current state
			delete currentstate;
		}

		//Change the state
		switch (stateinfo.nextstate)
		{
		case STATE_TITLE:
			currentstate = new Title();
			break;

		case STATE_OVERWORLD:
			currentstate = new Overworld();
			break;
		}

		//Change the current StateID
		stateinfo.stateID = stateinfo.nextstate;

		//Null the next state ID
		stateinfo.nextstate = STATE_NULL;
	}

}	//End: ChangeState()


//--------------------------------------------------------------------------------
//		StateInfo Constructor
//--------------------------------------------------------------------------------
StateInfo::StateInfo()
{
	//State Variables
	stateID = STATE_NULL;
	nextstate = STATE_NULL;

}	//End: StateInfo::StateInfo()


//--------------------------------------------------------------------------------
//		StateInfo Constructor
//--------------------------------------------------------------------------------
StateInfo::StateInfo(int ID)
{
	//State Variables
	stateID = ID;
	nextstate = STATE_NULL;
	
}	//End: StateInfo::StateInfo()


//--------------------------------------------------------------------------------
//		StateInfo Destructor
//--------------------------------------------------------------------------------
StateInfo::~StateInfo()
{
}	//End: StateInfo::~StateInfo()