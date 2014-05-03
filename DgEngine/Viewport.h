//================================================================================
// @ Viewport.h
// 
// Class: Viewport
//
// Viewport with a graphics pipeline.
//
// Simplifies rendering. Each instance of this class has its own viewpane to
// draw to. A viewport can gather masterPList to render to its viewpane at a later 
// stage, or, Images can be directly 'blitted' to the viewpane. Typically the 
// data is set to correspond with a camera object, so that the camera view looks
// natural on the viewpane. Once the viewpane is composed, it is typically
// applied to some master Image to be rendered directly onto the screen.
//
// To set up the viewport, call these functions in this order:
//
//		SetRelativeData(float x_rel, float y_rel, float w_rel, float h_rel)
//		SetViewpane(uint32 parent_w, uint32 parent_h);
//
//		//And then one of these to set up the clipper
//		SetViewportFromCamera(const Camera&);	
//		SetCameraFromViewport(Camera&);
//
// The renderer will now be ready to use.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef RENDERER_H
#define RENDERER_H

#include "Clipper.h"
#include "MasterPList.h"
#include "Image.h"
#include "Rasterizer.h"
#include "DgTypes.h"
#include "DgArray.h"
#include "DgRect.h"
#include "Text.h"
#include "Particle_RASTER.h"

namespace DgGraphics{enum BlendType;}
namespace pugi{class xml_node;}
struct Polygon;
class Mesh;
class Materials;
class Mipmap;
class Camera;
class WindowManager;
struct Particle;
class ParticleAlphaTemplate;
class MessageBox;

//--------------------------------------------------------------------------------
//	@	Viewport
//--------------------------------------------------------------------------------
class Viewport
{
	friend class ViewportManager;
	struct Flags;
public:
	//Constructor / destructor
	Viewport();
	~Viewport() {}

	//Copy operations
	Viewport(const Viewport&);
	Viewport& operator= (const Viewport&);
	
	//Build object from xml file and a Camera
	friend pugi::xml_node& operator>>(pugi::xml_node&, Viewport&);

	//--------------------------------------------------------------------------------
	//		Set up the viewport
	//--------------------------------------------------------------------------------
	
	//Set the relative dimensions of the viewport.
	void SetRelativeData(float x_rel, float y_rel, float w_rel, float h_rel);
	
	//Update the viewport size from parent dimensions.
	void UpdateSize(uint32 _parent_w, uint32 _parent_h);
	
	//Link viewport to a camera. This must be set before objects can be
	//passed to the viewport.
	void Initialise(const Frustum&, float _cam_wd2, float _cam_hd2, float _nearClip);

	//Is the viewpane colorkeyed, alpha etc...
	void SetBlend(DgGraphics::BlendType new_blend) {blend = new_blend;}

	//Reset
	void Reset(bool flushVP = true);

	void SetThreadNumber(uint32);

	//--------------------------------------------------------------------------------
	//		Adding content
	//--------------------------------------------------------------------------------

	//Add objects to the rendering lists
	void AddObject(Mesh*, const Materials&, const Mipmap*, uint8 planes);
	void AddParticle(const Particle&, const ParticleAlphaTemplate*);
	void AddSkyboxPolygon(const Polygon&, const Image*);

	//Blit an Image to the viewpane
	void BlitImage(const Image&, int32 atX, int32 atY, DgGraphics::BlendType);

	//Draw a shape on the viewplane;
	void DrawMessageBox(const MessageBox&);

	//Render polygon lists to the internal viewpane
	void Render();

	//Set a portion of the zBuffer to 0
	void MaskOut(DgRect);
	
	//--------------------------------------------------------------------------------
	//		Exporting the viewpane
	//--------------------------------------------------------------------------------

	//Blit viewpane to an external image (typically the screen)
	void BlitToImage(Image&) const;
	void BlitToRenderer(Viewport&) const;
	void BlitToWindow(WindowManager&) const;

	//--------------------------------------------------------------------------------
	//		Gets
	//--------------------------------------------------------------------------------
	int32 x() const {return absolute_x;}
	int32 y() const {return absolute_y;}
	uint32 w() const {return viewpane.w();}
	uint32 h() const {return viewpane.h();}

	//--------------------------------------------------------------------------------
	//		Flag control
	//--------------------------------------------------------------------------------

	void SwitchActive(bool b) {flags.active = b;}
	void SwitchLocked(bool b) {flags.locked = b;}
	bool IsActive() const	{ return flags.active; }
	bool IsLocked() const	{ return flags.locked; }
	bool IsGood() const		{ return flags.isGood; }

	//--------------------------------------------------------------------------------
	//		Gets
	//--------------------------------------------------------------------------------

	Image& Viewpane()			{ return viewpane; }
	DgArray<int32>& ZBuffer()	{ return zBuffer; }
	
private:
	//Data members
	//Flags
	struct Flags
	{
		Flags(): active(false), locked(false), isGood(false) {}
		bool active;
		bool locked;
		bool isGood;
	} flags;

	//Objects that do the work
	Clipper clipper;
	MasterPList masterPList;
	Rasterizer rasterizer;

	//Everything is drawn to here
	Image viewpane;			
	DgArray<int32> zBuffer;

	//Thread management for rasterization
	uint32 nThreads;
	DgArray<Plane4> innerPlanes;

	//Projection data
	float dist;		//Distance to the viewplane
	float wsc, hsc;
	float view_wd2, view_hd2;
	float view_w_max, view_h_max;
	float near_clip;
	float cam_wd2, cam_hd2;

	//How the viewpane is blended with other Images/viewpanes.
	DgGraphics::BlendType blend;	

	//Position on the target to blit to.
	int32 absolute_x; 
	int32 absolute_y;

	//Parent dimensions
	uint32 parent_h;
	uint32 parent_w;

	//Relative positions and sizes on the window
	float relative_x;
	float relative_y;
	float relative_w;
	float relative_h;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void init(const Viewport&);
	void SetProjectionData();
	void ProjectFromClipper(Clipper::Point* start, uint8 size);

};

#endif