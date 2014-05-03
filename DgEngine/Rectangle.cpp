/*!
* @file Rectangle.cpp
*
* @author Frank Hart
* @date 22/01/2014
*
* Class definitions: Rectangle
*/

#include "Rectangle.h"
#include "Image.h"
#include "CommonGraphics.h"

//--------------------------------------------------------------------------------
//	@	Rectangle::Rectangle()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Rectangle::Rectangle(): rect(DgRect(0,0,1,1)), color(0xFFFFFFFF)
{
}


//--------------------------------------------------------------------------------
//	@	Rectangle::Rectangle()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Rectangle::Rectangle(uint32 w, uint32 h, int32 x, int32 y, uint32 c) :
		rect(DgRect(x,y,w,h)), color(c)
{
}


//--------------------------------------------------------------------------------
//	@	Rectangle::Draw()
//--------------------------------------------------------------------------------
//		Draw the shape to an image
//--------------------------------------------------------------------------------
void Rectangle::Draw(Image& img) const
{
	//Clip the rectangle to the image bounds
	DgRect temp(rect);
	ClipRect(temp, img.w(), img.h());

	uint32 *pixels = img.pixels();
	uint32 pitch = img.w();

	uint32 x_min = uint32(temp.x);
	uint32 x_length = x_min + temp.w - x_min + 1;

	uint32 y = uint32(temp.y) * pitch;
	uint32  y_max = (uint32(temp.y) + temp.h + 1 ) * pitch;
	
	//If opaque
	if ((color & 0xFF00000) == 0xFF000000)
	{
		for (; y < y_max; y += pitch)
		{
			uint32 ref = y + x_min;
			uint32 ref_max = ref + x_length;

			for (; ref < ref_max; ++ref)
			{
				pixels[ref] = color;
			}
		}
	}
	else
	{
		uint32 at = (color & 0xFF000000) >> 16;
		uint32 one_minus_at = 0xFFFF - at;
		uint32 rt =   ((color & 0x00FF0000) >> 16) * at;
		uint32 gt = ((color & 0x0000FF00) >> 8)  * at;
		uint32 bt =  ((color & 0x000000FF))       * at;

		for (; y < y_max; y += pitch)
		{
			uint32 ref = y + x_min;
			uint32 ref_max = ref + x_length;

			for (; ref < ref_max; ++ref)
			{
				uint32 pxl = pixels[ref];
				uint32 ab = (pxl & 0xFF000000) >> 16;
				ab = (ab * one_minus_at) >> 16;
				uint32 rb = ((pxl >> 16) & 0xFF) * ab;
				uint32 gb = ((pxl >> 8) & 0xFF) * ab;
				uint32 bb = ((pxl)& 0xFF) * ab;

				pixels[ref] = (((
					(((at + ab) << 16) & 0xFF000000)
					| ((rt + rb) & 0x00FF0000))
					| (((gt + gb) >> 8) & 0x0000FF00))
					| (((bt + bb) >> 16) & 0x000000FF));
			}
		}
	}
}