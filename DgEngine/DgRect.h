/*!
* @file DgRect.h
*
* @author Frank Hart
* @date 2/02/2014
*
* Class header: DgRect
*/
#ifndef DGRECT_H
#define DGRECT_H

#include "DgTypes.h"
#include "SDL.h"

/*!
* @ingroup utility_other
*
* @class DgRect
*
* @brief A rectangle is defined by position int32(x,y) and size uint32(h,w)
*
* @author Frank Hart
* @date 2/01/2014
*/
struct DgRect
{
	DgRect(){}
	DgRect(int32 _x, int32 _y, uint32 _w, uint32 _h): 
		x(_x), y(_y), w(_w), h(_h) {}

	/*!
	 * @brief Constructs a DgRect from an SDL_Rect.
	 */
	DgRect(const SDL_Rect& sdlr): x(int32(sdlr.x)), y(int32(sdlr.y)), 
		w(uint32(sdlr.w)), h(uint32(sdlr.h)) {}

	int32 x;
	int32 y;
	uint32 h;
	uint32 w;
};

//--------------------------------------------------------------------------------
//	@	SetSDL_Rect()
//--------------------------------------------------------------------------------
/*!
* @ingroup utility_other
* @brief Converts a DgRect to a SDL_Rect
*/
inline void SetSDL_Rect(SDL_Rect& sdlr, const DgRect& dgr)
{
	sdlr.x = int16(dgr.x);
	sdlr.y = int16(dgr.y);
	sdlr.w = uint16(dgr.w);
	sdlr.h = uint16(dgr.h);

}	//End: SetSDL_Rect()

#endif