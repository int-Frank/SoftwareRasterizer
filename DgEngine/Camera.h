//================================================================================
// @ Camera.h
// 
// Class: Camera
//
// The camera class is essential to provide a portal to 'see' the game world. This
// class defines a camera's position, axes, horizontal and vertical FOV, and 
// near and far clipping distances. 
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef CAMERA_H
#define CAMERA_H

#include "DgTypes.h"
#include "BasisR3.h"
#include "Point4.h"
#include "Frustum.h"

class VQS;
namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//	@	Camera
//--------------------------------------------------------------------------------
class Camera
{
public:
	//Constructor/Destructor
	Camera();
	Camera(VQS new_vqs,
		float hfov,
		float vfov,
		float _near_clip_z,
		float _far_clip_z);
	~Camera() {}

	//Copy constructor
	Camera(const Camera&);
	Camera& Camera::operator=(const Camera&);

	//Set all data
	void Set(VQS new_vqs,
		float hfov,
		float vfov,
		float _near_clip_z,
		float _far_clip_z);

	//Set functions
	void SetNearFar(float near, float far);
	void SetFOV(float hfov, float vfov);
	void SetViewplane(float _wd2, float _hd2);

	//Create a frustum
	void SetFrustum(Frustum&) const;

	//Get frustum in camrea space
	Frustum BaseFrustum() const;

	//VQS manipulators
	void SetVQS(const VQS&);		//Set new VQS
	void UpdateVQS(const VQS&);		//Transform current VQS
	
	//Return functions
	const Point4& GetPosition() const {return origin;}

	float HFOV() const;
	float VFOV() const;
	float NearZ() const { return near_clip_z; }
	float Wd2() const { return wd2; }
	float Hd2() const { return hd2; }
	Vector4 Direction() const { return basis.x2(); }

	static const BasisR3 basis_base;

private:

	//Data members
	Point4 origin;
	BasisR3 basis;		//u,v,n vectors for uvn system
	
	//3D Clipping
	float near_clip_z;	//Near clipping plane
	float far_clip_z;	//Far clipping plane

	float wd2;			//Half Width of viewplane at distance 1 away from origin
	float hd2;			//Half Height of viewplane at distance 1 away from origin

	//--------------------------------------------------------------------------------	
	//		Functions
	//--------------------------------------------------------------------------------
	
	//Copy from other camera
	void init(const Camera&);

};


#endif