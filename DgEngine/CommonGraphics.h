//================================================================================
// @ CommonGraphics.h
// 
// Description: 
//
// Useful functions, enums and definitions for working with
// graphics, particularly Images.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef COMMONGRAPHICS_H
#define COMMONGRAPHICS_H

#include "Color.h"
#include "DgTypes.h"
#include <string>

struct SDL_Surface;
struct SDL_Rect;
struct DgRect;
class Image;

//--------------------------------------------------------------------------------
//		The different methods an image or pixel can be blended onto another.
//		NONE: no blend, straight override
//		ALPHA: pixels are blended together using their alpha components
//		COLORKEY: all pixels save for the color key are copied.
//--------------------------------------------------------------------------------
namespace DgGraphics
{
	enum BlendType{NONE, ALPHA, COLORKEY};
}


//--------------------------------------------------------------------------------
//		Given a path and file name, this function attempts to load an image.
//		Supports png, jpeg, bmp file formats. If failed to load, logs error
//		and returns NULL.
//--------------------------------------------------------------------------------
SDL_Surface* LoadImage(std::string filename);


//--------------------------------------------------------------------------------
//		Adjust rectangle to fit in the bounds w,h. Modifies input. Returns false
//		if the input rectangle is completly outside the bounds.
//--------------------------------------------------------------------------------
bool ClipRect(DgRect&, int32 w, int32 h);
bool ClipRect(SDL_Rect&, int32 w, int32 h);


//--------------------------------------------------------------------------------
//		Blits an image onto another.
//--------------------------------------------------------------------------------
void ApplyImage(	const Image& source, Image& destination, 
					int32 x, int32 y, DgGraphics::BlendType = DgGraphics::BlendType::NONE);


//--------------------------------------------------------------------------------
//		Draws a straight line onto an Image
//--------------------------------------------------------------------------------
void DrawLine(	Image destination, 
				int32 x0, int32 y0, 
				int32 x1, int32 y1, const Color&);


#endif