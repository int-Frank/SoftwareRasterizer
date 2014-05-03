/*!
* @file FPSTimer.cpp
*
* @author Frank Hart
* @date 2/01/2014
*
* Class definitions: FPSTimer
*/

#include "FPSTimer.h"
#include "Timer.h"
#include "Color.h"
#include "Image.h"
#include "Viewport.h"
#include <sstream>

//--------------------------------------------------------------------------------
//	@	FPSTimer::FPSTimer()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
FPSTimer::FPSTimer()
{
	//Create text object
	txt = Text("Sansation_Regular", 20, Color::YELLOW);
	txt.SetPosition(10,10);
	
	//set frame count
	frames = 0;

	//Set current fps
	currentfps = 0;

	//Set updatetime
	updatetime = 250;

	//Start timer
	timer.Start();

}	//End: FPSTimer::FPSTimer()


//--------------------------------------------------------------------------------
//	@	FPSTimer::GrabTime()
//--------------------------------------------------------------------------------
//		Generate text based of current time
//--------------------------------------------------------------------------------
void FPSTimer::GrabTime()
{
	//Increment frames
	frames++;

	if (timer.Time() > updatetime)
	{
		//Calculate current fps
		currentfps = uint16(1000*frames/timer.Time());

		//Reset frames
		frames = 0;

		//Reset Timer
		timer.Reset();
	}

	//Create string
	std::ostringstream oss;
	oss << currentfps;

	//Set Text
	txt.SetString(oss.str());

}	//End: FPSTimer::GrabTime()


//--------------------------------------------------------------------------------
//	@	FPSTimer::Draw()
//--------------------------------------------------------------------------------
//		Display counter
//--------------------------------------------------------------------------------
void FPSTimer::Draw(Image& dest)
{
	//Get text
	GrabTime();

	//Draw FPS
	txt.Draw(dest);

}	//End:FPSTimer::Draw()


//--------------------------------------------------------------------------------
//	@	FPSTimer::Draw()
//--------------------------------------------------------------------------------
//		Display counter on a renderer
//--------------------------------------------------------------------------------
void FPSTimer::Draw(Viewport& dest)
{
	//Get text
	GrabTime();

	//Draw FPS
	txt.Draw(dest);

}	//End:FPSTimer::Draw()