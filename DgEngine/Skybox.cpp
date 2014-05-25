#include "Skybox.h"
#include "Image.h"
#include "Polygon.h"
#include "Vertex.h"
#include "Common.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Point4.h"
#include "VQS.h"
#include "Dg_io.h"
#include "pugixml.hpp"
#include "ImageManager.h"
#include "TextureManager.h"
#include "Mesh_List.h"
#include "Clipper.h"
#include "Viewport.h"
#include "Dg_io.h"
#include <vector>

//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Skybox::Skybox(): top(NULL), bottom(NULL), left(NULL),
	right(NULL), front(NULL), back(NULL)
{
	//Load geometry
	cube = MESH_MANAGER[obj_file];

}	//End: Skybox::Skybox()


//--------------------------------------------------------------------------------
//	@	Skybox::init()
//--------------------------------------------------------------------------------
//		Copy function
//--------------------------------------------------------------------------------
void Skybox::init(const Skybox& other)
{
	//Copy atomic data
	top = other.top;
	bottom = other.bottom;
	left = other.left;
	right = other.right;
	front = other.front;
	back = other.back;

	Q_WLD_OBJ = other.Q_WLD_OBJ;

}	//End: Skybox::init()


//--------------------------------------------------------------------------------
//	@	Skybox::Skybox()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Skybox::Skybox(const Skybox& other) : cube(MESH_MANAGER[obj_file])
{
	init(other);

}	//End: Skybox::Skybox()


//--------------------------------------------------------------------------------
//	@	Skybox::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Skybox& Skybox::operator=(const Skybox& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;

}	//End: Skybox::operator=()


//--------------------------------------------------------------------------------
//		Read data into object
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Skybox& dest)
{
	//iterate through all nodes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "texture")
		{
			dest.SetTexture(it->child_value());
		}
		else if (tag == "orientation")
		{
			Quaternion temp;
			DgString(it->child_value()) >> temp;
			dest.SetOrientation(temp);
		}

    }

	return node;
}	//End: operator>>()


//--------------------------------------------------------------------------------
//		Sets new orientation
//--------------------------------------------------------------------------------
void Skybox::SetOrientation(const Quaternion& q)
{
	//Set rotation
	Q_WLD_OBJ = q;

}	//End: Skybox::SetOrientation()


//--------------------------------------------------------------------------------
//		Load Images
//--------------------------------------------------------------------------------
void Skybox::SetTexture(const std::string& name)
{
	//Open file
	pugi::xml_node node = TEXTURE_MANAGER.Find("skybox", name);

	if (node.empty())
	{
		top = bottom = left = right = front = back = NULL;
        std::cerr << "@Skybox::SetTexture -> Image set '" << name << "' not found." << std::endl;
	}
		
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "top")			top		= IMAGE_MANAGER.GetImage(it->child_value());
		else if (tag == "bottom")	bottom	= IMAGE_MANAGER.GetImage(it->child_value());
		else if (tag == "right")	right	= IMAGE_MANAGER.GetImage(it->child_value());
		else if (tag == "left")		left	= IMAGE_MANAGER.GetImage(it->child_value());
		else if (tag == "back")		back	= IMAGE_MANAGER.GetImage(it->child_value());
		else if (tag == "front")	front	= IMAGE_MANAGER.GetImage(it->child_value());

	}

    if ((top == NULL) || (bottom == NULL) ||
        (right == NULL) || (left == NULL) ||
        (front == NULL) || (back == NULL))
    {
        std::cerr << "@Skybox::SetTexture -> Some images not assigned" << std::endl;
    }

}	//End: Skybox::SetTexture()


//--------------------------------------------------------------------------------
//	@	Skybox::OrientateCubeToCamera()
//--------------------------------------------------------------------------------
//		Orientates the cube to camera space
//--------------------------------------------------------------------------------
void Skybox::OrientateCubeToCamera(const Quaternion& Q_CAM_WLD)
{
	//Transform the skybox
	Quaternion q = Q_CAM_WLD * Q_WLD_OBJ;

	//Get mesh vertices
	DgArray<Vertex>& VList = cube->GetVertices();

	//Transform vertices
	for (uint32 i = 0; i < VList.size(); ++i)
	{
		VList[i].position_temp = q.Rotate(VList[i].position);
	}

}	//End: Skybox::OrientateCubeToCamera()


//--------------------------------------------------------------------------------
//		Send Skybox down the graphics pipeline.
//--------------------------------------------------------------------------------
void Skybox::SendToRenderer(Viewport* rend) const
{
	//Get mesh vertices
	DgArray<Polygon>& PList = cube->GetPolygons();

	rend->AddSkyboxPolygon(PList[0], top);
	rend->AddSkyboxPolygon(PList[1], top);
	rend->AddSkyboxPolygon(PList[2], bottom);
	rend->AddSkyboxPolygon(PList[3], bottom);
	rend->AddSkyboxPolygon(PList[4], left);
	rend->AddSkyboxPolygon(PList[5], left);
	rend->AddSkyboxPolygon(PList[6], right);
	rend->AddSkyboxPolygon(PList[7], right);
	rend->AddSkyboxPolygon(PList[8], front);
	rend->AddSkyboxPolygon(PList[9], front);
	rend->AddSkyboxPolygon(PList[10], back);
	rend->AddSkyboxPolygon(PList[11], back);
	
		
}	//End: Skybox::AddToMasterPList()