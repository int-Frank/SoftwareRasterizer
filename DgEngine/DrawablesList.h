//================================================================================
// @ DrawablesList.h
// 
// Class: DrawablesList
//
// This class stores all drawables within the scene (typically within the frustum).
// The lists can be sorted to reduce overdraw.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DRAWABLESLIST_H
#define DRAWABLESLIST_H

#include "Polygon_RASTER.h"
#include "Polygon_RASTER_SB.h"
#include "Particle_RASTER.h"
#include "SortContainer.h"
#include "DgArray.h"
#include "DgTypes.h"
#include "Mesh.h"
#include "Materials.h"
#include "Mipmap.h"
#include "Clipper.h"

struct Polygon;
class Point4;
class Rasterizer;

//--------------------------------------------------------------------------------
//	@	DrawablesList
//--------------------------------------------------------------------------------
class DrawablesList
{
	//friend class Rasterizer;
public:

	//Constructor/Destructor
	DrawablesList();
	~DrawablesList();
	
	//Copy operations
	DrawablesList(const DrawablesList& other) {init(other);}
	DrawablesList& operator=(const DrawablesList&);

	//Delete current array and create new of size s
	void SetSize(uint32 new_size);
	
	//Add polygons to the list
	void Add(const Mesh&);
	void Add(Clipper::Point* start, uint32 size, const Materials*, const Mipmap*);
	void AddSB(Clipper::Point* start, uint32 size, const Image*);
	void Add(const Particle_RASTER&);
	
	//Draw polygons to screen
	void SendToRasterizer(Rasterizer&);

	//Effectively clears list, ready for new polygons to be added
	void Reset();

private:
	//Data members


	//List of Polygon_RASTERs
	DgArray<Vertex_RASTER>		VList;				//All polygon vertices
	DgArray<Polygon_RASTER>		PList;				//Object polygons
	DgArray<Polygon_RASTER>		AList;				//Alpha polygons
	DgArray<Polygon_RASTER_SB>	SkyboxList;			//Skybox polygons
	DgArray<Particle_RASTER>	ParticleList;		//Particle

	//Final sorted polygon list
	DgArray<SortContainer<Polygon_RASTER, float>>		PList_Sorted;
	DgArray<SortContainer<Drawable, float>>				AList_Sorted; //All alpha drawables

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------

	//Copy function
	void init(const DrawablesList& other);

	//Master Sort function
	void SortPolygons();
	void SortAlphas();

};

#endif