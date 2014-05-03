#include "Polygon_RASTER_SB.h"


//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Polygon_RASTER_SB& Polygon_RASTER_SB::operator= (const Polygon_RASTER_SB& other)
{
	if (this == &other)
		return *this;

	p0 = other.p0;
	p1 = other.p1;
	p2 = other.p2;
	
	image = other.image;

	return *this;

}	//End: Polygon_RASTER_SB::Polygon_RASTER_SB()
