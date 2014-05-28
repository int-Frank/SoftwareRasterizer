//================================================================================
// @ Mipmap.cpp
// 
// Description:
//
// Definitions for the Mipmap class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Mipmap.h"
#include "CommonMath.h"


//--------------------------------------------------------------------------------
//	@	Mipmap::SetDefault()
//--------------------------------------------------------------------------------
//		Set default values.
//--------------------------------------------------------------------------------
void Mipmap::SetDefault()
{
	//Create one mipmap of a default image
	mMipmaps.clear();
	mMipmaps.push_back(Image());

	baseW = baseH = 1;
	baseArea = 1.0f;
	number = 1;

}	//End: Image::SetDefault()


//--------------------------------------------------------------------------------
//	@	Mipmap::Mipmap
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Mipmap::Mipmap()
{
	//A mipmap must be valid. It must have at least one Image in the image list.
	SetDefault();
}	//End: Mipmap::Mipmap()


//--------------------------------------------------------------------------------
//	@	Mipmap::Mipmap()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
bool Mipmap::Load(std::string filename)
{
	Image temp;
	if (!temp.Load(filename))
	{
		SetDefault();
		return false;
	}
	
	SetFromImage(temp);

	return true;

}	//End: Mipmap::Mipmap()


//--------------------------------------------------------------------------------
//	@	Mipmap::init()
//--------------------------------------------------------------------------------
//		Initialize from Image
//--------------------------------------------------------------------------------
void Mipmap::SetFromImage(const Image& input)
{
	//Clear current mipmap list
	mMipmaps.clear();

	//Get input dimensions
	uint32 h = input.h();
	uint32 w = input.w();

	//Calculate data
	baseW = w;
	baseH = h;
	baseArea = float(w)*float(h);

	//Initiate data
	number = 0;

	//Loop through, adding mipmaps
	while (true)
	{
		//Add current image
		Image tempImage(input);
		Resize(tempImage, h, w);
		mMipmaps.push_back(tempImage);

		//Increment number
		number++;

		//Calculate new dimensions
		h = (h >> 1);
		w = (w >> 1);

		//Check if last mipmap
		if (h == 0 && w == 0)
			break;

		//Make sure all dimensions > 0
		h = DgMax(1, h);
		w = DgMax(1, w);

	}
}	//End: Mipmap::init()


//--------------------------------------------------------------------------------
//	@	Mipmap::~Mipmap()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Mipmap::~Mipmap()
{
}	//End: Mipmap::~Mipmap()


//--------------------------------------------------------------------------------
//	@	Mipmap::Mipmap()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Mipmap::Mipmap(const Mipmap& other): baseW(other.baseW), baseH(other.baseH),
	baseArea(other.baseArea), number(other.number), mMipmaps(other.mMipmaps)
{
}	//End: Mipmap::Mipmap()


//--------------------------------------------------------------------------------
//	@	Mipmap::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Mipmap& Mipmap::operator=(const Mipmap& other)
{
	if (this == &other)
		return *this;

	//Assign data
	baseW = other.baseW;
	baseH = other.baseH;
	baseArea = other.baseArea;
	number = other.number;
	mMipmaps = other.mMipmaps;

	return *this;

}	//End: Mipmap::operator=()


//--------------------------------------------------------------------------------
//	@	Mipmap::GetImageByRef()
//--------------------------------------------------------------------------------
//		Get Image by reference
//--------------------------------------------------------------------------------
const Image* Mipmap::GetImageByRef(uint8 ref) const
{
	//Range check
	if (ref >= number)
		return &mMipmaps[number - 1];

	return &mMipmaps[ref];

}	//End: Mipmap::GetDgImage()


//--------------------------------------------------------------------------------
//	@	Mipmap::GetLast()
//--------------------------------------------------------------------------------
//		Get the last mipmap in the list
//--------------------------------------------------------------------------------
const Image* Mipmap::GetLast() const
{
	return &mMipmaps[number - 1];

}	//End: Mipmap::GetDgImage()


//--------------------------------------------------------------------------------
//	@	Mipmap::GetImageByArea()
//--------------------------------------------------------------------------------
//		Choose image base on ratio of areas
//		input_ratio = texture_area / screen_pixels
//--------------------------------------------------------------------------------
const Image* Mipmap::GetImageByArea(float input_ratio) const
{
	//Find area ratio

	//Cannot cast float to uint32 for some reason. Causes screen flicker. Corrupt stack?
	uint32 ratio = uint32(baseArea * input_ratio);
	uint8 ref = uint8(DgLog2(ratio));

	ref = (ref >> 1);
	
	//Range check
	ref = DgMin(ref, (number-1));

	//Return pointer
	return &mMipmaps[ref];

}	//End: Mipmap::GetImageByArea()

