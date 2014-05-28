#include "Overworld.h"
#include "Clipper.h"
#include "Dg_io.h"
#include "Systems.h"
#include "WindowManager.h"

//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Overworld::Overworld()
{
	//Load from input file
	Init();

	//Load the help text
	helpBox.Load("help");

	//Assign cameras to viewports (use a SYSTEM)

	//Start timer
	timer.Start();

	//Grab mouse
	mousecontrol.Grab(WINDOW);
	
	fps.SetPosition(10, 10);

}	//End: Overworld::Overworld


//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Overworld::~Overworld()
{
	//Reset mouse
	mousecontrol.Release(WINDOW);
}


//--------------------------------------------------------------------------------
//		Load the overworld from an xml file
//--------------------------------------------------------------------------------
bool Overworld::Init()
{
    gameData.LoadDataFile("gamma.xml");

	//--------------------------------------------------------------------------------
	//		Run post processing system
	//--------------------------------------------------------------------------------

	
	SYSTEM_PostProcess(gameData);
	

}	//End: operator>>()