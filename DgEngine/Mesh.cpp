#include "Mesh.h"
#include "Polygon.h"
#include "Vertex.h"
#include "Common.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Point4.h"
#include "VQS.h"
#include "Matrix44.h"
#include "Light.h"
#include "Clipper.h"
#include "Materials.h"
#include "OBB.h"
#include "Sphere.h"
#include "MasterPList.h"
#include "Viewport.h"
#include <list>


//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Mesh::Mesh(): tag("NONE")
{
}	//End: Mesh::Mesh()


//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Mesh::Mesh(const Mesh& other)
{
	//Build Lists
	init(other);

}	//End: Mesh::Mesh()


//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Mesh& Mesh::operator=(const Mesh& other)
{
	if (this == &other)
		return *this;

	//Build Lists
	init(other);

	return *this;

}	//End: Mesh::operator=()


//--------------------------------------------------------------------------------
//		Set data from .obj file
//--------------------------------------------------------------------------------
DgReader& operator>> (DgReader& in, Mesh& dest)
{
	//Create temp lists to read to
	std::list<Polygon> temp_PList;
	std::list<Vertex> temp_VList;

	//Temp containers for input
	char chk;
	std::string str;
	Vector2 uv;
	Vector4 norm;
	Point4 pos;
	std::vector<Point4> v_pos;
	std::vector<Vector4> v_norm;
	std::vector<Vector2> v_uv;

	//Skip comments
	IgnoreComments(in);

	//Get tag
	in >> chk;
    if (chk == 'g' || chk == 'o')
    {
        in >> dest.tag;
    }
    else
    {
        std::cerr << "@operator>>(Mesh) -> Failed to read tag." << std::endl;
    }

	//Skip comments
	IgnoreComments(in);

	while (in >> str)
	{
		if (str == "vt")		//Texture coord
		{
			in >> uv;
			uv.y = 1.0f - uv.y;	//Flip v (as v is flipped in screen projection)
			v_uv.push_back(uv);
		}
		else if (str == "vn")	//normal
		{
			in >> norm;
			norm.Normalize();
			v_norm.push_back(norm);
		}
		else if (str == "v")	//xyz coord
		{
			in >> pos;

			v_pos.push_back(pos);
		}
		else if	(str == "f")	//Face
		{
			//Create temp data
			Polygon poly_temp;
			Vertex vertex_temp;
			int pos_ref, texel_ref, normal_ref;

			//First Vertex
			in >> pos_ref >> chk >> texel_ref >> chk >> normal_ref;
			
			vertex_temp.position = v_pos[pos_ref-1];
			vertex_temp.normal = v_norm[normal_ref-1];
				
			//Add to Polygon
			poly_temp.p0 = FindAdd(temp_VList, vertex_temp);
			poly_temp.uv0 = v_uv[texel_ref-1];


			//Second vertex
			in >> pos_ref >> chk >> texel_ref >> chk >> normal_ref;
			
			vertex_temp.position = v_pos[pos_ref-1];
			vertex_temp.normal = v_norm[normal_ref-1];
				
			//Add to Polygon
			poly_temp.p1 = FindAdd(temp_VList, vertex_temp);
			poly_temp.uv1 = v_uv[texel_ref-1];


			//Third Vertex
			in >> pos_ref >> chk >> texel_ref >> chk >> normal_ref;
			
			vertex_temp.position = v_pos[pos_ref-1];
			vertex_temp.normal = v_norm[normal_ref-1];
				
			//Add to Polygon
			poly_temp.p2 = FindAdd(temp_VList, vertex_temp);
			poly_temp.uv2 = v_uv[texel_ref-1];

			//Assign plane
			poly_temp.plane.Set(poly_temp.p0->position, 
								poly_temp.p1->position, 
								poly_temp.p2->position);

			//Add Polygon to PList
			temp_PList.push_back(poly_temp);
		}

		//Skip comments
		IgnoreComments(in);

	}


	//--------------------------------------------------------------------------------
	//		Now, build DgLinkedLists from the temp std::lists
	//--------------------------------------------------------------------------------

	//Resize lists
	dest.PList.resize(temp_PList.size());
	dest.VList.resize(temp_VList.size());

	//Copy contents of VList
	std::list<Vertex>::iterator vit = temp_VList.begin();
	for (; vit != temp_VList.end(); ++vit)
		dest.VList.push_back(*vit);

	//Copy contents of PList
	std::list<Polygon>::iterator pit = temp_PList.begin();
	for (; pit != temp_PList.end(); ++pit)
	{
		//Assign data
		Polygon poly = *pit;

		//Assign pointers
		poly.p0 = find(dest.VList, *pit->p0);
		poly.p1 = find(dest.VList, *pit->p1);
		poly.p2 = find(dest.VList, *pit->p2);

		//Check for errors
        if (poly.p0 == NULL || poly.p1 == NULL || poly.p2 == NULL)
        {
            std::cerr << "@Mesh::operator>>() -> Not all pointers assigned" << std::endl;
        }

		//Add polygon
		dest.PList.push_back(poly);
	}

	return in;
}	//End: operator>>(Mesh)


//--------------------------------------------------------------------------------
//		Back culling given a camera position
//--------------------------------------------------------------------------------
void Mesh::BackCull(const Point4& p)
{
	for (uint32 i = 0; i < PList.size(); ++i)
	{
		if (PList[i].plane.Test(p) < 0.0f)
			PList[i].state = 'x';	//Deactivate polygon
		else	//Activate vertices
		{
			PList[i].p0->state = 'a';
			PList[i].p1->state = 'a';
			PList[i].p2->state = 'a';
		}
	}

}	//End: Mesh::BackCull()


//--------------------------------------------------------------------------------
//		Send the object to a renderer
//--------------------------------------------------------------------------------
void Mesh::SendToRenderer( Viewport* view, const Materials& mat, const Mipmap* mm)
{
	//Send to clipper, clipper will output polygons to master list.
	for (uint32 i = 0; i < PList.size(); ++i)
	{
		//Check state
		if (PList[i].state == 'x')
			continue;

		//view->AddPolygon(PList[i], mat, mm);
	}
		
}	//End: Mesh::SentToRenderer()


//--------------------------------------------------------------------------------
//		Reset poly state
//--------------------------------------------------------------------------------
void Mesh::ResetStates()
{
	//Activate polys
	for (uint32 i = 0; i < PList.size(); ++i)
	{
		PList[i].reset();
	}

	//Deactivate vertices
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		VList[i].reset();
	}

}	//End: Mesh::ResetStates()


//--------------------------------------------------------------------------------
//		Activate all vertices
//--------------------------------------------------------------------------------
void Mesh::ActivateAll()
{
	//Activate polys
	for (uint32 i = 0; i < PList.size(); ++i)
	{
		PList[i].state = 'a';
	}

	//Activate vertices
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		VList[i].state = 'a';
	}

}	//End: Mesh::ActivateAll()


//--------------------------------------------------------------------------------
//		Transform active positions in each vertex.
//--------------------------------------------------------------------------------
void Mesh::TransformActiveVertices(const Matrix44& m)
{
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		//If the vertex is active
		if (VList[i].state == 'a')
		{
			VList[i].position_temp = m * VList[i].position;
		}
	}

}	//End: Mesh::TransformActiveVertices()


//--------------------------------------------------------------------------------
//		Transform all positions in each vertex.
//--------------------------------------------------------------------------------
void Mesh::TransformAllVertices(const Matrix44& m)
{
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		VList[i].position_temp = m * VList[i].position;
	}

}	//End: Mesh::TransformActiveVertices()


//--------------------------------------------------------------------------------
//		Transform active positions in each vertex.
//--------------------------------------------------------------------------------
void Mesh::TransformActiveVertices(const VQS& vqs)
{
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		//If the vertex is active
		if (VList[i].state == 'a')
		{
			VList[i].position_temp = vqs * VList[i].position;
		}
	}

}	//End: Mesh::TransformActiveVertices()


//--------------------------------------------------------------------------------
//		Transform all positions in each vertex.
//--------------------------------------------------------------------------------
void Mesh::TransformAllVertices(const VQS& vqs)
{
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		VList[i].position_temp = vqs * VList[i].position;
	}

}	//End: Mesh::TransformActiveVertices()


//--------------------------------------------------------------------------------
//		Build Lists (also copies bbox and lightsource)
//--------------------------------------------------------------------------------
void Mesh::init(const Mesh& other)
{
	//Copy data
	tag = other.tag;

	//Copy data
	PList = other.PList;
	VList = other.VList;

	//Assign Vertexs and UV coords in all polygons
	for (uint32 i = 0; i < PList.size(); ++i)
	{
		//Assign Vertexs
		PList[i].p0 = find(VList, *other.PList[i].p0);
		PList[i].p1 = find(VList, *other.PList[i].p1);
		PList[i].p2 = find(VList, *other.PList[i].p2);

		//Check for errors
        if (PList[i].p0 == NULL || PList[i].p1 == NULL || PList[i].p2 == NULL)
        {
            std::cerr << "@Mesh::BuildLists() -> Not all pointers assigned" << std::endl;
        }
	}

}	//End: Mesh::BuildLists()



//--------------------------------------------------------------------------------
//		Set Sphere and OBB from a mesh
//--------------------------------------------------------------------------------
void Mesh::SetBV(Sphere& sphere, OBB& box)
{
	//Check is VList is empty
	if (VList.empty())
	{
        std::cerr << "BoundingBox::SetFromMesh() -> Attempt to calculate bounding box for an empty object" <<
            std::endl;
		return;
	}

	//Compute minimal and maximal bounds
	float x_min = FLT_MAX, x_max = -FLT_MAX;
	float y_min = FLT_MAX, y_max = -FLT_MAX;
	float z_min = FLT_MAX, z_max = -FLT_MAX;

	//Find extremes
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		//Only check points
		if (VList[i].position.X() < x_min)
			x_min = (VList[i].position.X());
		if (VList[i].position.X() > x_max)
			x_max = (VList[i].position.X());
		if (VList[i].position.Y() < y_min)
			y_min = (VList[i].position.Y());
		if (VList[i].position.Y() > y_max)
			y_max = (VList[i].position.Y());
		if (VList[i].position.Z() < z_min)
			z_min = (VList[i].position.Z());
		if (VList[i].position.Z() > z_max)
			z_max = (VList[i].position.Z());
	}

	//Find half lengths
	Tuple<float> half_lengths;
	half_lengths[0] = 0.5f*(x_max - x_min);
	half_lengths[1] = 0.5f*(y_max - y_min);
	half_lengths[2] = 0.5f*(z_max - z_min);

	//Box center
	Point4 center(	(x_min + half_lengths[0]), 
					(y_min + half_lengths[1]), 
					(z_min + half_lengths[2]));

	//Set box data
	box.Set(center, BasisR3::xyz, half_lengths);

	//Loop through all vertices to find radius of sphere
	float radius2 = 0;

	//Loop through all vertices
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		//Find distance to current point
		Vector4 arm = center - VList[i].position;
		float dist2 = arm.LengthSquared();

		if (dist2 > radius2)
			radius2 = dist2;
	}

	//Set radius
	float radius = DgSqrt(radius2);

	//Set sphere
	sphere.Set(center, radius);
}	//End: SetBV()