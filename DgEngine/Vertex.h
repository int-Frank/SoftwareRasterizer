#ifndef VERTEX_H
#define VERTEX_H

#include "Tuple.h"
#include "Vector4.h"
#include "Point4.h"


//--------------------------------------------------------------------------------
//		3D Positional info for vertices
//--------------------------------------------------------------------------------
struct Vertex
{
	//Constructor
	Vertex(): clr(Tuple<float>(0.0f,0.0f,0.0f)), position(Point4::origin),
		normal(Vector4::origin), position_temp(Point4::origin), normal_temp(Vector4::origin),
		state('x') {}
	
	//Resets the vertex
	inline void reset() {clr.Set(0.0f); state = 'x';}

	//Comparison
	bool operator==(const Vertex& rhs) const {return (rhs.position == position);}
	bool operator!=(const Vertex& rhs) const {return (rhs.position != position);}


	//--------------------------------------------------------------------------------
	//		Data
	//--------------------------------------------------------------------------------

	//Static data for base object
	Point4 position;		
	Vector4 normal;

	//Dynamic data for rasterization
	Point4 position_temp;	//Storage for camera/screen transformations
	Vector4 normal_temp;	//Storage for transformations
	Tuple<float> clr;		//Color

	//State:	'a' - active
	//			'x' - inactive
	char state;
};

#endif