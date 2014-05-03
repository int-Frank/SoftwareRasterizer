//================================================================================
// @ MasterPList.h
// 
// Class: MasterPList
//
// The Master P (Polygon) List is a place to dump all clipped polygons that are 
// to be rendered to the screen. The lists can be sorted to reduce overdraw.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef MASTERPLIST_H
#define MASTERPLIST_H

#include "Polygon_RASTER.h"
#include "Polygon_RASTER_SB.h"
#include "Particle_RASTER.h"
#include "SortContainer.h"
#include "DgArray.h"
#include "DgTypes.h"

struct Polygon;
class Point4;
class Rasterizer;

//--------------------------------------------------------------------------------
//	@	MasterPList
//--------------------------------------------------------------------------------
class MasterPList
{
	//friend class Rasterizer;
public:

	//Constructor/Destructor
	MasterPList();
	~MasterPList();
	
	//Copy operations
	MasterPList(const MasterPList& other) {init(other);}
	MasterPList& operator=(const MasterPList&);

	//Delete current array and create new of size s
	void SetSize(uint32 new_size);
	
	//Add polygons to the list
	void Add(const Polygon_RASTER&);
	void Add(const Polygon_RASTER_SB&);
	void Add(const Particle_RASTER&);
	
	//Draw polygons to screen
	void SendToRasterizer(Rasterizer&);

	//Effectively clears list, ready for new polygons to be added
	void Reset();

private:
	//Data members

	//List of Polygon_RASTERs
	DgArray<Polygon_RASTER>		PList;				//Object polygons
	DgArray<Polygon_RASTER>		AList;				//Alpha polygons
	DgArray<Particle_RASTER>	ParticleList;		//Particle
	DgArray<Polygon_RASTER_SB>	SkyboxList;			//Skybox polygons

	//Final sorted polygon list
	DgArray<SortContainer<Polygon_RASTER, float>>  PList_Sorted;
	DgArray<SortContainer<Drawable, float>>		   AList_Sorted;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------

	//Copy function
	void init(const MasterPList& other);

	//Master Sort function
	void SortPolygons();
	void SortAlphas();

};


//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
//extern MasterPList MASTERPLIST;


//--------------------------------------------------------------------------------
//		Inlines
//--------------------------------------------------------------------------------

#endif