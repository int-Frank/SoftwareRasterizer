#ifndef POLYGONSELF_H
#define POLYGONSELF_H

#include "Vertex_RASTER.h"
#include "Polygon.h"
#include "Materials.h"
#include "Mipmap.h"
#include "Rasterizer.h"
#include "Drawable.h"


//--------------------------------------------------------------------------------
//		Polygon class: Self-contained
//--------------------------------------------------------------------------------
struct Polygon_RASTER : public Drawable
{
	//Constructor/Destructor
	Polygon_RASTER(): materials(NULL), mipmap(NULL) {} 
	Polygon_RASTER(const Polygon& p, const Materials* mat, const Mipmap* mm);
	~Polygon_RASTER() {}

	//Copy operatorations

	void Draw(Rasterizer& r) { r.DrawPolygon(*this); }
	float GetSortValue() const { return (p0.pos.Z() + p1.pos.Z() + p2.pos.Z()) * 0.333333f;}

	Polygon_RASTER& operator= (const Polygon_RASTER&);

	//Manipulate data
	inline void Set(const Polygon& p, const Materials* mat, const Mipmap* mm);

	//Data
	//The first vertex needs to be a number reference when building
	//the vertex list, but a pointer for the rasterizer.
	Vertex_RASTER p0, p1, p2;
	const Materials* materials;	//materials
	const Mipmap* mipmap;		//The image mipmap
};


//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//		Create Polygon_RASTER from Polygon
//--------------------------------------------------------------------------------
inline void Polygon_RASTER::Set(const Polygon& p,
								const Materials* mat,
								const Mipmap* mm)
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
	materials = mat;
	mipmap = mm;

}	//End: Polygon_RASTER::Set()


//--------------------------------------------------------------------------------
//	@	Polygon_RASTER::Polygon_RASTER()
//--------------------------------------------------------------------------------
//		Initiate from Polygon
//--------------------------------------------------------------------------------
inline Polygon_RASTER::Polygon_RASTER(const Polygon& p, const Materials* mat, const Mipmap* mm) :
	p0(*p.p0, p.uv0), p1(*p.p1, p.uv1), p2(*p.p2, p.uv2), materials(mat), mipmap(mm)
{

}	//End: Polygon_RASTER::Polygon_RASTER()

#endif