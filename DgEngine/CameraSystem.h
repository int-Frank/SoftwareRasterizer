//================================================================================
// @ CameraSystem.h
// 
// Class: CameraSystem
//
// The camera system combines a camera, frustum and viewport to simplify the 
// interaction between these objects.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "Camera.h"
#include "Frustum.h"
#include "ViewportHandler.h"

class VQS;
class Mesh;
class Skybox;
namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//	@	CameraSystem
//--------------------------------------------------------------------------------
class CameraSystem
{
public:
	//Constructor / destructor
	CameraSystem() {}
	~CameraSystem() {}

	//Copy operations
	CameraSystem(const CameraSystem&);
	CameraSystem& operator= (const CameraSystem&);

	//Input
	friend pugi::xml_node& operator>>(pugi::xml_node&, CameraSystem& dest);

	void Reset();	//Use after each frame

	//--------------------------------------------------------------------------------
	//		Specific to Viewport
	//--------------------------------------------------------------------------------

	bool SetViewport(viewportID);
	bool Activate();
	bool Deactivate();
	void ToggleActive();
	bool IsActive() const;
	bool IsAttached() const;

	//--------------------------------------------------------------------------------
	//		Specific to Camera
	//--------------------------------------------------------------------------------

	//Set functions
	void SetFOV(float hfov);

	//VQS manipulators
	void SetVQS(const VQS&);		//Set new VQS
	void UpdateVQS(const VQS&);		//Transform current VQS

	//--------------------------------------------------------------------------------
	//		Rendering
	//--------------------------------------------------------------------------------

	//Send objects down the pipeline
	void AddObject(Mesh*, const Materials&, const Mipmap*, uint8 planes);
	void AddSkybox(Skybox&);

	void Render();

	//--------------------------------------------------------------------------------
	//		Gets
	//--------------------------------------------------------------------------------
	const Frustum& GetFrustum() const {return frustum;}
	Viewport* GetViewport() { return view.GetViewport(); }
	Point4 CameraOrigin() const { return camera.GetPosition(); }
	Vector4 CameraDirection() const { return camera.Direction(); }

private:
	//Data members
	Camera camera;
	Frustum frustum;
	ViewportHandler view;

	//Functions
	friend void ReadCamera(pugi::xml_node&, CameraSystem& dest);

};

#endif