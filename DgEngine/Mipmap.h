//================================================================================
// @ Mipmap.h
// 
// Class: Mipmap
//
// A mipmap is a scaled down version of an original Image that can be used 
// when required to reduce the number of texels per fragment when mapping the 
// texture to the polygon. Each subsequent image in the list has dimensions
// half of the previous, until both height and width are 1. 
// The base image can have any dimensions.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef Mipmap_H
#define Mipmap_H

#include "DgTypes.h"
#include "Image.h"
#include <vector>
#include <string>

//--------------------------------------------------------------------------------
//	@	Mipmap
//--------------------------------------------------------------------------------
class Mipmap
{
	struct Flags;
public:

	//Constructor/Destructor
	Mipmap();
	~Mipmap();

	//Copy operations
	Mipmap(const Mipmap&);
	Mipmap& operator=(const Mipmap&);

	//! Load Image from file
	bool Load(std::string);

	//! Set the mipmap from an image
	void SetFromImage(const Image&);

	//Return an image from the array
	const Image* GetImageByArea(float) const;
	const Image* GetImageByRef(uint8 ref) const;
	const Image* GetLast() const;

	//Return functions
	uint8 GetNumber()	const						{return number;}
	void GetBaseSize(uint32& w, uint32& h) const	{w = baseW; h = baseH;}

	static const Mipmap DEFAULT;

private:
	//Data members
	uint32 baseW, baseH;	//eg. 256 x 256
	float baseArea;
	uint8 number;			//Number of mipmaps

	//The images
	std::vector<Image> mMipmaps;

private:
	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void Mipmap::SetDefault();
};

#endif