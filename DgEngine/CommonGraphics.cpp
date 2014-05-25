//================================================================================
// @ CommonGraphics.cpp
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

#include <string>
#include "Color.h"
#include "SDL.h"
#include "SDL_image.h"
#include "CommonGraphics.h"
#include "CommonMath.h"
#include "Image.h"
#include "Dg_io.h"
#include "DgTypes.h"
#include "DgRect.h"


//--------------------------------------------------------------------------------
//		Given a path and file name, this function attempts to load an image.
//		Supports png, jpeg, bmp file formats. If failed to load, logs error
//		and returns NULL.
//--------------------------------------------------------------------------------
SDL_Surface* LoadImage(std::string filename)
{
	//The image that's loaded
    SDL_Surface* loadedImage(NULL);

    //The optimized surface that will be used
    SDL_Surface* optimizedImage(NULL);

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded

    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_ConvertSurfaceFormat( loadedImage,
			SDL_PIXELFORMAT_ARGB8888, 0);

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, 
							 SDL_TRUE, 
							 SDL_MapRGBA( optimizedImage->format, 
							 Color::COLORKEY.rgba.red,
							 Color::COLORKEY.rgba.green,
							 Color::COLORKEY.rgba.blue,
							 Color::COLORKEY.rgba.alpha) );
        }
		else
		{
            std::cerr << "@LoadImageSDL(): failed to optimise image: " << filename << std::endl;
		}
    }
	else
	{
        std::cerr << "@LoadImageSDL(): failed to load image: " << filename << std::endl;
	}

    return optimizedImage;

}	//End: LoadImage()


//--------------------------------------------------------------------------------
//		Adjust rectangle to fit in the bounds w,h. Modifies input. Returns false
//		if the input rectangle is completly outside the bounds.
//--------------------------------------------------------------------------------
bool ClipRect(DgRect& rect, int32 w, int32 h)
{
	int rx = rect.x;
	int ry = rect.y;
	int rh = rect.h;
	int rw = rect.w;

	//Clip to width
	if (rx > w)
		return false;
	else if ((rx < 0))
	{
		if (rx + rw < 0)
			return false;
		
		rx = 0;
	}
	if (rx + rw > w)
		rw = w - rx;
	
	//Clip to width
	if (ry > h)
		return false;
	else if ((ry < 0))
	{
		if (ry + rh < 0)
			return false;
		
		ry = 0;
	}
	if (ry + rh > h)
		rh = h - ry;

	//Assign values
	rect.x = rx;
	rect.y = ry;
	rect.h = rh;
	rect.w = rw;

	return true;

}	//End: AdjustRect()


//--------------------------------------------------------------------------------
//		Adjust rectangle to fit in the bounds w,h. Modifies input. Returns false
//		if the input rectangle is completly outside the bounds.
//--------------------------------------------------------------------------------
bool ClipRect(SDL_Rect& sdlr, int32 w, int32 h)
{
	DgRect dgr(sdlr);

	if (!ClipRect(dgr, w, h))
		return false;

	SetSDL_Rect(sdlr, dgr);

	return true;
}	//End: AdjustRect()


//--------------------------------------------------------------------------------
//		Blits an image onto another.
//--------------------------------------------------------------------------------
void ApplyImage(const Image& src, Image& dest, 
				int x, int y, DgGraphics::BlendType type)
{
	//Get pixel data
	uint32 *dest_pixels = (uint32*)dest.pixels();
	uint32 *src_pixels = (uint32*)src.pixels();

	//Extract coordinates
	int src_w = src.w();
	int src_h = src.h();

	int dest_w = dest.w();
	int dest_h = dest.h();

	int src_x_min;
	int src_x_max;
	int src_y_min;
	int src_y_max;

	int dest_x_min = DgMax(0, x);
	int dest_x_max = DgMin(dest_w, (x+src_w));
	int dest_y_min = DgMax(0, y);
	int dest_y_max = DgMin(dest_h, (y+src_h));

	//Set min limits
	if (x < 0)
		src_x_min = -x;
	else
		src_x_min = 0;
	if (y < 0)
		src_y_min = -y;
	else
		src_y_min = 0;

	//Set max limits
	if (x + src_w > dest_w)
		src_x_max = dest_w - x;
	else
		src_x_max = src_w;
	if (y + src_h > dest_h)
		src_y_max = dest_h - y;
	else
		src_y_max = src_h;

	switch (type)
	{
		//--------------------------------------------------------------------------------
		//		Color key
		//--------------------------------------------------------------------------------
	case DgGraphics::COLORKEY:
		{
			for (int ys = src_y_min, yd = dest_y_min;
				ys < src_y_max; ys++, yd++)
			{
				for (int xs = src_x_min, xd = dest_x_min;
					xs < src_x_max; xs++, xd++)
				{
					//Current pixel
					uint32 src_p = src_pixels[ys*src_w + xs];

					//Check for colorkey
					if (src_p == Color::COLORKEY.i)
						continue;

					dest_pixels[yd*dest_w + xd] = src_p;
			
				}
			}
			break;
		}

		//--------------------------------------------------------------------------------
		//		Alpha key
		//--------------------------------------------------------------------------------
	case DgGraphics::ALPHA:
		{
			for (int ys = src_y_min, yd = dest_y_min;
				ys < src_y_max; ys++, yd++)
			{

				uint32 ys_ref = ys * src_w;
				uint32 yd_ref = yd * dest_w;

				for (int xs = src_x_min, xd = dest_x_min;
					xs < src_x_max; xs++, xd++)
				{
					//Get source pixel
					uint32 cs(src_pixels[ys_ref + xs]);

					//Get source pixel color
					uint32 as = ((cs & 0xFF000000) >> 16);
					uint32 rs = ((cs & 0x00FF0000) >> 16) * as;
					uint32 gs = ((cs & 0x0000FF00) >> 8) * as;
					uint32 bs = (cs & 0x000000FF) * as;
				
					//Get desination pixel
					uint32 ind = yd_ref + xd;
					uint32 cd = dest_pixels[ind];

					//Get destination pixel color
					uint32 ad = ((cd & 0xFF000000) >> 16);
					ad = ad * (0xFFFF - as) >> 16;
					uint32 rd = ((cd & 0x00FF0000) >> 16) * ad;
					uint32 gd = ((cd & 0x0000FF00) >> 8) * ad;
					uint32 bd = (cd & 0x000000FF) * ad;

					//Add to desination pixel array
					dest_pixels[ind] = (((
						(((ad + as) << 16) & 0xFF000000)
						| ((rs + rd) & 0x00FF0000))
						| (((gs + gd) >> 8) & 0x0000FF00))
						| (((bs + bd) >> 16) & 0x000000FF));
			
				}
			}
			break;
		}

		//--------------------------------------------------------------------------------
		//		Straight blit, no checks
		//--------------------------------------------------------------------------------
	case DgGraphics::NONE:
		{
			for (int ys = src_y_min, yd = dest_y_min;
				ys < src_y_max; ys++, yd++)
			{
				for (int xs = src_x_min, xd = dest_x_min;
					xs < src_x_max; xs++, xd++)
				{
					dest_pixels[yd*dest_w + xd] = 
						src_pixels[ys*src_w + xs];
				}
			}
			break;
		}
	}
}	//End: ApplyImage()


//--------------------------------------------------------------------------------
//		Draws a straight line onto an Image
//--------------------------------------------------------------------------------
void DrawLine(	Image dest, 
				int32 x0, int32 y0, 
				int32 x1, int32 y1, const Color& C)
{
	//Get pixel array
	uint32* pixels = dest.pixels();
	int32 width = int32(dest.w());

	int32 dx = abs(x1-x0);
    int32 dy = abs(y1-y0); 
	int32 sx, sy;

    if (x0 < x1)
		sx = 1;
	else 
		sx = -1;
    if (y0 < y1)
		sy = 1; 
	else 
		sy = -1;
    
	int32 err = dx-dy;
 
	for (int32 i = 0; i < std::max(dx,dy); i++)
	{
		//Set the pixel
		pixels[width*y0 + x0] = C.i;

		if (x0 == x1 && y0 == y1)
			break;

		int32 e2 = 2*err;

		if (e2 > -dy) 
		{ 
			err -= dy;
			x0 += sx;
		}
		if (e2 <  dx) 
		{  
			err += dx;
			y0 += sy; 
		}
	}

}	//End: DrawLine()