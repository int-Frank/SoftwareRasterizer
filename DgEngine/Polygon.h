#ifndef POLYGON_H
#define POLYGON_H

#include "Plane4.h"
#include "Vertex.h"
#include "Vector2.h"
#include "Tuple.h"
#include "DgTypes.h"
#include <list>


//--------------------------------------------------------------------------------
/*		Polygon with external dependancies
		Positional data and Texel data are decoupled as
		any Vertex can have multiple uv coords
*/
//--------------------------------------------------------------------------------
struct Polygon
{
	//Constructor/Destructor
	Polygon(): state('a') {}

	//Resets the vertex
	inline void reset() {state = 'a';}

	//Data
	Vertex *p0, *p1, *p2;		//3D positional info
	Vector2 uv0, uv1, uv2;		//Texture coords
	Plane4 plane;				//Plane4 (normal vector and d)

	//State:	'a' - active
	//			'x' - inactive
	char state;

};


//--------------------------------------------------------------------------------
//		Return the point at the center of the polygon
//--------------------------------------------------------------------------------
inline Point4 Center(const Polygon& poly)
{
	return Point4(
		(poly.p0->position.X() + poly.p1->position.X() + poly.p2->position.X())/3.0f, 
		(poly.p0->position.Y() + poly.p1->position.Y() + poly.p2->position.Y())/3.0f, 
		(poly.p0->position.Z() + poly.p1->position.Z() + poly.p2->position.Z())/3.0f);

}	//End: Polygon::Center()

#endif