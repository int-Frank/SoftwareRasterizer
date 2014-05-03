#include "Rasterizer.h"
#include "CommonMath.h"
#include "Polygon_RASTER.h"
#include "Polygon_RASTER_SB.h"
#include "Vertex_RASTER.h"
#include "DgTypes.h"
#include "DgError.h"
#include "Materials.h"
#include "Image.h"
#include "rasterizer_defines.h"


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_SOLID()
//--------------------------------------------------------------------------------
//		Draws a solid fill triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_SOLID()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			//Increment limits
			IncrementTriangle((y_start - output_H + 1));
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xs != xe)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//x-range
			int64 dx = int64((xe - xs));

			//Find interpolants 
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants

			int32 zi = zs;

			//Precalculate
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment z interpolant
				zi += (dz * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//Check Z-buffer
				int32 ref = SCREEN_W_by_y + x;

				if (zi > zBuffer[ref])
				{
					//Set pixel
					output_pixels[ref] = DEFAULT_COLOR;

					//Set Z-buffer element
					zBuffer[ref] = zi;
				}

				//Increment z interpolant
				zi += dz;
			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

	}
}	//End: Rasterizer::InnerLoop_SOLID()

//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE()
//--------------------------------------------------------------------------------
//		Draws a perspective-correct textured triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//x-range
			int64 dx = int64((xe - xs));

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;

			//Precalculate
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//Check Z-buffer
				int32 ref = SCREEN_W_by_y + x;

				if (zi > zBuffer[ref])
				{
					//Get texel
					int32 u = (ui / (zi >> ZU_SHFT)) & u_bit;
					int32 v = (vi / (zi >> ZV_SHFT)) & v_bit;

					//Set pixel
					output_pixels[ref] = pixels[image_w*v + u];

					//Set Z-buffer element
					zBuffer[ref] = zi;
				}

				//Increment values
				ui += du;
				vi += dv;
				zi += dz;
			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

	}
}	//End: Rasterizer::InnerLoop_PERSPECTIVE()


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE()
//--------------------------------------------------------------------------------
//		Draws a perspective-correct, transparent textured triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_ALPHAM()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//x-range
			int64 dx = int64((xe - xs));

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;

			//Precalculate
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//Check Z-buffer
				int32 ref = SCREEN_W_by_y + x;

				if (zi > zBuffer[ref])
				{
					//Get texel
					int32 u = (ui / (zi >> ZU_SHFT)) & u_bit;
					int32 v = (vi / (zi >> ZV_SHFT)) & v_bit;

					//Get the pixel
					uint32 pxl1 = pixels[image_w*v + u];

					//Get the color values in 16:16 format
					uint32 red1 = ((pxl1 >> 16) & 0xFF)  * alphaMaster_top;
					uint32 green1 = ((pxl1 >> 8) & 0xFF) * alphaMaster_top;
					uint32 blue1 = (pxl1 & 0xFF)		 * alphaMaster_top;

					//Get the color values in 16:16 format
					uint32 pxl2 = output_pixels[ref];
					uint32 red2 = ((pxl2 >> 16) & 0xFF)  * alphaMaster_bottom;
					uint32 green2 = ((pxl2 >> 8) & 0xFF) * alphaMaster_bottom;
					uint32 blue2 = (pxl2 & 0xFF)		 * alphaMaster_bottom;

					output_pixels[ref] = (((
						A_BITMASK
						| ((red1 + red2) & R_BITMASK))
						| (((green1 + green2) >> 8) & G_BITMASK))
						| (((blue1 + blue2) >> 16) & B_BITMASK));
				}

				//Increment values
				ui += du;
				vi += dv;
				zi += dz;
			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

	}
}	//End: Rasterizer::InnerLoop_PERSPECTIVE()


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_ALPHA()
//--------------------------------------------------------------------------------
//		Draws a textured and alpha triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_ALPHAPP()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//x-range
			int64 dx = int64((xe - xs));

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants
			int32 dred = int32((int64(rede - reds) << X_SFT) / dx);
			int32 dgreen = int32((int64(greene - greens) << X_SFT) / dx);
			int32 dblue = int32((int64(bluee - blues) << X_SFT) / dx);

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;

			//Precalculate
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//pre calculate screen reference
				int32 ref = SCREEN_W_by_y + x;

				//Check z buffer
				if (zi > zBuffer[ref])
				{
					//Get texel
					uint32 u = uint32(ui / (zi >> ZU_SHFT)) & u_bit;
					uint32 v = uint32(vi / (zi >> ZV_SHFT)) & v_bit;

					//Get pixels
					uint32 pxl1 = pixels[image_w*v + u];

					//Get current alpha component
					uint32 alpha1 = (pxl1 >> 24);

					//Deal with common alpha values separately for speed.
					switch (alpha1)
					{
					case 0:		//completely transparent
					{
									break;
					}
					case 255:	//completely opaque
					{
									//Set pixel
									output_pixels[ref] = pxl1;

									//Set the z buffer
									zBuffer[ref] = zi;

									break;
					}
					default:
					{
							   uint32 red1 = ((pxl1 >> 8) & 0xFF00) * alpha1;
							   uint32 green1 = (pxl1 & 0xFF00) * alpha1;
							   uint32 blue1 = ((pxl1 & 0xFF) << 8) * alpha1;

							   //Move colors to 3rd octet.
							   //Multiply by a_2*(1-a_1) to move to 1st octet.
							   uint32 pxl2 = output_pixels[ref];
							   uint32 val = 0xFF * (0xFF - alpha1);
							   uint32 red2 = ((pxl2 >> 16) & 0xFF) * val;
							   uint32 green2 = ((pxl2 >> 8) & 0xFF) * val;
							   uint32 blue2 = (pxl2 & 0xFF) * val;

							   output_pixels[ref] = (((
								   A_BITMASK
								   | ((red1 + red2) & R_BITMASK))
								   | (((green1 + green2) >> 8) & G_BITMASK))
								   | (((blue1 + blue2) >> 16) & B_BITMASK));

							   break;
					}
					}
				}

				//Increment increments
				ui += du;
				vi += dv;
				zi += dz;

			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;
	}
}


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_ALPHA()
//--------------------------------------------------------------------------------
//		Draws a textured and alpha triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_ALPHAPP_ALPHAM()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//x-range
			int64 dx = int64((xe - xs));

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants
			int32 dred = int32((int64(rede - reds) << X_SFT) / dx);
			int32 dgreen = int32((int64(greene - greens) << X_SFT) / dx);
			int32 dblue = int32((int64(bluee - blues) << X_SFT) / dx);

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;

			//Precalculate
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//pre calculate screen reference
				int32 ref = SCREEN_W_by_y + x;

				//Check z buffer
				if (zi > zBuffer[ref])
				{
					//Get texel
					uint32 u = uint32(ui / (zi >> ZU_SHFT)) & u_bit;
					uint32 v = uint32(vi / (zi >> ZV_SHFT)) & v_bit;

					//Get pixels
					uint32 pxl1 = pixels[image_w*v + u];

					//Get current alpha component
					uint32 alpha1 = (pxl1 >> 24);

					//Deal with common alpha values separately for speed.
					if (alpha1 != 0)
					{
						//Find alpha value
						uint32 alpha_final_top = (alpha1 * alphaMaster_top) >> 8;
						uint32 alpha_final_bottom = (0xFFFF * (0xFFFF - alpha_final_top)) >> 16;

						//Get the color values in 16:16 format
						uint32 red1 = ((pxl1 >> 16) & 0xFF)  * alpha_final_top;
						uint32 green1 = ((pxl1 >> 8) & 0xFF) * alpha_final_top;
						uint32 blue1 = (pxl1 & 0xFF)			* alpha_final_top;

						//Get the color values in 16:16 format
						uint32 pxl2 = output_pixels[ref];
						uint32 red2 = ((pxl2 >> 16) & 0xFF)  * alpha_final_bottom;
						uint32 green2 = ((pxl2 >> 8) & 0xFF) * alpha_final_bottom;
						uint32 blue2 = (pxl2 & 0xFF)		    * alpha_final_bottom;

						output_pixels[ref] = (((
							A_BITMASK
							| ((red1 + red2) & R_BITMASK))
							| (((green1 + green2) >> 8) & G_BITMASK))
							| (((blue1 + blue2) >> 16) & B_BITMASK));

						break;
					}
				}

				//Increment increments
				ui += du;
				vi += dv;
				zi += dz;

			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;
	}
}


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING()
//--------------------------------------------------------------------------------
//		Draws a textured and per-vertex shaded triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			IncrementColor(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			IncrementColor(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//dx_64d
			int64 dx = int64((xe - xs));	//int64: 12_20

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants
			int32 dred = int32((int64(rede - reds) << X_SFT) / dx);
			int32 dgreen = int32((int64(greene - greens) << X_SFT) / dx);
			int32 dblue = int32((int64(bluee - blues) << X_SFT) / dx);

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;
			int32 redi = reds;
			int32 greeni = greens;
			int32 bluei = blues;

			//Precalculate
			//uint32 format
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				redi += (dred   * (-x_left));
				greeni += (dgreen * (-x_left));
				bluei += (dblue  * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//pre calculate screen reference
				int32 ref = SCREEN_W_by_y + x;

				//Check z buffer
				if (zi > zBuffer[ref])
				{
					//Get texel
					uint32 u = uint32(ui / (zi >> ZU_SHFT)) & u_bit;
					uint32 v = uint32(vi / (zi >> ZV_SHFT)) & v_bit;

					uint32 pxl = pixels[image_w*v + u];
					uint32 red = (pxl >> 16) & 255;
					uint32 green = (pxl >> 8) & 255;
					uint32 blue = pxl & 255;

					uint32 r = ((red * redi) & R_BITMASK);
					uint32 g = ((green * greeni) >> 8) & G_BITMASK;
					uint32 b = ((blue * bluei) >> COLOR_SHFT);



					//Set pixel
					output_pixels[ref] = ((((
						r )
						| g) )
						| b) | A_BITMASK;

					//Set Z-buffer element
					zBuffer[ref] = zi;
				}

				ui += du;
				vi += dv;
				zi += dz;

				redi += dred;
				greeni += dgreen;
				bluei += dblue;

			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

		reds += dreddy_left;
		greens += dgreendy_left;
		blues += dgbluedy_left;
		rede += dreddy_right;
		greene += dgreendy_right;
		bluee += dbluedy_right;

	}

}


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING()
//--------------------------------------------------------------------------------
//		Draws a textured and per-vertex shaded triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING_ALPHAM()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			IncrementColor(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			IncrementColor(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//dx_64d
			int64 dx = int64((xe - xs));	//int64: 12_20

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants
			int32 dred = int32((int64(rede - reds) << X_SFT) / dx);
			int32 dgreen = int32((int64(greene - greens) << X_SFT) / dx);
			int32 dblue = int32((int64(bluee - blues) << X_SFT) / dx);

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;
			int32 redi = reds;
			int32 greeni = greens;
			int32 bluei = blues;

			//Precalculate
			//uint32 format
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				redi += (dred   * (-x_left));
				greeni += (dgreen * (-x_left));
				bluei += (dblue  * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//pre calculate screen reference
				int32 ref = SCREEN_W_by_y + x;

				//Check z buffer
				if (zi > zBuffer[ref])
				{
					//Get texel
					uint32 u = uint32(ui / (zi >> ZU_SHFT)) & u_bit;
					uint32 v = uint32(vi / (zi >> ZV_SHFT)) & v_bit;

					//Get the pixel
					uint32 pxl1 = pixels[image_w*v + u];

					//Get the color values in 16:16 format
					uint32 red1 = ((((pxl1 >> 16) & 0xFF)  * redi)   >> COLOR_SHFT) * alphaMaster_top;
					uint32 green1 = ((((pxl1 >> 8) & 0xFF) * greeni) >> COLOR_SHFT) * alphaMaster_top;
					uint32 blue1 = (((pxl1 & 0xFF)         * bluei)  >> COLOR_SHFT) * alphaMaster_top;

					//Get the color values in 16:16 format
					uint32 pxl2 = output_pixels[ref];
					uint32 red2 = ((pxl2 >> 16) & 0xFF)  * alphaMaster_bottom;
					uint32 green2 = ((pxl2 >> 8) & 0xFF) * alphaMaster_bottom;
					uint32 blue2 = (pxl2 & 0xFF)		 * alphaMaster_bottom;

					output_pixels[ref] = (((
						A_BITMASK
						| ((red1 + red2) & R_BITMASK))
						| (((green1 + green2) >> 8) & G_BITMASK))
						| (((blue1 + blue2) >> 16) & B_BITMASK));

					//Set Z-buffer element
					//zBuffer[ref] = zi;
				}

				ui += du;
				vi += dv;
				zi += dz;

				redi += dred;
				greeni += dgreen;
				bluei += dblue;

			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

		reds += dreddy_left;
		greens += dgreendy_left;
		blues += dgbluedy_left;
		rede += dreddy_right;
		greene += dgreendy_right;
		bluee += dbluedy_right;

	}

}


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING_ALPHA()
//--------------------------------------------------------------------------------
//		Draws a textured, shaded and alpha triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING_ALPHAPP()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			IncrementColor(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			IncrementColor(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//dx_64d
			int64 dx = int64((xe - xs));	//int64: 12_20

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants
			int32 dred = int32((int64(rede - reds) << X_SFT) / dx);
			int32 dgreen = int32((int64(greene - greens) << X_SFT) / dx);
			int32 dblue = int32((int64(bluee - blues) << X_SFT) / dx);

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;
			int32 redi = reds;
			int32 greeni = greens;
			int32 bluei = blues;

			//Precalculate
			//uint32 format
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				redi += (dred   * (-x_left));
				greeni += (dgreen * (-x_left));
				bluei += (dblue  * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//pre calculate screen reference
				int32 ref = SCREEN_W_by_y + x;

				//Check z buffer
				if (zi > zBuffer[ref])
				{
					//Get texel
					uint32 u = uint32(ui / (zi >> ZU_SHFT)) & u_bit;
					uint32 v = uint32(vi / (zi >> ZV_SHFT)) & v_bit;

					//Get pixels
					uint32 pxl1 = pixels[image_w*v + u];

					//Get alpha of the current pixel
					uint32 alpha1 = (pxl1 >> 24);

					//Deal with common alpha values separately for speed.
					switch (alpha1)
					{
					case 0:
					{
							  break;
					}
					case 255:
					{
								uint32 red = (pxl1 >> 16) & 255;
								uint32 green = (pxl1 >> 8) & 255;
								uint32 blue = pxl1 & 255;

								uint8 r = uint8((red * redi) >> COLOR_SHFT);
								uint8 g = uint8((green * greeni) >> COLOR_SHFT);
								uint8 b = uint8((blue * bluei) >> COLOR_SHFT);

								//Set pixel
								output_pixels[ref] = ((((
									r << 8)
									| g) << 8)
									| b) | A_BITMASK;

								//Set the z buffer
								zBuffer[ref] = zi;

								break;
					}
					default:
					{
							   //Move colors to 4th octet.
							   //Multiply by alpha and colori to move to 1st octet.
							   uint32 red1 = ((pxl1 >> 16) & 0xFF) * alpha1 * redi;
							   uint32 green1 = ((pxl1 >> 8) & 0xFF) * alpha1 * greeni;
							   uint32 blue1 = ((pxl1)& 0xFF) * alpha1 * bluei;

							   //Move colors to 3rd octet.
							   //Multiply by a_2*(1-a_1) to move to 1st octet.
							   uint32 pxl2 = output_pixels[ref];
							   uint32 val = 0xFF * (0xFF - alpha1);
							   uint32 red2 = ((pxl2 >> 8) & 0xFF00) * val;
							   uint32 green2 = ((pxl2)& 0xFF00) * val;
							   uint32 blue2 = ((pxl2 << 8) & 0xFF00) * val;

							   output_pixels[ref] = (((
								   A_BITMASK
								   | (((red1 + red2) >> 8) & R_BITMASK))
								   | (((green1 + green2) >> 16) & G_BITMASK))
								   | (((blue1 + blue2) >> 24) & B_BITMASK));

							   break;
					}
					}
				}

				ui += du;
				vi += dv;
				zi += dz;

				redi += dred;
				greeni += dgreen;
				bluei += dblue;

			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

		reds += dreddy_left;
		greens += dgreendy_left;
		blues += dgbluedy_left;
		rede += dreddy_right;
		greene += dgreendy_right;
		bluee += dbluedy_right;

	}

}


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING_ALPHA()
//--------------------------------------------------------------------------------
//		Draws a textured, shaded and alpha triangle.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_LIGHTING_ALPHAPP_ALPHAM()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			IncrementColor(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			IncrementColor(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//dx_64d
			int64 dx = int64((xe - xs));	//int64: 12_20

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants
			int32 dred = int32((int64(rede - reds) << X_SFT) / dx);
			int32 dgreen = int32((int64(greene - greens) << X_SFT) / dx);
			int32 dblue = int32((int64(bluee - blues) << X_SFT) / dx);

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;
			int32 redi = reds;
			int32 greeni = greens;
			int32 bluei = blues;

			//Precalculate
			//uint32 format
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				redi += (dred   * (-x_left));
				greeni += (dgreen * (-x_left));
				bluei += (dblue  * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//pre calculate screen reference
				int32 ref = SCREEN_W_by_y + x;

				//Check z buffer
				if (zi > zBuffer[ref])
				{
					//Get texel
					uint32 u = uint32(ui / (zi >> ZU_SHFT)) & u_bit;
					uint32 v = uint32(vi / (zi >> ZV_SHFT)) & v_bit;

					//Get pixels
					uint32 pxl1 = pixels[image_w*v + u];

					//Get alpha of the current pixel
					uint32 alpha1 = (pxl1 >> 24);

					//Deal with common alpha values separately for speed.
					if (alpha1 != 0)
					{
						//Find alpha value
						uint32 alpha_final_top = (alpha1 * alphaMaster_top) >> 8;
						uint32 alpha_final_bottom = (0xFFFF * (0xFFFF - alpha_final_top)) >> 16;

						//Get the color values in 16:16 format
						uint32 red1 = ((((pxl1 >> 16) & 0xFF)  * redi)   >> COLOR_SHFT) * alpha_final_top;
						uint32 green1 = ((((pxl1 >> 8) & 0xFF) * greeni) >> COLOR_SHFT) * alpha_final_top;
						uint32 blue1 = (((pxl1 & 0xFF)         * bluei)  >> COLOR_SHFT) * alpha_final_top;

						//Get the color values in 16:16 format
						uint32 pxl2 = output_pixels[ref];
						uint32 red2 = ((pxl2 >> 16) & 0xFF)  * alpha_final_bottom;
						uint32 green2 = ((pxl2 >> 8) & 0xFF) * alpha_final_bottom;
						uint32 blue2 = (pxl2 & 0xFF)		    * alpha_final_bottom;

						output_pixels[ref] = (((
							A_BITMASK
							| ((red1 + red2) & R_BITMASK))
							| (((green1 + green2) >> 8) & G_BITMASK))
							| (((blue1 + blue2) >> 16) & B_BITMASK));
					}
				}

				ui += du;
				vi += dv;
				zi += dz;

				redi += dred;
				greeni += dgreen;
				bluei += dblue;

			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

		reds += dreddy_left;
		greens += dgreendy_left;
		blues += dgbluedy_left;
		rede += dreddy_right;
		greene += dgreendy_right;
		bluee += dbluedy_right;

	}

}


//--------------------------------------------------------------------------------
//	@	Rasterizer::InnerLoop_PERSPECTIVE_BACK()
//--------------------------------------------------------------------------------
//		Draws a textured triangle behind all other pixels.
//--------------------------------------------------------------------------------
void Rasterizer::InnerLoop_PERSPECTIVE_BACK()
{
#if defined SAFE_RASTER
	//Clip y values
	if (modifier == 1)
	{
		if (y_end > output_H - 1)
			y_end = output_H - 1;
		if (y_start < 0)
		{
			//Increment limits
			IncrementTriangle(-y_start);
			IncrementTexel(-y_start);
			y_start = 0;
		}
	}
	else
	{
		if (y_end < 0)
			y_end = 0;
		if (y_start > output_H - 1)
		{
			int dy_fix = y_start - output_H + 1;

			//Increment limits
			IncrementTriangle(dy_fix);
			IncrementTexel(dy_fix);
			y_start = output_H - 1;
		}
	}
#endif

	//Draw lines
	for (int32 y = y_start; y != y_end + modifier; y += modifier)
	{
		//Eliminate divide by zero
		if (xe != xs)
		{
			//Set left and right limits
			int32 x_left = ((xs - magic_int) >> X_SFT) + 1;
			int32 x_right = ((xe + magic_int) >> X_SFT);

			//dx_64
			int64 dx = int64((xe - xs));

			//Find interpolants 
			int32 du = int32((int64(ue - us) << X_SFT) / dx);	//texel interpolants 
			int32 dv = int32((int64(ve - vs) << X_SFT) / dx);
			int32 dz = int32((int64(ze - zs) << X_SFT) / dx);	//z interpolants

			int32 ui = us;
			int32 vi = vs;
			int32 zi = zs;

			//Precalculate
			//uint32 format
			int32 SCREEN_W_by_y = output_W*y;

#if defined SAFE_RASTER
			//Clip x values.
			if (x_right >= output_W)
				x_right = output_W - 1;
			if (x_left < 0)
			{
				//Increment values
				ui += (du * (-x_left));
				vi += (dv * (-x_left));
				zi += (dz * (-x_left));

				x_left = 0;
			}
#endif

			//Draw line
			for (int32 x = x_left; x <= x_right; ++x)
			{
				//Check Z-buffer
				int32 ref = SCREEN_W_by_y + x;

				if (zBuffer[ref] == 0)
				{
					//Get texel
					int32 u = (ui / (zi >> ZU_SHFT)) & u_bit;
					int32 v = (vi / (zi >> ZV_SHFT)) & v_bit;

					//Set pixel
					output_pixels[ref] = pixels[image_w*v + u];
				}

				ui += du;
				vi += dv;
				zi += dz;
			}
		}

		//Increment limits
		xs += dxdy_left;
		xe += dxdy_right;
		us += dudy_left;
		ue += dudy_right;
		vs += dvdy_left;
		ve += dvdy_right;
		zs += dzdy_left;
		ze += dzdy_right;

	}

}
