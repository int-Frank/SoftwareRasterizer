//================================================================================
// @ Rasterizer.h
// 
// Class: Rasterizer
//
// The rasterizer is responsible for drawing triangles on a pixel array. The
// pixel type must be 32BPP (ARGB). The rasterizer does not clip to the output
// buffer dimensions; this must be done before send triangles through.
//
// The rasterizing currently supports four types of drawing effects:
//		
//		* Single color fill
//		* Perspective correct texturing
//		* Per vertex light shading
//		* Transparency (per pixel)
//		* Transparency (master)
//
// The materials object within the input polygon object determines which effects
// will be applied to the final triangle. The rasterizer also contains a separate
// function to draw skybox polygons, as these are typically drawn will no effects.
// See Rasterizer.cpp for a description of how the drawing process.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "DgTypes.h"
#include "DgArray.h"
#include "Vertex_RASTER.h"
#include <string>

class Image;
struct Polygon_RASTER;
struct Polygon_RASTER_SB;
struct Particle_RASTER;
class Materials;
struct Polygon;
class ParticleAlphaTemplate;

//--------------------------------------------------------------------------------
//	@	Rasterizer
//--------------------------------------------------------------------------------
class Rasterizer
{
public:

	//Constructor/Destructor
	Rasterizer(): KEY(0), p0(NULL), p1(NULL), p2(NULL), 
	materials(NULL), pixels(NULL), output_pixels(NULL), zBuffer(NULL){}
	~Rasterizer() {}

	//Set output pixel array and z-buffer. Must be set before the
	//rasterizer can be used.
	void SetOutput(Image&, DgArray<int32>& zbuffer);
	void NoOutput() { output_pixels = NULL; zBuffer = NULL; }

	//Render a polygon to the screen.
	//void Draw(const Polygon&);
	void DrawPolygon(const Polygon_RASTER&);
	void DrawSkyBoxPolygon(const Polygon_RASTER_SB&);

	//Draw a particle to the screen
	void DrawParticle(const Particle_RASTER&);

private:
	
	//--------------------------------------------------------------------------------
	//		DATA
	//--------------------------------------------------------------------------------

	static const uint32 DEFAULT_COLOR;	

	//--------------------------------------------------------------------------------
	//		Key manipulators. The key is a place to log which effects will be
	//		added to the final tringle raster.
	//--------------------------------------------------------------------------------

	//Rasterization key. Logs all effects currently in use. 
	uint8 KEY;
	/*
	0	-	Textured
	0	-	Lighting
	0	-	Alpha per pixel
	0	-	Alpha entire polygon
	0
	0
	0
	0
	*/

	//'Teeth' for the key
	static const uint8 TEXTURED;
	static const uint8 LIGHTING;
	static const uint8 ALPHA_PP;
	static const uint8 ALPHA_MASTER;

	//The different combinations of effects that exist.
	static const uint8 KEY_;
	static const uint8 KEY_T;
	static const uint8 KEY_T_L;
	static const uint8 KEY_T_AM;
	static const uint8 KEY_T_L_AM;
	static const uint8 KEY_T_APP;
	static const uint8 KEY_T_L_APP;
	static const uint8 KEY_T_AM_APP;
	static const uint8 KEY_T_L_AM_APP;


	//--------------------------------------------------------------------------------
	//		Set during Draw(). Fetching of data.
	//--------------------------------------------------------------------------------

	//Pointers to input vertices
	const Vertex_RASTER *p0;
	const Vertex_RASTER *p1;
	const Vertex_RASTER *p2;

	//Texture data
	const Materials* materials;
	const uint32* pixels;
	int32 image_w;
	int32 image_h;


	//--------------------------------------------------------------------------------
	//		Set during Continue_Draw(). Creating vertex raster points in screen space.
	//--------------------------------------------------------------------------------

	//Current vertices in play
	Vertex_RASTER current_p0;
	Vertex_RASTER current_p1;
	Vertex_RASTER current_p2;


	//--------------------------------------------------------------------------------
	//		Set during SetData_TRIANGLE(). Iterative values for drawing the triangle.
	//--------------------------------------------------------------------------------

	//y positional variables
	int32 y_start, y_end;
	int64 dy_inv;

	//Variables for drawing a triangle on the screen
	int64 z0_inv, z1_inv, z2_inv;
	int32 dxdy_left, dxdy_right;
	int32 dzdy_left, dzdy_right;
	int32 y_bump;
	int32 xs, zs;
	int32 xe, ze;


	//--------------------------------------------------------------------------------
	//		Set during _PERSPECTIVE_TEXTURE_MAPPING(). Values needed to 
	//		sample the texture.
	//--------------------------------------------------------------------------------

	//Final shift values
	uint32 ZU_SHFT, ZV_SHFT;
	uint32 u_bit, v_bit;

	//Step variables
	int32 dudy_left, dvdy_left;
	int32 dudy_right, dvdy_right;
	int32 us, vs;
	int32 ue, ve;


	//--------------------------------------------------------------------------------
	//		Set during SetData_LIGHTING(). Values needed to iterate light colors
	//		between vertices.
	//--------------------------------------------------------------------------------

	//Variables for color manipulation
	int32 dreddy_left, dgreendy_left, dgbluedy_left;
	int32 dreddy_right, dgreendy_right, dbluedy_right;
	int32 reds, greens, blues;
	int32 rede, greene, bluee;


	//--------------------------------------------------------------------------------
	//		General alpha value
	//--------------------------------------------------------------------------------

	uint32 alphaMaster_top;
	uint32 alphaMaster_bottom;

	//--------------------------------------------------------------------------------
	//		Utility
	//--------------------------------------------------------------------------------

	//Determines if the triangle is flat top or bottom.
	// +1 = flat top, -1 = flat bottom.
	int32 modifier;


	//--------------------------------------------------------------------------------
	//		Output Image data
	//--------------------------------------------------------------------------------
	uint32* output_pixels;
	int32 output_W;
	int32 output_H;

	//--------------------------------------------------------------------------------
	//		Z-Buffer
	//--------------------------------------------------------------------------------
	int32* zBuffer;

private:

	//--------------------------------------------------------------------------------
	//		FUNCTIONS
	//--------------------------------------------------------------------------------

	//Log an effect into the KEY
	void AddSignature(uint8);

	//--------------------------------------------------------------------------------
	//		Drawing functions. After all effects have been setup and logged,
	//		one of these functions will be chosen depending on the KEY.
	//--------------------------------------------------------------------------------
	void InnerLoop_SOLID();
	void InnerLoop_PERSPECTIVE_BACK();

	void InnerLoop_PERSPECTIVE();
	void InnerLoop_PERSPECTIVE_LIGHTING();
	void InnerLoop_PERSPECTIVE_ALPHAPP();
	void InnerLoop_PERSPECTIVE_LIGHTING_ALPHAPP();
	void InnerLoop_PERSPECTIVE_ALPHAM();
	void InnerLoop_PERSPECTIVE_LIGHTING_ALPHAM();
	void InnerLoop_PERSPECTIVE_ALPHAPP_ALPHAM();
	void InnerLoop_PERSPECTIVE_LIGHTING_ALPHAPP_ALPHAM();

	//--------------------------------------------------------------------------------
	//		Functions to set the relevant data
	//--------------------------------------------------------------------------------
	bool SetData_TRIANGLE(bool top);
	void SetData_PERSPECTIVE_TEXTURE_MAPPING();
	void SetData_LIGHTING();
	void SetData_ALPHA_MASTER();
	void RasterTriangle(bool top);

	void IncrementTriangle(int val);
	void IncrementTexel(int val);
	void IncrementColor(int val);

	//--------------------------------------------------------------------------------
	//		Particle drawing functions
	//--------------------------------------------------------------------------------
	void DrawParticle(const uint8*, uint32 pitch);
	void DrawParticleDisk(){}
	void DrawParticleSquare(){}
	void DrawParticle(const ParticleAlphaTemplate*, uint32 alpha){}
};


#endif