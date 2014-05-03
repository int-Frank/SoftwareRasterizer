#include "Polygon_RASTER.h"


//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Polygon_RASTER& Polygon_RASTER::operator= (const Polygon_RASTER& other)
{
	if (this == &other)
		return *this;

	p0 = other.p0;
	p1 = other.p1;
	p2 = other.p2;
	
	materials = other.materials;
	mipmap = other.mipmap;

	return *this;

}	//End: Polygon_RASTER::Polygon_RASTER()
