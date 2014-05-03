#ifndef MESH_H
#define MESH_H

#include "Dg_io.h"
#include <string>
#include "DgArray.h"
#include "Polygon.h"
#include "Vertex.h"

class Point4;
class Matrix44;
class Light;
class VQS;
class Materials;
class Mipmap;
class Sphere;
class OBB;
class Viewport;

//--------------------------------------------------------------------------------
/*
	Base object class. 
*/
//--------------------------------------------------------------------------------
class Mesh
{
	friend class Mesh_List;
	friend class Materials;
public:
	//Constructor/Destructor
	Mesh();
	~Mesh() {}

	//Copy operations
	Mesh(const Mesh&);
	Mesh& operator=(const Mesh&);

	//Input
	friend DgReader& operator>> (DgReader& in, Mesh& dest);
	
	void SetTag(const std::string& str) {tag = str;}
	const std::string& GetTag() {return tag;}

	//Comparison
	bool operator==(const Mesh& o) {return tag == o.tag;}
	bool operator!=(const Mesh& o) {return tag != o.tag;}

	//Set Physics from mesh
	void SetBV(Sphere&, OBB&);

	//Data manipulators for RENDERING purposes ONLY
	void BackCull(const Point4& p);	//Deactivates polys givin camera position
	
	//Reset Polygon and Vertex states:
	//Polygon on, Vertex off.
	void ResetStates();
	void ActivateAll();

	//Send polygons to clipper
	void SendToRenderer( Viewport*, const Materials&, const Mipmap*);

	//Transformation functions

	//Transform Vertices
	void TransformActiveVertices(const Matrix44&);
	void TransformAllVertices(const Matrix44&);
	void TransformActiveVertices(const VQS&);
	void TransformAllVertices(const VQS&);

	//Get the lists
	DgArray<Polygon>& GetPolygons() {return PList;}
	DgArray<Vertex>& GetVertices() {return VList;}

protected:
	//Data members
	std::string tag;			//Name of the object
	DgArray<Polygon>	PList;	//Polygon list
	DgArray<Vertex>		VList;	//Vertex list
	
	//Copies lists from other Meshs
	void init(const Mesh& other);
};



#endif