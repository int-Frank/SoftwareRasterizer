#ifndef VERTEX_RASTER_H
#define VERTEX_RASTER_H

#include "Vector2.h"
#include "Point4.h"
#include "Tuple.h"
#include "Vertex.h"

struct Vertex_RASTER
{
	//Construct vertex_raster from vertex temp values.
	Vertex_RASTER() {}
	Vertex_RASTER(const Vertex& v, const Vector2 _uv) : 
		pos(v.position_temp), clr(v.clr), uv(_uv) {}

	Vertex_RASTER(const Vertex_RASTER& other) :
		pos(other.pos), clr(other.clr), uv(other.uv) {}

	Point4 pos;
	Tuple<float> clr;
	Vector2 uv;
};

#endif