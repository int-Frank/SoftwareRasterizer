#ifndef OBJECT_H
#define OBJECT_H

#include "BaseClass.h"
#include "DgTypes.h"

class VQS;
class Sphere;
class OBB;
class Frustum;
class Cone;
class Plane4;
class Line4;
class Ray4;
class LineSegment4;
class Point4;

//--------------------------------------------------------------------------------
/*
	Generic object class. Represents everything that can be represented in the 
	3D environment, eg geometries, lights, billboards...
*/
//--------------------------------------------------------------------------------
class Object : public BaseClass<Object>
{
public:
	//Constructor / destructor
	Object() {}
	virtual ~Object() {}

	//Copy operations
	Object(const Object&) {}

	//Transformations
	virtual void Transform(const VQS&) =0;
	virtual void TransformQuick(const VQS&) =0;

protected:
	//Data members

};

#endif