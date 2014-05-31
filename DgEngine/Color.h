//================================================================================
// @ Color.h
// 
// Class: Color
//
// A color object stores a color in both rbga and uint32 format. 
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef COLOR_H
#define COLOR_H

#include "DgTypes.h"
#include "SDL.h"
#include "Dg_io.h"

//--------------------------------------------------------------------------------
//	@	Color
//--------------------------------------------------------------------------------
struct Color
{
	//DATA
	union 
	{
		struct RGBA
		{
      uint8_t blue;
      uint8_t green;
      uint8_t red;
      uint8_t alpha;
		} rgba;			//Uint8 rgba values

    uint32_t i;			//Uint32 value

	};


	//Constructor
	Color(): i(0) {}
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0);
  Color(uint32_t clr) : i(clr) {}
	
	//Copy operations
	Color(const Color& clr): i(clr.i) {}
	Color& operator= (const Color&);

	//Input/output
	friend DgReader& operator>> (DgReader& in, Color& c);
	friend DgWriter& operator<< (DgWriter& out, const Color& c);

	//Conversion
	operator SDL_Color() const;

	//Comparison
	bool operator==(const Color& a) const;
	bool operator!=(const Color& a) const;

	static const Color COLORKEY;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color YELLOW;
	static const Color CYAN;
	static const Color INDIGO;
	static const Color BLACK;
	static const Color WHITE;
};


#endif