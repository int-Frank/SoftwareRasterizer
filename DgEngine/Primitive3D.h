//================================================================================
// @ Primitive3D.h
// 
// Class: Primitive3D
//
// Base 3D Primitive class. Derives from Primitive.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef PRIMITIVE3D_H
#define PRIMITIVE3D_H

#include "Primitive.h"

class Primitive3D: public Primitive
{
public:
	//Constructor / destructor
	Primitive3D() {}
	virtual ~Primitive3D() {}

	virtual bool IsInside(const Point4&) const =0;
	
protected:
	//Data members

};

#endif