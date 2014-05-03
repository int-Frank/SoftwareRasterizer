/*!
* @file FPSTimer.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: FPSTimer
*/

#ifndef FPSTIMER_H
#define FPSTIMER_H

#include "Timer.h"
#include "Text.h"
#include "DgTypes.h"

class Image;
class Viewport;

/*!
* @ingroup utility_other
*
* @class FPSTimer
*
* @brief A class to display current fps on the screen.
*
* This class is mainly for debugging purposes and so does not contain
* methods to change the style of the output.
*
* @author Frank Hart
* @date 2/01/2014
*/
class FPSTimer
{
public:
	//Constructor/destructor
	FPSTimer();
	~FPSTimer() {}

	//Copy constructor
	FPSTimer(const FPSTimer&);

	/*!
	 * Sets the time in which the counter is updated.
	 *
	 * @param[in] tme Time in ms.
	 */
	void SetUpdateTime(uint32 tme) {updatetime = tme;}

	//! The position of the timer on the screen.
	void SetPosition(int32 _x, int32 _y) {txt.SetPosition(_x, _y);}

	//! Draw the timer on an Image.
	void Draw(Image& dest);

	//! Send the output to a viewport.
	void Draw(Viewport& dest);

private:

	//Data members
	Timer timer;			//The timer
	uint32 frames;			//frame counter
	uint32 updatetime;		//update fps counter after this time
	uint16 currentfps;		//Current fps
	Text txt;				//Text object

	//Generate text objects based off time
	void GrabTime();
};

#endif