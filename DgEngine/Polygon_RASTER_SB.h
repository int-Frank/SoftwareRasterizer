#ifndef POLYGON_RASTER_SB_H
#define POLYGON_RASTER_SB_H

#include "Vertex_RASTER.h"
#include "Polygon.h"
#include "Image.h"


//--------------------------------------------------------------------------------
//		Polygon class: Self-contained
//--------------------------------------------------------------------------------
struct Polygon_RASTER_SB
{
	//Constructor/Destructor
	Polygon_RASTER_SB(): image(NULL) {} 
	~Polygon_RASTER_SB() {}

	//Copy operatorations
	Polygon_RASTER_SB(const Polygon_RASTER_SB& other):
		p0(other.p0), p1(other.p1), p2(other.p2), 
		image(other.image) {}

	Polygon_RASTER_SB& operator= (const Polygon_RASTER_SB&);

	//Manipulate data
	inline void Set(const Polygon& p, const Image* img);

	//Data
	Vertex_RASTER p0, p1, p2;	//Vertices of the triangle
	const Image* image;			//The image
};


//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//		Create Polygon_RASTER_SB from Polygon
//--------------------------------------------------------------------------------
inline void Polygon_RASTER_SB::Set(const Polygon& p,
								const Image* img)
{
	//Positional data
	p0.pos = p.p0->position_temp; 
	p1.pos = p.p1->position_temp; 
	p2.pos = p.p2->position_temp;

	//Vertex colors
	p0.clr = p.p0->clr;
	p1.clr = p.p1->clr;
	p2.clr = p.p2->clr;

	//Texel coordinates
	p0.uv = p.uv0;
	p1.uv = p.uv1;
	p2.uv = p.uv2;

	//Texture
	image = img;

}	//End: Polygon_RASTER_SB::Set()

#endif