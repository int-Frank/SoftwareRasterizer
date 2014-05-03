//================================================================================
// @ Primitive1D.h
// 
// Class: Primitive1D
//
// Base 1D Primitive class. Derives from Primitive.
//
// -------------------------------------------------------------------------------
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef PRIMITIVE1D_H
#define PRIMITIVE1D_H

#include "Primitive.h"

//--------------------------------------------------------------------------------
//	@	Primitive1D
//--------------------------------------------------------------------------------
class Primitive1D: public Primitive
{
public:
	//Constructor / destructor
	Primitive1D() {}
	virtual ~Primitive1D() {}

	//Entry Point
	virtual uint8 ClosestPoints(const Primitive1D&, Point4&, Point4&) const =0;

	//Closest points
	virtual uint8 ClosestPointsLine(const Line4&, Point4&, Point4&) const =0;
	virtual uint8 ClosestPointsRay(const Ray4&, Point4&, Point4&) const =0;
	virtual uint8 ClosestPointsLineSegment(const LineSegment4&, Point4&, Point4&) const =0;

};

#endif