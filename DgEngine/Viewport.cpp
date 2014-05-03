//================================================================================
// @ Viewport.cpp
// 
// Description: This file defines Viewport's methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Viewport.h"
#include "CommonGraphics.h"
#include "WindowManager.h"
#include "Camera.h"
#include "Polygon.h"
#include "Dg_io.h"
#include "pugixml.hpp"
#include "Mesh.h"
#include "Particle.h"
#include "MessageBox.h"


//--------------------------------------------------------------------------------
//	@	Viewport::Viewport()
//--------------------------------------------------------------------------------
//		Constructor, Default viewpane size is 1x1 pixel
//--------------------------------------------------------------------------------
Viewport::Viewport(): dist(1.0f), wsc(0.0f), hsc(0.0f), near_clip(1.0f),
	absolute_x(0), absolute_y(0), parent_h(1), parent_w(1), 
	view_wd2(0.5f), view_hd2(0.5f), view_w_max(0.0f), view_h_max(0.0f),
	blend(DgGraphics::BlendType::NONE), cam_wd2(1.0f), cam_hd2(1.0f),
	relative_x(0.0f), relative_y(0.0f), relative_w(1.0f), relative_h(1.0f)
{
	//Set rasterizer output
	UpdateSize(parent_w, parent_h);

}	//End: Viewport::Viewport()


//--------------------------------------------------------------------------------
//	@	Viewport::init()
//--------------------------------------------------------------------------------
//		General copying
//--------------------------------------------------------------------------------
void Viewport::init(const Viewport& other)
{
	//Copy data
	flags = other.flags;

	clipper = other.clipper;
	masterPList = other.masterPList;
	rasterizer = other.rasterizer;

	viewpane = other.viewpane;

	blend = other.blend;
	
	absolute_x = other.absolute_x;
	absolute_y = other.absolute_y;
	parent_h = other.parent_h;
	parent_w = other.parent_w;

	dist = other.dist;
	near_clip = other.near_clip;
	wsc = other.wsc;
	hsc = other.hsc;
	view_wd2 = other.view_wd2;
	view_hd2 = other.view_hd2;
	view_w_max = other.view_w_max;
	view_h_max = other.view_h_max;
	cam_wd2 = other.cam_wd2;
	cam_hd2 = other.cam_hd2;

	relative_x = other.relative_x;
	relative_y = other.relative_y;
	relative_w = other.relative_w;
	relative_h = other.relative_h;

	//Functions
	zBuffer.resize(viewpane.w() * viewpane.h());
	rasterizer.SetOutput(viewpane, zBuffer);

}	//End: Viewport::init()


//--------------------------------------------------------------------------------
//	@	Viewport::Viewport()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Viewport::Viewport(const Viewport& other)
{
	init(other);

}	//End: Viewport::Viewport()


//--------------------------------------------------------------------------------
//	@	Viewport::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Viewport& Viewport::operator=(const Viewport& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;

}	//End: Viewport::operator=()


//--------------------------------------------------------------------------------
//		Builds the Viewport from an xml file
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Viewport& dest)
{
	float x_rel = dest.relative_x;
	float y_rel = dest.relative_y;
	float w_rel = dest.relative_w;
	float h_rel = dest.relative_h;

	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

		if (tag == "blend")
		{
			uint32 blend;
			if (StringToNumber(blend, it->child_value(), std::dec))
			{
				dest.SetBlend(DgGraphics::BlendType(blend));
			}
		}
		else if (tag == "x_rel")
		{
			StringToNumber(x_rel, it->child_value(), std::dec);
		}
		else if (tag == "y_rel")
		{
			StringToNumber(y_rel, it->child_value(), std::dec);
		}
		else if (tag == "w_rel")
		{
			StringToNumber(w_rel, it->child_value(), std::dec);
		}
		else if (tag == "h_rel")
		{
			StringToNumber(h_rel, it->child_value(), std::dec);
		}
    }

	dest.SetRelativeData(x_rel, y_rel, w_rel, h_rel);

	return node;

}	//End: GameDatabase::BuildDataFromCamera()


//--------------------------------------------------------------------------------
//	@	Viewport::SetRelativeData()
//--------------------------------------------------------------------------------
//		Sets relative position and dimensions
//--------------------------------------------------------------------------------
void Viewport::SetRelativeData(float x_rel, float y_rel, float w_rel, float h_rel)
{
	//Input value checks.
	if (w_rel < 0.0f || h_rel < 0.0f)
	{
		flags.isGood = false;
		return;
	}

	relative_x = x_rel;
	relative_y = y_rel;
	relative_w = w_rel;
	relative_h = h_rel;

	UpdateSize(parent_w, parent_h);

	//Success
	flags.isGood = true;

}	//End: Viewport::SetRelativeData()


//--------------------------------------------------------------------------------
//	@	Viewport::SetViewpane()
//--------------------------------------------------------------------------------
//		Set absolute viewpane dimensions.
//--------------------------------------------------------------------------------
void Viewport::UpdateSize(uint32 _parent_w, uint32 _parent_h)
{
	//Input value checks.
	if (_parent_w < 1 || _parent_h < 1)
	{
		flags.isGood = false;
		return;
	}

	//Assign data
	parent_w = _parent_w;
	parent_h = _parent_h;

	//Calculate absolute values
	uint32 new_w = uint32 (float(parent_w) * relative_w);
	uint32 new_h = uint32 (float(parent_h) * relative_h); 
	absolute_x = uint32 (float(parent_w) * relative_x);
	absolute_y = uint32 (float(parent_h) * relative_y);

	//ensure new sizes are valid.
	new_w = (new_w > 0) ? new_w : 1;
	new_h = (new_h > 0) ? new_h : 1;

	//Set viewpane size
	::Resize(viewpane, new_h, new_w);
	viewpane.Flush();
	
	//Clear masterPList
	masterPList.Reset();

	//Set zBuffer
	zBuffer.resize(new_h*new_w);

	//Set rasterizer
	rasterizer.SetOutput(viewpane, zBuffer);

	//Set projections data.
	SetProjectionData();

	//Success
	flags.isGood = true;

}	//End: Viewport::SetViewpane()


//--------------------------------------------------------------------------------
//	@	Viewport::Initialise()
//--------------------------------------------------------------------------------
//		Set clipping and projection data.
//--------------------------------------------------------------------------------
void Viewport::Initialise(const Frustum& f, float _cam_wd2, 
						  float _cam_hd2, float _nearClip)
{
	cam_wd2 = _cam_wd2;
	cam_hd2 = _cam_hd2;
	near_clip = -_nearClip;

	SetProjectionData();

	clipper.SetFrustum(f);

}	//End: Viewport::Initialise()


//--------------------------------------------------------------------------------
//	@	Viewport::AddObject()
//--------------------------------------------------------------------------------
//		Send an object down the graphics pipeline.
//		Pre:	mesh is a valid pointer.
//		Post:	Clips and projects an objects masterPList and adds them to the
//				master polygon list.
//--------------------------------------------------------------------------------
void Viewport::AddObject(Mesh* mesh, const Materials& materials, 
	const Mipmap* mipmap, uint8 planes)
{
	//Extract polygon list.
	DgArray<Polygon>& polygons = mesh->GetPolygons();

	//If completely inside frustum, bypass clipping
	if (planes == Frustum::INSIDE)
	{
		//Extract vertex list.
		DgArray<Vertex>& vertices = mesh->GetVertices();

		//Project active vertices in the mesh
		for (uint32 i = 0; i < vertices.size(); ++i)
		{
			if (vertices[i].state == 'x')
				continue;

			//--------------------------------------------------------------------------------
			//		Project points. NOTE: Must ensure pos.z >= 1.
			//		This is to simplify rasterization.
			//		Do this by dividing all z distances by the near clipping distance.
			//--------------------------------------------------------------------------------
			float val = dist / vertices[i].position_temp.Z();
			vertices[i].position_temp.X() = wsc * vertices[i].position_temp.X() * val + view_wd2;
			vertices[i].position_temp.Y() = view_hd2 - hsc * vertices[i].position_temp.Y() * val;
			vertices[i].position_temp.Z() /= near_clip;
		}

		//Send active polygons to masterlist
		for (uint32 i = 0; i < polygons.size(); ++i)
		{
			//Check state
			if (polygons[i].state == 'x')
				continue;

			Polygon_RASTER Ptemp(polygons[i], &materials, mipmap);

			//Output Ptemp;
			masterPList.Add(Ptemp);
		}
	}
	else if (planes != Frustum::OUTSIDE)
	{
		//Send to clipper.
		for (uint32 i = 0; i < polygons.size(); ++i)
		{
			//Check state
			if (polygons[i].state == 'x')
				continue;

			//Clip polygon.
			Clipper::Point* start(NULL);
			uint8 size;

			if (!clipper.ClipPolygon(polygons[i], planes, start, size))
				continue;

			//Project
			ProjectFromClipper(start, size);

			//Strip off triangles and add to master polygon list
			//Get first triangle
			Clipper::Point* p1(start->next);
			Clipper::Point* p2(p1->next);
			
			Polygon_RASTER Ptemp;

			//Assign extra data
			Ptemp.materials = &materials;
			Ptemp.mipmap = mipmap;

			for (uint32 j = 2; j < size; j++)
			{
				//Assign Vertices to Ptemp
				Ptemp.p0.pos = start->vertex.pos;
				Ptemp.p1.pos = p1->vertex.pos;
				Ptemp.p2.pos = p2->vertex.pos;

				Ptemp.p0.clr = start->vertex.clr;
				Ptemp.p1.clr = p1->vertex.clr;
				Ptemp.p2.clr = p2->vertex.clr;

				Ptemp.p0.uv = start->vertex.uv;
				Ptemp.p1.uv = p1->vertex.uv;
				Ptemp.p2.uv = p2->vertex.uv;

				//Output Ptemp;
				masterPList.Add(Ptemp);

				//Increment to next points
				p1 = p2;
				p2 = p2->next;
			}

		}
	}
}	//End: Viewport::AddObject()


//--------------------------------------------------------------------------------
//	@	Viewport::AddParticle()
//--------------------------------------------------------------------------------
//		Send a particle down the graphics pipeline.
//		Assumes particle is at least partly in the view frustum.
//--------------------------------------------------------------------------------
void Viewport::AddParticle(const Particle& input, const ParticleAlphaTemplate* pat)
{
	Particle_RASTER temp;

	//Project location
	float val = dist / input.screenPosition.Z();
	temp.position.X() = wsc * input.screenPosition.X() * val + view_wd2;
	temp.position.Y() = view_hd2 - hsc * input.screenPosition.Y() * val;
	temp.position.Z() = input.screenPosition.Z() / near_clip;

	//Project size
	temp.radius = (wsc + hsc) * 0.5f * input.screenSize * val;

	//How far along in life is it?
	float dl = input.life / input.life_total;

	//Adjust colors
	float red = input.red_start + (input.d_red * dl);
	float green = input.green_start + (input.d_green * dl);
	float blue = input.blue_start + (input.d_blue * dl);
	float alpha = input.alpha_start + (input.d_alpha * dl);

	//Build final color
	uint32 red_ui = uint32(red) << 16;
	uint32 green_ui = uint32(green) << 8;
	uint32 blue_ui = uint32(blue);
	uint32 alpha_ui = uint32(alpha) << 24;

	temp.color = (((red_ui | green_ui) | blue_ui) | alpha_ui);;

	//Assign alpha template
	temp.alphaTemplate = pat;

	//Output temp;
	masterPList.Add(temp);

}	//End: Viewport::AddParticle()


//--------------------------------------------------------------------------------
//	@	Viewport::AddSkyboxPolygon()
//--------------------------------------------------------------------------------
//		Send a skybox object down the graphics pipeline.
//		Pre:	mesh and image are pointers to valid objects(not NULL pointers).
//		Post:	Clips and projects an objects masterPList and adds them to the
//				master polygon list.
//--------------------------------------------------------------------------------
void Viewport::AddSkyboxPolygon(const Polygon& polygon, const Image* image)
{
	//Clip polygon.
	Clipper::Point* start(NULL);
	uint8 size;

	if (!clipper.ClipPolygon(polygon, Frustum::ALL_BUT_FAR, start, size))
		return;

	//Project
	ProjectFromClipper(start, size);

	//Strip off triangles and add to master polygon list
	//Get first triangle
	Clipper::Point* p1(start->next);
	Clipper::Point* p2(p1->next);

	Polygon_RASTER_SB Ptemp;

	//Assign extra data
	Ptemp.image = image;

	for (uint32 j = 2; j < size; j++)
	{
		//Assign Vertices to Ptemp
		Ptemp.p0.pos = start->vertex.pos;
		Ptemp.p1.pos = p1->vertex.pos;
		Ptemp.p2.pos = p2->vertex.pos;

		Ptemp.p0.clr = start->vertex.clr;
		Ptemp.p1.clr = p1->vertex.clr;
		Ptemp.p2.clr = p2->vertex.clr;

		Ptemp.p0.uv = start->vertex.uv;
		Ptemp.p1.uv = p1->vertex.uv;
		Ptemp.p2.uv = p2->vertex.uv;

		//Output Ptemp;
		masterPList.Add(Ptemp);

		//Increment to next points
		p1 = p2;
		p2 = p2->next;
	}

}	//End: Viewport::AddSkyboxPolygon()


//--------------------------------------------------------------------------------
//	@	Viewport::ClipAndProject()
//--------------------------------------------------------------------------------
//		Clip and project a polygon
//--------------------------------------------------------------------------------
void Viewport::ProjectFromClipper(Clipper::Point* start, uint8 size)
{
	//--------------------------------------------------------------------------------
	//		Project points. NOTE: Must ensure pos.z >= 1.
	//		This is to simplify rasterization.
	//		Do this by dividing all z distances by the near clipping distance.
	//--------------------------------------------------------------------------------
	for (uint8 j = 0; j < size; ++j)
	{
		float val = dist / start->vertex.pos.Z();
		float temp_x = wsc * start->vertex.pos.X() * val + view_wd2;
		float temp_y = view_hd2 - hsc * start->vertex.pos.Y() * val;
		start->vertex.pos.Z() /= near_clip;

		//Ensure all coords are on the viewpane
		if (temp_x < EPSILON)
			start->vertex.pos.X() = 0.0f;
		else if (temp_x > view_w_max)
			start->vertex.pos.X() = view_w_max;
		else
			start->vertex.pos.X() = temp_x;

		if (temp_y < EPSILON)
			start->vertex.pos.Y() = 0.0f;
		else if (temp_y > view_h_max)
			start->vertex.pos.Y() = view_h_max;
		else
			start->vertex.pos.Y() = temp_y;

		start = start->next;
	}
}	//End: Viewport::ProjectFromClipper()


//--------------------------------------------------------------------------------
//	@	Viewport::Reset()
//--------------------------------------------------------------------------------
//		Reset all data
//--------------------------------------------------------------------------------
void Viewport::Reset(bool clearVP)
{
	//Clear ZBuffer
	int32* zbuf(zBuffer.Data());
	uint32 zbuf_size = zBuffer.max_size();
	for (uint32 i = 0; i < zbuf_size; i++)
		zbuf[i] = 0;

	masterPList.Reset();

	if (clearVP)
		viewpane.Flush();

}	//End: Viewport::Reset()


//--------------------------------------------------------------------------------
//	@	Viewport::BlitToImage()
//--------------------------------------------------------------------------------
//		Applys the viewpane to an Image
//--------------------------------------------------------------------------------
void Viewport::BlitToImage(Image& dest) const
{
	ApplyImage(viewpane, dest, absolute_x, absolute_y, blend);

}	//End: Viewport::BlitToImage()


//--------------------------------------------------------------------------------
//	@	Viewport::BlitImage()
//--------------------------------------------------------------------------------
//		Blit an Image to the viewpane
//--------------------------------------------------------------------------------
void Viewport::BlitImage(const Image& img, int32 atX, int32 atY, 
						 DgGraphics::BlendType b)
{
	ApplyImage(img, viewpane, atX, atY, b);

}	//End: Viewport::BlitImage()


//--------------------------------------------------------------------------------
//	@	Viewport::DrawShape()
//--------------------------------------------------------------------------------
//		Draw a shape on the image
//--------------------------------------------------------------------------------
void Viewport::DrawMessageBox(const MessageBox& m)
{
	m.Draw(viewpane);

}	//End: Viewport::BlitImage()


//--------------------------------------------------------------------------------
//	@	Viewport::Render()
//--------------------------------------------------------------------------------
//		Draw all masterPList to the viewpane
//--------------------------------------------------------------------------------
void Viewport::Render()
{
	masterPList.SendToRasterizer(rasterizer);

}	//End: Viewport::RenderToViewpane()


//--------------------------------------------------------------------------------
//	@	Viewport::MaskOut()
//--------------------------------------------------------------------------------
//		Set a portion of the zBuffer to 0
//--------------------------------------------------------------------------------
void Viewport::MaskOut(DgRect rect)
{
	//Clip mask to viewpane
	if (!ClipRect(rect, viewpane.w(), viewpane.h()))
		return;

	int32* data = zBuffer.Data();
	int32 vw = int32(viewpane.w());

	//Mask out
	for (int32 j = rect.y; j < rect.y+int32(rect.h); ++j)
	{
		for (int32 i = rect.x; i < rect.x+int32(rect.w); ++i)
		{
			zBuffer[j*vw + i] = 0;
		}
	}

}	//End: Viewport::MaskOut()


//--------------------------------------------------------------------------------
//	@	Viewport::BlitToRenderer()
//--------------------------------------------------------------------------------
//		Applys the viewpane to the viewpane of another Viewport
//--------------------------------------------------------------------------------
void Viewport::BlitToRenderer(Viewport& dest) const
{
	ApplyImage(viewpane, dest.viewpane, absolute_x, absolute_y, blend);

}	//End: Viewport::BlitTo()


//--------------------------------------------------------------------------------
//	@	Viewport::BlitToWindow()
//--------------------------------------------------------------------------------
//		Applys the viewpane directly to a window
//--------------------------------------------------------------------------------
void Viewport::BlitToWindow(WindowManager& dest) const
{
	dest.UpdateBuffer(viewpane, absolute_x, absolute_y);

}	//End: Viewport::BlitToWindow()


//--------------------------------------------------------------------------------
//	@	Viewport::SetProjectionData()
//--------------------------------------------------------------------------------
//		Sets dist, wsc, hsc; projection data
//--------------------------------------------------------------------------------
void Viewport::SetProjectionData()
{
	view_wd2 = float(viewpane.w()) / 2.0f;
	view_hd2 = float(viewpane.h()) / 2.0f;

	view_w_max = float(viewpane.w()) - 1.0f;
	view_h_max = float(viewpane.h()) - 1.0f;

	//Find aspect ratios for camera space and window space
	float ascam = cam_wd2 / cam_hd2;
	float aswin = view_wd2 / view_hd2;

	//if camera space is longer than window space
	if (ascam > aswin)
	{
		//Find dist (width is filled, height is stretched)
		dist = -view_wd2 / cam_wd2;

		//Set scales
		wsc = 1.0f;
		hsc = view_hd2 / (-dist * cam_hd2);
	}
	//else if camera space is taller than window space
	else if (ascam < aswin)
	{
		//Find dist (height is filled, width is stretched)
		dist = -view_hd2 / cam_hd2;

		//Set scales
		hsc = 1.0f;
		wsc = view_wd2 / (-dist * cam_wd2);
	}
	//else ratios are equal
	else
	{
		//Find dist (camera dimensions fits into window)
		dist = -view_hd2 / cam_hd2;

		//Set scales
		hsc = 1.0f;
		wsc = 1.0f;
	}

}	//End: Viewport::SetProjectionData()