//================================================================================
// @ Image.cpp
// 
// Description:
//
// Definitions for the Image class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Image.h"
#include "DgError.h"
#include "Color.h"
#include "CommonGraphics.h"


//--------------------------------------------------------------------------------
//	@	Image::SetDefault()
//--------------------------------------------------------------------------------
//		Set default values.
//--------------------------------------------------------------------------------
void Image::SetDefault()
{
	//Create a one pixel image by default
	delete[] mPixels;
	mPixels = new uint32[1];
	mPixels[0] = 0x00FF00FF;
	mH = mW = 1;

}	//End: Image::SetDefault()


//--------------------------------------------------------------------------------
//	@	Image::SetFromSDL_Surface()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
bool Image::Set(SDL_Surface* img, bool dealloc)
{
	if (img == NULL)
	{
		SetDefault();
		ERROR_OUT("@Image::SetFromSDL_Surface(SDL_Surface* img, bool dealloc) -> Attemp to construct Image from NULL SDL_Surface pointer.");
		return false;
	}
	else if (img->format->BitsPerPixel != 32)
	{
		SetDefault();
		ERROR_OUT("@Image::SetFromSDL_Surface(SDL_Surface* img, bool dealloc) -> wrong format.");
		return false;
	}

	//Copy dimensions
	mH = img->h;
	mW = img->w;

	//Copy pixels
	delete[] mPixels;
	mPixels = new uint32[mH*mW];
	const uint32* input_pixels = (uint32*)img->pixels;

	for (uint32 i = 0; i < mH*mW; i++)
	{
		mPixels[i] = input_pixels[i];
	}

	//Free source surface if requested
	if (dealloc)
		SDL_FreeSurface(img);

	return true;

}	//End: Image::SetFromSDL_Surface()


//--------------------------------------------------------------------------------
//	@	Image::Image()
//--------------------------------------------------------------------------------
//		Constructor. Images have a default size of 1x1.
//--------------------------------------------------------------------------------
Image::Image(): mPixels(NULL), mH(0), mW(0)
{
	// An Image must be valid. That is it must have at least one pixel.
	SetDefault();

}	//End: Image::Image()


//--------------------------------------------------------------------------------
//	@	Image::Load()
//--------------------------------------------------------------------------------
//		Load an Image from a file
//--------------------------------------------------------------------------------
bool Image::Load(std::string filename)
{
	return Set(LoadImage(filename), true);

}	//End: Image::Image()


//--------------------------------------------------------------------------------
//	@	Image::~Image()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Image::~Image()
{
	delete[] mPixels;

}	//End: Image::~Image()


//--------------------------------------------------------------------------------
//	@	Image::Image()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Image::Image(const Image& other) : mPixels(NULL), mH(other.mH), mW(other.mW)
{
	mPixels = new uint32[mH*mW];
	
	for (uint32 i = 0; i < mH*mW; i++)
		mPixels[i] = other.mPixels[i];

}	//End: Image::Image()


//--------------------------------------------------------------------------------
//	@	Image::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Image& Image::operator=(const Image& other)
{
	if (this == &other)
		return *this;

	//Delete old pixel data
	delete[] mPixels;

	//Assign new data
	mH = other.mH;
	mW = other.mW;

	mPixels = new uint32[mH*mW];

	for (uint32 i = 0; i < mH*mW; i++)
		mPixels[i] = other.mPixels[i];

	return *this;

}	//End: Image::operator=()


//--------------------------------------------------------------------------------
//	@	Image::Flush()
//--------------------------------------------------------------------------------
//		Set all pixels to a value
//--------------------------------------------------------------------------------
void Image::Flush(uint32 val)
{
	std::fill(mPixels, mPixels+mH*mW, val);

}	//End: Image::Flush()


//--------------------------------------------------------------------------------
//	@	Resize()
//--------------------------------------------------------------------------------
//		Resize an Image. Images are shrunk using a pixel averaging method.
//		Enlargements use a bilinear method. Input image is resized.
//--------------------------------------------------------------------------------
void Resize(Image& img, uint32 new_h, uint32 new_w)
{
	//Ensure new dimensions are valid.
	if (new_h == 0)
		new_h = 1;
	if (new_w == 0)
		new_w = 1;

	//If no resize is needed
	if (img.mH == new_h && img.mW == new_w)
	{
		return;
	}

	//Common color object
	Color pxl;

	//--------------------------------------------------------------------------------
	//		Size reduction in both w and h directions
	//--------------------------------------------------------------------------------
	if (new_w <= img.mW && new_h <= img.mH)
	{
		//Create pixel array
		uint32* new_mPixels = new uint32[new_h*new_w];

		//Input dimensions (fixed-point 48:16 format)
		uint64 in_h64 = (uint64(img.mH) << 16);
		uint64 in_w64 = (uint64(img.mW) << 16);

		//Output dimensions (fixed-point 48:16 format)
		uint64 out_h64 = (uint64(new_h) << 16);
		uint64 out_w64 = (uint64(new_w) << 16);

		//Step through each pixel on new surface
		for (uint32 i = 0; i < new_h ; i++)
		{
			//y bounds
			uint32 ys = (i*img.mH) / new_h;
			uint32 ye = ((i+1)*img.mH) / new_h;

			for (uint32 j = 0; j < new_w ; j++)
			{
				//x bounds
				uint32 xs = (j*img.mW) / new_w;
				uint32 xe = ((j+1)*img.mW) / new_w;
				
				//Final pixel colors in 48:16 format
				uint32 red		= 0;
				uint32 green	= 0;
				uint32 blue		= 0;
				uint32 alpha	= 0;

				//Count number of pixels added in
				uint32 rgb_denom = 0;

				//Alpha channel will alway be added in, 
				//(even if pixel is colorkeyed). 
				uint32 a_denom = 0;

				//Add in whole pixels
				for (uint32 ii = ys; ii < ye; ii++)
				{
					for (uint32 jj = xs; jj < xe; jj++)
					{
						//Get current pixel in image
						pxl.i = img.mPixels[(ii)*img.mW + jj];

						//Check if pixel is colorkeyed
						if (pxl == Color::COLORKEY)
						{
							//increment alpha counter
							a_denom++;

							//Do not include
							continue;
						}

						//Add to base colors
						red		+= pxl.rgba.red;
						green	+= pxl.rgba.green;
						blue	+= pxl.rgba.blue;
						alpha	+= pxl.rgba.alpha;

						//Increment counter
						rgb_denom++;
						a_denom++;
					}
				}
				
				uint32 final_pixel;

				//If final pixel is the colorkey
				if (rgb_denom == 0)
				{
					final_pixel = Color::COLORKEY.i;
				}
				else
				{
					//Final colors
					red /= rgb_denom;
					green /= rgb_denom;
					blue /= rgb_denom;
					alpha /= a_denom;

					final_pixel  = ((((((	  alpha << 8)
											| red) << 8)
											| green) << 8)
											| blue);
				}

				//Map to new image
				new_mPixels[i*new_w + j] = final_pixel;

			}
		}
	
		//Assign data to image
		img.mH = new_h;
		img.mW = new_w;
		delete[] img.mPixels;
		img.mPixels = new_mPixels;

		return;

	}	//End: size reduction in w and h

	
	
	//--------------------------------------------------------------------------------
	//		Width reduction only, height will be enlarged later
	//--------------------------------------------------------------------------------
	if (new_w < img.mW)
	{
		//Create pixel array
		uint32* new_mPixels = new uint32[img.mH*new_w];

		//Input dimensions (fixed-point 48:16 format)
		uint64 in_h64 = (uint64(img.mH) << 16);
		uint64 in_w64 = (uint64(img.mW) << 16);

		//Output dimensions (fixed-point 48:16 format)
		uint64 out_h64 = (uint64(img.mH) << 16);
		uint64 out_w64 = (uint64(new_w) << 16);

		//Step through each pixel on new surface
		for (uint32 i = 0; i < img.mH ; i++)
		{
			for (uint32 j = 0; j < new_w ; j++)
			{
				//x bounds
				uint32 xs = (j*img.mW) / new_w;
				uint32 xe = ((j+1)*img.mW) / new_w;
				
				//Final pixel colors in 48:16 format
				uint32 red		= 0;
				uint32 green	= 0;
				uint32 blue		= 0;
				uint32 alpha	= 0;

				//Count number of pixels added in
				uint32 rgb_denom = 0;

				//Alpha channel will alway be added in, 
				//(even if pixel is colorkeyed). 
				uint32 a_denom = 0;

				//Add in whole pixels
				for (uint32 jj = xs; jj < xe; jj++)
				{
					//Get current pixel in image
					pxl.i = img.mPixels[i*img.mW + jj];

					//Check if pixel is colorkeyed
					if (pxl == Color::COLORKEY)
					{
						//increment alpha counter
						a_denom++;

						//Do not include
						continue;
					}

					//Add to base colors
					red		+= pxl.rgba.red;
					green	+= pxl.rgba.green;
					blue	+= pxl.rgba.blue;
					alpha	+= pxl.rgba.alpha;

					//Increment counter
					rgb_denom++;
					a_denom++;
				}
				
				
				uint32 final_pixel;

				//If final pixel is the colorkey
				if (rgb_denom == 0)
				{
					final_pixel = Color::COLORKEY.i;
				}
				else
				{
					//Final colors
					red /= rgb_denom;
					green /= rgb_denom;
					blue /= rgb_denom;
					alpha /= a_denom;

					final_pixel  = ((((((	  alpha << 8)
											| red) << 8)
											| green) << 8)
											| blue);
				}

				//Map to new image
				new_mPixels[i*new_w + j] = final_pixel;

			}
		}
	
		//Assign data to image
		img.mW = new_w;
		delete[] img.mPixels;
		img.mPixels = new_mPixels;

		//If no hieght change, done
		if (new_h == img.mH)
		{
			return;
		}

	}	//End: size reduction in w


	////--------------------------------------------------------------------------------
	////		Height reduction only, width is enlarged later
	////--------------------------------------------------------------------------------
	if (new_h < img.mH)
	{
		//Create pixel array
		uint32* new_mPixels = new uint32[img.mW*new_h];

		//Input dimensions (fixed-point 48:16 format)
		uint64 in_h64 = (uint64(img.mH) << 16);
		uint64 in_w64 = (uint64(img.mW) << 16);

		//Output dimensions (fixed-point 48:16 format)
		uint64 out_h64 = (uint64(new_h) << 16);
		uint64 out_w64 = (uint64(img.mW) << 16);

		//Step through each pixel on new surface
		for (uint32 i = 0; i < new_h ; i++)
		{
			//y bounds
			uint32 ys = (i*img.mH) / new_h;
			uint32 ye = ((i+1)*img.mH) / new_h;

			for (uint32 j = 0; j < img.mW ; j++)
			{
				//Final pixel colors in 48:16 format
				uint32 red		= 0;
				uint32 green	= 0;
				uint32 blue		= 0;
				uint32 alpha	= 0;

				//Count number of pixels added in
				uint32 rgb_denom = 0;

				//Alpha channel will alway be added in, 
				//(even if pixel is colorkeyed). 
				uint32 a_denom = 0;

				//Add in whole pixels
				for (uint32 ii = ys; ii < ye; ii++)
				{
					//Get current pixel in image
					pxl.i = img.mPixels[(ii)*img.mW + j];

					//Check if pixel is colorkeyed
					if (pxl == Color::COLORKEY)
					{
						//increment alpha counter
						a_denom++;

						//Do not include
						continue;
					}

					//Add to base colors
					red		+= pxl.rgba.red;
					green	+= pxl.rgba.green;
					blue	+= pxl.rgba.blue;
					alpha	+= pxl.rgba.alpha;

					//Increment counter
					rgb_denom++;
					a_denom++;
					
				}
				
				uint32 final_pixel;

				//If final pixel is the colorkey
				if (rgb_denom == 0)
				{
					final_pixel = Color::COLORKEY.i;
				}
				else
				{
					//Final colors
					red /= rgb_denom;
					green /= rgb_denom;
					blue /= rgb_denom;
					alpha /= a_denom;

					final_pixel  = ((((((	  alpha << 8)
											| red) << 8)
											| green) << 8)
											| blue);
				}

				//Map to new image
				new_mPixels[i*img.mW + j] = final_pixel;

			}
		}
	
		//Assign data to image
		img.mH = new_h;
		delete[] img.mPixels;
		img.mPixels = new_mPixels;
		
		//If no width change, done
		if (new_w == img.mW)
		{
			return;
		}

	}	//End: size reduction in h

	//--------------------------------------------------------------------------------
	//		If the function has made it this far, some enlarging will be required.
	//--------------------------------------------------------------------------------
	
	//Create final pixel array
	uint32* dest = new uint32 [new_w*new_h];

	//--------------------------------------------------------------------------------
	//		Enlarging, Bilinear method
	//--------------------------------------------------------------------------------
	
	//Final pixel colors in 48:16 format
	uint64 red00, red10, red01, red11, redF;
	uint64 green00, green10, green01, green11, greenF;
	uint64 blue00, blue10, blue01, blue11, blueF;
	uint64 alpha00, alpha10, alpha01, alpha11, alphaF;

	//Loop through all destination pixels
	for (uint32 i = 0; i < new_h; i++)
	{
		for (uint32 j = 0; j < new_w; j++)
		{
			//texel cordinates: fixed-point 16:16 format
			uint32 u = uint32( uint64(img.mW)*((uint64(j) << 16) / new_w) );
			uint32 v = uint32( uint64(img.mH)*((uint64(i) << 16) / new_h) );

			//Bottom left texel coord on image
			uint32 u_int = u >> 16;
			uint32 v_int = v >> 16;

			//48:16 format
			uint64 u_frac = uint64( uint16(u) );
			uint64 v_frac = uint64( uint16(v) );
			uint64 one_u_frac = 65536 - u_frac;
			uint64 one_v_frac = 65536 - v_frac;

			//Get colors
			Color pxl00, pxl10, pxl01, pxl11;
			pxl00.i = img.mPixels[(v_int)*img.mW + u_int];
			pxl01.i = img.mPixels[(v_int+1)*img.mW + u_int];
			pxl10.i = img.mPixels[(v_int)*img.mW + u_int + 1];
			pxl11.i = img.mPixels[(v_int+1)*img.mW + u_int + 1];

			//Do not add any colorkeyed pixels
			
			//if all are coloykeyed
			if (pxl00 == Color::COLORKEY && pxl10 == Color::COLORKEY &&
				pxl01 == Color::COLORKEY && pxl11 == Color::COLORKEY)
			{
				//New pixel is colorkey
				dest[i*new_w + j] = Color::COLORKEY.i;

				continue;
			}

			//if at least one pixel is coloykeyed
			if (pxl00 == Color::COLORKEY || pxl10 == Color::COLORKEY ||
				pxl01 == Color::COLORKEY || pxl11 == Color::COLORKEY)
			{
				//Get the average pixel from all non colorkey pixels
				uint32 pxl_avg_r = 0;
				uint32 pxl_avg_g = 0;
				uint32 pxl_avg_b = 0;
				uint32 pxl_avg_a = 0;
				Color pxl_avg;

				//Counter
				uint32 counter = 0;

				//Add in colors if not colorkey
				if (pxl00 != Color::COLORKEY)
				{
					pxl_avg_r += uint32(pxl00.rgba.red);
					pxl_avg_g += uint32(pxl00.rgba.green);
					pxl_avg_b += uint32(pxl00.rgba.blue);
					pxl_avg_a += uint32(pxl00.rgba.alpha);

					counter++;
				}
				if (pxl10 != Color::COLORKEY)
				{
					pxl_avg_r += uint32(pxl10.rgba.red);
					pxl_avg_g += uint32(pxl10.rgba.green);
					pxl_avg_b += uint32(pxl10.rgba.blue);
					pxl_avg_a += uint32(pxl10.rgba.alpha);

					counter++;
				}
				if (pxl01 != Color::COLORKEY)
				{
					pxl_avg_r += uint32(pxl01.rgba.red);
					pxl_avg_g += uint32(pxl01.rgba.green);
					pxl_avg_b += uint32(pxl01.rgba.blue);
					pxl_avg_a += uint32(pxl01.rgba.alpha);

					counter++;
				}
				if (pxl11 != Color::COLORKEY)
				{
					pxl_avg_r += uint32(pxl11.rgba.red);
					pxl_avg_g += uint32(pxl11.rgba.green);
					pxl_avg_b += uint32(pxl11.rgba.blue);
					pxl_avg_a += uint32(pxl11.rgba.alpha);

					counter++;
				}

				//Find average value
				pxl_avg.rgba.red = uint8(pxl_avg_r/counter);
				pxl_avg.rgba.green = uint8(pxl_avg_g/counter);
				pxl_avg.rgba.blue = uint8(pxl_avg_b/counter);
				pxl_avg.rgba.alpha = uint8(pxl_avg_a/4);

				//Set any of the original 4 pixels to this new color average
				//if they are colorkeyed
				if (pxl00 == Color::COLORKEY)
					pxl00 = pxl_avg;
				if (pxl01 == Color::COLORKEY)
					pxl01 = pxl_avg;
				if (pxl10 == Color::COLORKEY)
					pxl10 = pxl_avg;
				if (pxl11 == Color::COLORKEY)
					pxl11 = pxl_avg;

			}

			red00 = uint64(pxl00.rgba.red) << 16;
			green00 = uint64(pxl00.rgba.green) << 16;
			blue00 = uint64(pxl00.rgba.blue) << 16;
			alpha00 = uint64(pxl00.rgba.alpha) << 16;

			red01 = uint64(pxl01.rgba.red) << 16;
			green01 = uint64(pxl01.rgba.green) << 16;
			blue01 = uint64(pxl01.rgba.blue) << 16;
			alpha01 = uint64(pxl01.rgba.alpha) << 16;

			red10 = uint64(pxl10.rgba.red) << 16;
			green10 = uint64(pxl10.rgba.green) << 16;
			blue10 = uint64(pxl10.rgba.blue) << 16;
			alpha10 = uint64(pxl10.rgba.alpha) << 16;

			red11 = uint64(pxl11.rgba.red) << 16;
			green11 = uint64(pxl11.rgba.green) << 16;
			blue11 = uint64(pxl11.rgba.blue) << 16;
			alpha11 = uint64(pxl11.rgba.alpha) << 16;

			redF = ((	(((red00*one_u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((red10*u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((red01*one_u_frac) >> 16)*v_frac) >> 16) + 
					(	(((red11*u_frac) >> 16)*v_frac) >> 16) ) >> 16; 

			greenF = ((	(((green00*one_u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((green10*u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((green01*one_u_frac) >> 16)*v_frac) >> 16) + 
					(	(((green11*u_frac) >> 16)*v_frac) >> 16) ) >> 16;  

			blueF = ((	(((blue00*one_u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((blue10*u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((blue01*one_u_frac) >> 16)*v_frac) >> 16) + 
					(	(((blue11*u_frac) >> 16)*v_frac) >> 16) ) >> 16; 

			alphaF = ((	(((alpha00*one_u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((alpha10*u_frac) >> 16)*one_v_frac) >> 16) + 
					(	(((alpha01*one_u_frac) >> 16)*v_frac) >> 16) + 
					(	(((alpha11*u_frac) >> 16)*v_frac) >> 16) ) >> 16; 

			//Build final pixel
			uint32 final_pixel = ((((((	  uint8(alphaF) << 8)
										| uint8(redF)) << 8)
										| uint8(greenF)) << 8)
										| uint8(blueF));

			//Map to new image
			dest[i*new_w + j] = final_pixel;
		}
	}

	//Delete old pixel data
	delete[] img.mPixels;

	//Assign dest pointer to mPixels pointer
	img.mPixels = dest;

	//Assign sizes
	img.mW = new_w;
	img.mH = new_h;

}	//End: Resize()
