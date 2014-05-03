/*!
* @file Timer.cpp
*
* @author Frank Hart
* @date 2/01/2014
*
* Class definitions: Timer
*/

#include "Timer.h"
#include <time.h>

//--------------------------------------------------------------------------------
//	@	Timer::Timer()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Timer::Timer()
{
	startticks = 0;
	stopticks = clock();
	lapticks = stopticks;
	lap_current = 0;
	on = false;

}	//End: Timer::Timer()


//--------------------------------------------------------------------------------
//	@	Timer::Timer()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Timer::Timer(const Timer& other)
{
	startticks = other.startticks;
	stopticks = other.stopticks;
	lapticks = other.lapticks;
	lap_current = other.lap_current;
	on = other.on;

}	//End: Timer::Timer()


//--------------------------------------------------------------------------------
//	@	Timer::Start()
//--------------------------------------------------------------------------------
//		Start timer
//--------------------------------------------------------------------------------
void Timer::Start()
{
	//Check if already going
	if (on)
		return;

	//Set bools
	on = true;

	//Get currect ticks
	uint32 time = clock();
	stopticks = time - startticks;
	startticks = time - stopticks;
	lapticks = time;

}	//End: Timer::Start()


//--------------------------------------------------------------------------------
//	@	Timer::Stop()
//--------------------------------------------------------------------------------
//		Stop timer
//--------------------------------------------------------------------------------
void Timer::Stop()
{
	if (!on)
		return;

	on = false;

	//Add last on time to lap_current
	lap_current += clock() - lapticks;

}	//End: Timer::Stop()


//--------------------------------------------------------------------------------
//	@	Timer::Time()
//--------------------------------------------------------------------------------
//		Get time (ticks)
//--------------------------------------------------------------------------------
uint32 Timer::Time()
{
	//Check if timer is running
	if (on)
	{
		//Return current time minus stopticks
		return clock() - stopticks;
	}

	return startticks;

}	//End: Timer::Time()


//--------------------------------------------------------------------------------
//	@	Timer::Lap()
//--------------------------------------------------------------------------------
//		Get time since last lap
//--------------------------------------------------------------------------------
uint32 Timer::Lap()
{
	if (!on)
		return 0;

	//get lap time
	uint32 total = lap_current + (clock() - lapticks);

	//Reset data
	lap_current = 0;
	lapticks = clock();

	return total;

}	//End: Timer::Lap()


//--------------------------------------------------------------------------------
//	@	Timer::Reset()
//--------------------------------------------------------------------------------
//		Reset Timer
//--------------------------------------------------------------------------------
void Timer::Reset()
{
	startticks = 0;
	stopticks = clock();
	lapticks = stopticks;
	lap_current = 0;

}	//End: Timer::Reset()
