/*!
* @file Timer.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: Timer
*/

#ifndef TIMER_H
#define TIMER_H

#include "DgTypes.h"

/*!
* @ingroup utility_other
*
* @class Timer
*
* @brief Simple timer with start, stop, lap and reset.
*
* @author Frank Hart
* @date 2/01/2014
*/
class Timer
{
public:
	//constructor/destructor
	Timer();
	~Timer() {}

	//copy constructor
	Timer(const Timer&);

	//Clock actions
	void Start();
	void Stop();
	void Reset();

	//! Get the current time (ms).
	uint32 Time();

	//! Get the time since last lap (ms).
	uint32 Lap();

	//! Check status of the timer.
	bool IsOn() {return on;}

private:
	//Clock timer when the timer started
	uint32 startticks;

	//Ticks stored when the timer was paused
	uint32 stopticks;

	//Ticks since last check
	uint32 lapticks; 

	//Current lap time
	uint32 lap_current;

	//Timer status
	bool on;

};


#endif