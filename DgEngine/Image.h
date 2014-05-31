//================================================================================
// @ Image.h
// 
// Class: Image
//
// 32 bits per pixel image class. A simplified version of the SDL_Surface class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef IMAGE_H
#define IMAGE_H

#include "DgTypes.h"
#include <string>

struct SDL_Surface;

//--------------------------------------------------------------------------------
//	@	Image
//--------------------------------------------------------------------------------
class Image
{
	struct Flags;
public:
	//Constructor/Destructor
	Image();
	~Image();

	//Copy operations
	Image(const Image&);
	Image& operator=(const Image&);

	//Resizing functions
	friend void Resize(Image&, uint32 h, uint32 w);

	//! Load Image from file
	bool Load(std::string);

	//! Set Image from SDL_Surface.
	bool Set(SDL_Surface*, bool dealloc = true);

	//Return functions
	uint32 h() const {return mH;}
	uint32 w() const {return mW;}
	uint32 pitch() const {return mW * sizeof(uint32);}
	const uint32_t* pixels() const {return mPixels;}
	uint32_t* pixels() {return mPixels;}

	//Set all pixels to this value
	void Flush(uint32_t val = 0);

private:
	//Data members

	//Pixel data
	uint32_t* mPixels;

	//Dimensions
	uint32 mH, mW;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void SetDefault();

};

#endif