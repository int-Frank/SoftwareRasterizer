//================================================================================
// @ Rasterizer.cpp
// 
// Description: This file defines Rasterizer's methods.
//
// The drawing process starts with a call to the Draw() function. This function
// extracts all data needed for the drawing process from the input polygon and
// stores it in class variables. The polygon is then split into flat bottom/top
// triangles and control is passed to RasterTriangle().
//
// 
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Rasterizer.h"
#include "CommonMath.h"
#include "Polygon_RASTER.h"
#include "Polygon_RASTER_SB.h"
#include "Vertex_RASTER.h"
#include "DgTypes.h"
#include "Materials.h"
#include "Image.h"
#include "rasterizer_defines.h"


//--------------------------------------------------------------------------------
//		Definitions
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//		With SAFE_RASTER defined there is no chance of rasterizing outside of the
//		target window, however may draw a little slower due to a few more checks.
//		Undefine for maximum speed, but ensure polygons are clipped to the 
//		viewpane. 
//--------------------------------------------------------------------------------
//TODO: Safe raster is bugged and is not safe. FIX.
#undef SAFE_RASTER

//Default color for error renders
const uint32 Rasterizer::DEFAULT_COLOR	= 0xFF0000FF;

//--------------------------------------------------------------------------------
//		Key manipulators
//--------------------------------------------------------------------------------

const uint8 Rasterizer::TEXTURED		= (1 << 0);
const uint8 Rasterizer::LIGHTING		= (1 << 1);
const uint8 Rasterizer::ALPHA_PP		= (1 << 2);
const uint8 Rasterizer::ALPHA_MASTER			= (1 << 3);

//The different combinations off effects
const uint8 Rasterizer::KEY_			= 0;
const uint8 Rasterizer::KEY_T			= TEXTURED;
const uint8 Rasterizer::KEY_T_L			= TEXTURED | LIGHTING;
const uint8 Rasterizer::KEY_T_APP		= TEXTURED | ALPHA_PP;
const uint8 Rasterizer::KEY_T_L_APP		= TEXTURED | LIGHTING | ALPHA_PP;
const uint8 Rasterizer::KEY_T_AM		= TEXTURED | ALPHA_MASTER;
const uint8 Rasterizer::KEY_T_L_AM		= TEXTURED | LIGHTING | ALPHA_MASTER;
const uint8 Rasterizer::KEY_T_AM_APP	= TEXTURED | ALPHA_PP | ALPHA_MASTER;
const uint8 Rasterizer::KEY_T_L_AM_APP	= TEXTURED | LIGHTING | ALPHA_PP | ALPHA_MASTER;

//--------------------------------------------------------------------------------
//	@	Rasterizer::AddSignature()
//--------------------------------------------------------------------------------
//		Logs a particular effect in the key.
//--------------------------------------------------------------------------------
void Rasterizer::AddSignature(uint8 val)
{
	KEY = KEY | val;
}	//End: Rasterizer::AddSignature()


//--------------------------------------------------------------------------------
//		Functions
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//		These functions raster a triangle with all the different combinations
//		of the different effects this engine supports.
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Rasterizer::IncrementTexel()
//--------------------------------------------------------------------------------
//		Increment texel data by 'val' steps
//--------------------------------------------------------------------------------
void Rasterizer::IncrementTexel(int val)
{
	us += (dudy_left  * val);
	ue += (dudy_right * val);
	vs += (dvdy_left  * val);
	ve += (dvdy_right * val);

}	//End: Rasterizer::IncrementTexel()


//--------------------------------------------------------------------------------
//	@	Rasterizer::IncrementColor()
//--------------------------------------------------------------------------------
//		Increment color data by 'val' steps
//--------------------------------------------------------------------------------
void Rasterizer::IncrementColor(int val)
{
	reds += (dreddy_left * val);
	greens += (dgreendy_left * val);
	blues += (dgbluedy_left * val);
	rede += (dreddy_right * val);
	greene += (dgreendy_right * val);
	bluee += (dbluedy_right * val);

}	//End: Rasterizer::IncrementColor()



//--------------------------------------------------------------------------------
//		Functions to set the relevant data
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//	@	Rasterizer::SetData_TRIANGLE()
//--------------------------------------------------------------------------------
//		Set data ready to raster a flat top/bottom triangle
//--------------------------------------------------------------------------------
bool Rasterizer::SetData_TRIANGLE(bool top)
{
	//Extract x values
	int32 x0 = int32(cvrt_x * current_p0.pos.X());
	int32 x1 = int32(cvrt_x * current_p1.pos.X());
	int32 x2 = int32(cvrt_x * current_p2.pos.X());

	//y presetting value
	int64 y2_64 = int64(current_p2.pos.Y()*cvrt_ybump);

	//Extract y values
	if (top)
	{
		y_start =  int32(current_p2.pos.Y()) + 1;

#if defined SAFE_RASTER
		//Ensure triangle is inside the viewpane.
		if (y_start > (output_H-1))
			return false;
#endif

		y_end =  int32(current_p0.pos.Y());

#if defined SAFE_RASTER
		//Ensure triangle is inside the viewpane.
		if (y_end < 0)
			return false;
#endif

		modifier = 1;

		//Find inverse y
		dy_inv = int64(cvrt_dy / (current_p0.pos.Y() - current_p2.pos.Y()));

		//Y_bump
		y_bump = int32(YBUMP - (y2_64 & LOWER_30_BITS_IN_64));
	}
	else
	{
		y_start = int32(current_p2.pos.Y());

#if defined SAFE_RASTER
		//Ensure triangle is inside the viewpane.
		if (y_start < 0)
			return false;
#endif

		y_end = int32(current_p0.pos.Y()) + 1;

#if defined SAFE_RASTER
		//Ensure triangle is inside the viewpane.
		if (y_end > (output_H - 1))
			return false;
#endif

		modifier = -1;

		//Find inverse y
		dy_inv = int64(cvrt_dy / (current_p2.pos.Y() - current_p0.pos.Y()));

		//Y_bump
		y_bump = int32(y2_64 & LOWER_30_BITS_IN_64);
	}
	

	//Extract z values and convert to 1/z
	z0_inv = int64(cvrt_z / current_p0.pos.Z());	
	z1_inv = int64(cvrt_z / current_p1.pos.Z());
	z2_inv = int64(cvrt_z / current_p2.pos.Z());

	//dx/dy,dz for the triangle
	dxdy_left   = int32( (int64(x0 - x2) * dy_inv) / DY_INV_SFT);
	dxdy_right  = int32( (int64(x1 - x2) * dy_inv) / DY_INV_SFT);
	int64 dz_left  = z0_inv - z2_inv;
	int64 dz_right = z1_inv - z2_inv;

	//dz/dy
	dzdy_left = int32((dz_left * dy_inv) / DY_INV_SFT);
	dzdy_right = int32((dz_right * dy_inv) / DY_INV_SFT);
	
	//Starting positions
	xs = x2			   + int32((int64(dxdy_left) * y_bump) / YBUMP);
	zs = int32(z2_inv) + int32((int64(dzdy_left) * y_bump) / YBUMP);

	//Ending positions
	xe = x2			   + int32((int64(dxdy_right) * y_bump) / YBUMP);
	ze = int32(z2_inv) + int32((int64(dzdy_right) * y_bump) / YBUMP);

	return true;
}


//--------------------------------------------------------------------------------
//	@	Rasterizer::SetData_PERSPECTIVE_TEXTURE_MAPPING()
//--------------------------------------------------------------------------------
//		Set data needed for perspective texture mapping
//--------------------------------------------------------------------------------
void Rasterizer::SetData_PERSPECTIVE_TEXTURE_MAPPING()
{
	//Get final z shift
	ZU_SHFT = DgLog2(image_w);
	ZV_SHFT = DgLog2(image_h);

	//Get bit numbers to wrap texture coordinates to
	u_bit = (1 << ZU_SHFT) - 1;
	v_bit = (1 << ZV_SHFT) - 1;

	//Extract texel coords
	int64 u0 = int64(current_p0.uv.x*cvrt_uv);	
	int64 u1 = int64(current_p1.uv.x*cvrt_uv);	
	int64 u2 = int64(current_p2.uv.x*cvrt_uv);	
	int64 v0 = int64(current_p0.uv.y*cvrt_uv);	
	int64 v1 = int64(current_p1.uv.y*cvrt_uv);	
	int64 v2 = int64(current_p2.uv.y*cvrt_uv);	

	//du and dv
	int64 du_left  = ( (u0 * z0_inv) / Z_SFT) - ( (u2 * z2_inv) / Z_SFT);
	int64 dv_left  = ( (v0 * z0_inv) / Z_SFT) - ( (v2 * z2_inv) / Z_SFT);
	int64 du_right = ( (u1 * z1_inv) / Z_SFT) - ( (u2 * z2_inv) / Z_SFT);
	int64 dv_right = ( (v1 * z1_inv) / Z_SFT) - ( (v2 * z2_inv) / Z_SFT);

	//du/dy, dv/dy
	dudy_left = int32((du_left * dy_inv) / DY_INV_SFT);
	dvdy_left = int32((dv_left * dy_inv) / DY_INV_SFT);
	dudy_right = int32((du_right * dy_inv) / DY_INV_SFT);
	dvdy_right = int32((dv_right * dy_inv) / DY_INV_SFT);

	//Common terms
	int32 u2_div_z2 = int32((u2 * int64(z2_inv)) / Z_SFT);
	int32 v2_div_z2 = int32((v2 * int64(z2_inv)) / Z_SFT);

	//Starting positions
	us = u2_div_z2 + int32((int64(dudy_left) * y_bump) / YBUMP);
	vs = v2_div_z2 + int32((int64(dvdy_left) * y_bump) / YBUMP);

	//Ending positions
	ue = u2_div_z2 + int32((int64(dudy_right) * y_bump) / YBUMP);
	ve = v2_div_z2 + int32((int64(dvdy_right) * y_bump) / YBUMP);

	//Set tooth in the KEY
	AddSignature(TEXTURED);
}


//--------------------------------------------------------------------------------
//	@	Rasterizer::SetData_LIGHTING()
//--------------------------------------------------------------------------------
//		Set data needed for lighting
//--------------------------------------------------------------------------------
void Rasterizer::SetData_LIGHTING()
{
	//Extract color values and convert to fixed point format.
	int32 x0_red	=	int32(cvrt_color * current_p0.clr[0]);
	int32 x0_green	=	int32(cvrt_color * current_p0.clr[1]);
	int32 x0_blue	=	int32(cvrt_color * current_p0.clr[2]);
	int32 x1_red	=	int32(cvrt_color * current_p1.clr[0]);
	int32 x1_green	=	int32(cvrt_color * current_p1.clr[1]);
	int32 x1_blue	=	int32(cvrt_color * current_p1.clr[2]);
	int32 x2_red	=	int32(cvrt_color * current_p2.clr[0]);
	int32 x2_green	=	int32(cvrt_color * current_p2.clr[1]);
	int32 x2_blue	=	int32(cvrt_color * current_p2.clr[2]);

	//d color
	dreddy_left		= int32( (int64(x0_red - x2_red) * dy_inv) / DY_INV_SFT);
	dgreendy_left	= int32( (int64(x0_green - x2_green) * dy_inv) / DY_INV_SFT);
	dgbluedy_left	= int32( (int64(x0_blue - x2_blue) * dy_inv) / DY_INV_SFT);
	dreddy_right	= int32( (int64(x1_red - x2_red) * dy_inv) / DY_INV_SFT);
	dgreendy_right	= int32( (int64(x1_green - x2_green) * dy_inv) / DY_INV_SFT);
	dbluedy_right	= int32( (int64(x1_blue - x2_blue) * dy_inv) / DY_INV_SFT);

	//Starting positions
	reds = x2_red		+ int32((int64(dreddy_left) * y_bump) / YBUMP);
	greens = x2_green + int32((int64(dgreendy_left) * y_bump) / YBUMP);
	blues = x2_blue	+ int32((int64(dgbluedy_left) * y_bump) / YBUMP);

	//Ending positions
	rede = x2_red		+ int32((int64(dreddy_right) * y_bump) / YBUMP);
	greene = x2_green + int32((int64(dgreendy_right) * y_bump) / YBUMP);
	bluee = x2_blue	+ int32((int64(dbluedy_right) * y_bump) / YBUMP);
	
	//Set tooth in the KEY
	AddSignature(LIGHTING);
}


//--------------------------------------------------------------------------------
//	@	Rasterizer::SetData_LIGHTING()
//--------------------------------------------------------------------------------
//		Preps the rasterizer ready to render an alpha polygon
//--------------------------------------------------------------------------------
void Rasterizer::SetData_ALPHA_MASTER()
{
	//Set the alpha value
	alphaMaster_top = materials->GetAlpha();
	uint32 val = (1 << alpha_SHFT) - 1;
	alphaMaster_bottom = ((val * (val - alphaMaster_top)) >> alpha_SHFT);

	//Set the key
	AddSignature(ALPHA_MASTER);

}	//End: SetData_ALPHA_MASTER()


//--------------------------------------------------------------------------------
//	@	Rasterizer::RasterTriangle()
//--------------------------------------------------------------------------------
//		All input infomation set, ready to draw a triangle to the screen
//--------------------------------------------------------------------------------
void Rasterizer::RasterTriangle(bool top)
{
	//Set triangle - MUST call this function, and must pass
	//before continuing.
	if (!SetData_TRIANGLE(top))
		return;

	//Set Texture
	if (pixels != NULL)
	{
		//Valid pixel data, set for texture mapping
		SetData_PERSPECTIVE_TEXTURE_MAPPING();

		//Check for valid materials
		if (materials != NULL && materials->IsMasterOn())
		{
			//Set Lighting
			SetData_LIGHTING();

			//Set per pixel alpha
			if (materials->IsAlphaPP())
				AddSignature(ALPHA_PP);

			//Set master alpha
			if (materials->IsAlphaMaster())
				SetData_ALPHA_MASTER();
		}
	}

	//Now the key is set, choose correct inner loop
	switch (KEY)
	{
	case KEY_T:				//Textured
		{
			InnerLoop_PERSPECTIVE();
			break;
		}
	case KEY_T_L:			//Textured, Lighting
		{
			InnerLoop_PERSPECTIVE_LIGHTING();
			break;
		}
	case KEY_T_APP:			//Textured, Alpha_PP
		{
			InnerLoop_PERSPECTIVE_ALPHAPP();
			break;
		}
	case KEY_T_L_APP:		//Textured, Lighting, Alpha_PP
		{
			InnerLoop_PERSPECTIVE_LIGHTING_ALPHAPP();
			break;
		}
	case KEY_T_AM:			//Textured, Alpha_M
		{
			InnerLoop_PERSPECTIVE_ALPHAM();
			break;
		}
	case KEY_T_L_AM:		//Textured, Lighting, Alpha_M
		{
			InnerLoop_PERSPECTIVE_LIGHTING_ALPHAM();
			break;
		}
	case KEY_T_AM_APP:		//Textured, Alpha_M, Alpha_PP
		{
			InnerLoop_PERSPECTIVE_ALPHAPP_ALPHAM();
			break;
		}
	case KEY_T_L_AM_APP:	//Textured, Lighting, Alpha_M, Alpha_PP
		{
			InnerLoop_PERSPECTIVE_LIGHTING_ALPHAPP_ALPHAM();
			break;
		}
	case KEY_:			//Default (Solid color)
		{
			InnerLoop_SOLID();
		}
	}

	//Reset key
	KEY = KEY_;

}	//End: Rasterizer::RasterTriangle()


//--------------------------------------------------------------------------------
//	@	Rasterizer::Draw()
//--------------------------------------------------------------------------------
//		Entry point for drawing polygons. This function breaks up the polygon
//		into flat top and/or flat bottom triangles and passes these onto
//		RasterTriangle().
//--------------------------------------------------------------------------------
void Rasterizer::DrawPolygon(const Polygon_RASTER& input )
{
	//check for horizontal or vertical lines
	if ( (DgAreEqual(input.p0.pos.X(), input.p1.pos.X()) && 
			DgAreEqual(input.p0.pos.X(), input.p2.pos.X())) ||
			(DgAreEqual(input.p0.pos.Y(), input.p1.pos.Y()) && 
			DgAreEqual(input.p0.pos.Y(), input.p2.pos.Y()))  )
		return;
	
	//Bring forward relevant data from current Polygon
	p0 = &input.p0;
	p1 = &input.p1;
	p2 = &input.p2;

	//Determine mipmap
	float screen_area_by2 =  
			(	input.p0.pos.X()*(input.p1.pos.Y() - input.p2.pos.Y()) + 
				input.p1.pos.X()*(input.p2.pos.Y() - input.p0.pos.Y()) +
				input.p2.pos.X()*(input.p0.pos.Y() - input.p1.pos.Y()));

	float texture_area_by2 =
		(input.p0.uv.x*(input.p1.uv.y - input.p2.uv.y) +
		input.p1.uv.x*(input.p2.uv.y - input.p0.uv.y) +
		input.p2.uv.x*(input.p0.uv.y - input.p1.uv.y));

	//+ EPSILON to avoid divide by zero
	float ratio = (DgAbs(texture_area_by2)) / (DgAbs(screen_area_by2) + EPSILON);

	//Check for valid texture
	if (input.mipmap == NULL)
	{
		pixels = NULL;
	}
	else
	{
		//Obtain correct mipmap
		const Image* image = input.mipmap->GetImageByArea(ratio);

		//Set image data
		pixels = image->pixels();
		image_w = image->w();
		image_h = image->h();
	}

	//Assign materials
	materials = input.materials;
	
	//Sort points in ascending y order
	if (p0->pos.Y() > p1->pos.Y())
		DgSwap<const Vertex_RASTER*>(p0, p1);
	if (p0->pos.Y() > p2->pos.Y())
		DgSwap<const Vertex_RASTER*>(p0, p2);
	if (p1->pos.Y() > p2->pos.Y())
		DgSwap<const Vertex_RASTER*>(p1, p2);

	//Check for flat top or flat bottom
	if (DgAreEqual(p0->pos.Y(), p1->pos.Y()))
	{
		//Sort for x
		current_p2 = *p2;

		if (p0->pos.X() > p1->pos.X())
		{
			current_p0 = *p1;
			current_p1 = *p0;
		}
		else
		{
			current_p0 = *p0;
			current_p1 = *p1;
		}

		//Continue to draw
		RasterTriangle(false);
	}
	else if (DgAreEqual(p1->pos.Y(), p2->pos.Y()))
	{
		//Sort for x
		current_p2 = *p0;

		if (p2->pos.X() > p1->pos.X())
		{
			current_p0 = *p1;
			current_p1 = *p2;
		}
		else
		{
			current_p0 = *p2;
			current_p1 = *p1;
		}

		//Continue to draw
		RasterTriangle(true);
	}
	else	//Split triangle
	{
		//To store temp vertex
		Vertex_RASTER split;

		//Split poly_rast_temp
		float frac = (p1->pos.Y() - p0->pos.Y()) / (p2->pos.Y() - p0->pos.Y());
		split.pos.X() = p0->pos.X() + frac*(p2->pos.X() - p0->pos.X());
		split.pos.Y() = p0->pos.Y() + frac*(p2->pos.Y() - p0->pos.Y());
		split.clr = p0->clr + (p2->clr - p0->clr)*frac;	//Affine clip

		if (DgAreEqual(p0->pos.Z(), p2->pos.Z()))
		{
			//Linear clip
			split.pos.Z() = p0->pos.Z();
			split.uv = p0->uv + frac*(p2->uv - p0->uv);
		}
		else
		{
			//Perspective clip
			float z0_inv_f = 1.0f / p0->pos.Z();
			float dz = 1.0f / p2->pos.Z() - z0_inv_f;
			float z = 1.0f / (z0_inv_f + frac*dz);
			dz = (z - p0->pos.Z()) / (p2->pos.Z() - p0->pos.Z());

			split.pos.Z() = z;								//Perspective clip
			split.uv = p0->uv + dz*(p2->uv - p0->uv);		//Perspective clip

		}

		//Create flat top poly
		current_p2 = *p2;

		//Sort for x
		if (split.pos.X() < p1->pos.X())
		{
			current_p0 = split;
			current_p1 = *p1;
		}
		else
		{
			current_p0 = *p1;
			current_p1 = split;
		}

		//Raster flat top
		RasterTriangle(false);

		//Create flat top poly
		current_p2 = *p0;

		//Raster flat top
		RasterTriangle(true);
	}
}


//--------------------------------------------------------------------------------
//	@	Rasterizer::Draw()
//--------------------------------------------------------------------------------
//		Entry point for drawing Skybox polygons. This function breaks up the polygon
//		into flat top and/or flat bottom triangles and passes these onto
//		RasterTriangle().
//--------------------------------------------------------------------------------
void Rasterizer::DrawSkyBoxPolygon(const Polygon_RASTER_SB& input)
{
	//check for horizontal or vertical lines
	if ( (DgAreEqual(input.p0.pos.X(), input.p1.pos.X()) && 
			DgAreEqual(input.p0.pos.X(), input.p2.pos.X())) ||
			(DgAreEqual(input.p0.pos.Y(), input.p1.pos.Y()) && 
			DgAreEqual(input.p0.pos.Y(), input.p2.pos.Y()))  )
		return;
	
	//Bring forward relevant data from current Polygon
	p0 = &input.p0;
	p1 = &input.p1;
	p2 = &input.p2;

	//Check for valid texture
	if (input.image == NULL)
	{
		pixels = NULL;
	}
	else
	{
		//Set image data
		pixels = input.image->pixels();
		image_w = input.image->w();
		image_h = input.image->h();
	}

	
	//Assign materials
	materials = NULL;
	
	//Sort points in ascending y order
	if (p0->pos.Y() > p1->pos.Y())
		DgSwap<const Vertex_RASTER*>(p0, p1);
	if (p0->pos.Y() > p2->pos.Y())
		DgSwap<const Vertex_RASTER*>(p0, p2);
	if (p1->pos.Y() > p2->pos.Y())
		DgSwap<const Vertex_RASTER*>(p1, p2);

	//Check for flat top or flat bottom
	if (DgAreEqual(p0->pos.Y(), p1->pos.Y()))
	{
		//Sort for x
		current_p2 = *p2;

		if (p0->pos.X() > p1->pos.X())
		{
			current_p0 = *p1;
			current_p1 = *p0;
		}
		else
		{
			current_p0 = *p0;
			current_p1 = *p1;
		}

		//Continue to draw
		SetData_TRIANGLE(false);
		SetData_PERSPECTIVE_TEXTURE_MAPPING();
		InnerLoop_PERSPECTIVE_BACK();
	}
	else if (DgAreEqual(p1->pos.Y(), p2->pos.Y()))
	{
		//Sort for x
		current_p2 = *p0;

		if (p2->pos.X() > p1->pos.X())
		{
			current_p0 = *p1;
			current_p1 = *p2;
		}
		else
		{
			current_p0 = *p2;
			current_p1 = *p1;
		}

		//Continue to draw
		SetData_TRIANGLE(true);
		SetData_PERSPECTIVE_TEXTURE_MAPPING();
		InnerLoop_PERSPECTIVE_BACK();
	}
	else	//Split triangle
	{
		//To store temp vertex
		Vertex_RASTER split;

		//Split poly_rast_temp
		float frac = (p1->pos.Y() - p0->pos.Y())/(p2->pos.Y() - p0->pos.Y());
		split.pos.X() = p0->pos.X() + frac*(p2->pos.X() - p0->pos.X());
		split.pos.Y() = p0->pos.Y() + frac*(p2->pos.Y() - p0->pos.Y());
			
		if (DgAreEqual(p0->pos.Z(), p2->pos.Z()))
		{
			//Linear clip
			split.pos.Z() = p0->pos.Z();
			split.uv = p0->uv + frac*(p2->uv - p0->uv);
		}
		else
		{
			//Perspective clip
			float z0_inv_f = 1.0f/p0->pos.Z();
			float dz = 1.0f/p2->pos.Z() - z0_inv_f;
			float z = 1.0f/(z0_inv_f + frac*dz);
			dz = (z - p0->pos.Z())/(p2->pos.Z() - p0->pos.Z());
				
			split.pos.Z() = z;								//Perspective clip
			split.uv = p0->uv + dz*(p2->uv - p0->uv);		//Perspective clip
				
		}

		//Create flat top poly
		current_p2 = *p2;

		//Sort for x
		if (split.pos.X() < p1->pos.X())
		{
			current_p0 = split;
			current_p1 = *p1;
		}
		else
		{
			current_p0 = *p1;
			current_p1 = split;
		}

		//Raster flat bottom
		SetData_TRIANGLE(false);
		SetData_PERSPECTIVE_TEXTURE_MAPPING();
		InnerLoop_PERSPECTIVE_BACK();

		//Create flat top poly
		current_p2 = *p0;

		//Raster flat top
		SetData_TRIANGLE(true);
		SetData_PERSPECTIVE_TEXTURE_MAPPING();
		InnerLoop_PERSPECTIVE_BACK();
	}

}


//--------------------------------------------------------------------------------
//	@	Rasterizer::SetOutput()
//--------------------------------------------------------------------------------
//		Sets all data needed for output
//--------------------------------------------------------------------------------
void Rasterizer::SetOutput(Image& out, DgArray<int32>& z)
{
	if (z.max_size() < out.w() * out.h())
	{
		output_pixels = NULL;
		zBuffer = NULL;
		output_W = output_H = 0;

        std::cerr << "Rasterizer::SetOutput() -> ZBuffer too small for output image" << std::endl;

		return;
	}

	output_pixels = out.pixels();
	output_W = int32(out.w());
	output_H = int32(out.h());

	zBuffer = z.Data();

}	//End: Rasterizer::SetOutput()