//================================================================================
// @ MasterPList.cpp
// 
// Description: This file defines MasterPList's methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "MasterPList.h"
#include "Polygon.h"
#include "Rasterizer.h"
#include <algorithm>


//--------------------------------------------------------------------------------
//	@	MasterPList::MasterPList()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
MasterPList::MasterPList()
{
}	//End: MasterPList::MasterPList()


//--------------------------------------------------------------------------------
//	@	MasterPList::MasterPList()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
MasterPList::~MasterPList()
{
}	//End: MasterPList::~MasterPList()


//--------------------------------------------------------------------------------
//	@	MasterPList::init()
//--------------------------------------------------------------------------------
//		Copy function, does not copy sorted lists.
//--------------------------------------------------------------------------------
void MasterPList::init(const MasterPList& other)
{
	PList = other.PList;
	AList = other.AList;
	ParticleList = other.ParticleList;
	SkyboxList = other.SkyboxList;
	PList_Sorted.clear();
	AList_Sorted.clear();

}	//End: MasterPList::init()


//--------------------------------------------------------------------------------
//	@	MasterPList::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
MasterPList& MasterPList::operator=(const MasterPList& other)
{
	if (this == & other)
		return *this;

	init(other);

	return *this;

}	//End: MasterPList::operator=()


//--------------------------------------------------------------------------------
//	@	MasterPList::SetSize()
//--------------------------------------------------------------------------------
//		Set size of polygon arrays
//--------------------------------------------------------------------------------
void MasterPList::SetSize(uint32 new_size)
{
	//Resize arrays
	PList.resize(new_size);
	PList_Sorted.resize(new_size);

}	//End: MasterPList::SetSize()


//--------------------------------------------------------------------------------
//	@	MasterPList::Add()
//--------------------------------------------------------------------------------
//		Add a polygon
//--------------------------------------------------------------------------------
void MasterPList::Add(const Polygon_RASTER& p)
{
	//Add to list
	if (p.materials->IsAlphaPP() || p.materials->IsAlphaMaster())
	{
		AList.push_back(p);
	}
	else
	{
		PList.push_back(p);
	}
	
}	//End: MasterPList::Add()


//--------------------------------------------------------------------------------
//	@	MasterPList::AddToSkybox()
//--------------------------------------------------------------------------------
//		Add a polygon to the skybox list
//--------------------------------------------------------------------------------
void MasterPList::Add(const Polygon_RASTER_SB& p)
{
	//Add to list
	SkyboxList.push_back(p);
	
}	//End: MasterPList::AddToSkybox()


//--------------------------------------------------------------------------------
//	@	MasterPList::Add()
//--------------------------------------------------------------------------------
//		Add a particle to the skybox list
//--------------------------------------------------------------------------------
void MasterPList::Add(const Particle_RASTER& p)
{
	//Add to list
	ParticleList.push_back(p);

}	//End: MasterPList::Add()


//--------------------------------------------------------------------------------
//	@	MasterPList::Sort()
//--------------------------------------------------------------------------------
//		Create z-sorted list, assume distance from origin 
//--------------------------------------------------------------------------------
void MasterPList::SortPolygons()
{
	//Assign pointers and z-distances
	for (uint32 i = 0; i < PList.size(); ++i)
	{
		PList_Sorted.push_back(SortContainer<Polygon_RASTER, float>(
			PList[i], PList[i].GetSortValue()));
	}

	//Obtain underlying array
	SortContainer<Polygon_RASTER, float> *Sorted = PList_Sorted.Data();

	//Sort nearest to farthest
	std::sort(Sorted, Sorted + PList_Sorted.size());

}	//End: MasterPlist::Sort()


//--------------------------------------------------------------------------------
//	@	MasterPList::SortAlphas()
//--------------------------------------------------------------------------------
//		Create z-sorted alpha list, assume distance from origin 
//--------------------------------------------------------------------------------
void MasterPList::SortAlphas()
{
	//Add alpha polygons
	for (uint32 i = 0; i < AList.size(); ++i)
	{
		AList_Sorted.push_back(SortContainer<Drawable, float>(
			AList[i], AList[i].GetSortValue()));
	}

	//Add particles
	for (uint32 i = 0; i < ParticleList.size(); ++i)
	{
		AList_Sorted.push_back(SortContainer<Drawable, float>(
			ParticleList[i], ParticleList[i].GetSortValue()));
	}

	//Get pointer to data
	SortContainer<Drawable, float>* alphaData = AList_Sorted.Data();

	//Sort nearest to farthest
	std::sort(alphaData, alphaData + AList_Sorted.size());

}	//End: MasterPlist::SortAlphas()


//--------------------------------------------------------------------------------
//	@	MasterPList::SendToRasterizer()
//--------------------------------------------------------------------------------
//		Send the polygons down the pipeline
//--------------------------------------------------------------------------------
void MasterPList::SendToRasterizer(Rasterizer& output)
{
	//Sort the drawables
	SortPolygons();
	SortAlphas();

	//Obtain underlying array
	SortContainer<Polygon_RASTER, float> *Sorted_P = PList_Sorted.Data();

	//Send each polygon from PList_Sorted to the rasterizer
	for (int32 i = PList_Sorted.size() - 1; i > -1; --i)
	{
		output.DrawPolygon( *(Sorted_P[i].ptr) );
	}
	
	//Send skybox through
	for (uint32 i = 0; i < SkyboxList.size(); ++i)
	{
		output.DrawSkyBoxPolygon( SkyboxList[i] );
	}

	//Send alpha polygons through

	//Obtain underlying array
	SortContainer<Drawable, float> *Sorted_A = AList_Sorted.Data();

	for (int32 i = AList_Sorted.size() - 1; i > -1; --i)
	{
		Sorted_A[i].ptr->Draw(output);
	}

}	//End: MasterPList::Draw()


//--------------------------------------------------------------------------------
//	@	MasterPList::Reset()
//--------------------------------------------------------------------------------
//		Finish and reset masterlist
//--------------------------------------------------------------------------------
void MasterPList::Reset()
{
	//Reset sizes
	PList.clear();		
	AList.clear();
	ParticleList.clear();
	SkyboxList.clear();

	AList_Sorted.clear();
	PList_Sorted.clear();

}	//End: MasterPList::Reset()