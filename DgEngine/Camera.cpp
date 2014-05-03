//================================================================================
// @ Camera.cpp
// 
// Description: This file defines Camera's methods.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Camera.h"
#include <string>
#include "VQS.h"
#include "Clipper.h"
#include "pugixml.hpp"

//--------------------------------------------------------------------------------
//		Statics
//--------------------------------------------------------------------------------
const BasisR3 Camera::basis_base(Vector4(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f));


//--------------------------------------------------------------------------------
//	@	Camera::Camera()
//--------------------------------------------------------------------------------
//		Default constructor
//--------------------------------------------------------------------------------
Camera::Camera(): origin(Point4::origin), basis(basis_base)
{
	//Default values
	Set(VQS(), 1.5f, 1.5f, 0.5f, 100.0f);

}	//End: Camera::Camera()


//--------------------------------------------------------------------------------
//	@	Camera::Camera()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Camera::Camera(VQS new_vqs,
	float hfov,
	float vfov,
	float _near_clip_z,
	float _far_clip_z)
{
	Set(new_vqs, hfov, vfov, _near_clip_z, _far_clip_z);

}	//End: Camera::Camera()


//--------------------------------------------------------------------------------
//	@	Camera::Camera()
//--------------------------------------------------------------------------------
//		Initiate from other
//--------------------------------------------------------------------------------
void Camera::init(const Camera& other)
{
	//Copy data
	basis = other.basis;
	origin = other.origin;
	near_clip_z = other.near_clip_z;
	far_clip_z = other.far_clip_z;
	wd2 = other.wd2;
	hd2 = other.hd2;

}	//End: Camera::init()


//--------------------------------------------------------------------------------
//	@	Camera::Camera()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Camera::Camera(const Camera& other)
{
	//Copy data
	init(other);

}	//End: Camera::Camera()


//--------------------------------------------------------------------------------
//	@	Camera::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Camera& Camera::operator=(const Camera& other)
{
	//Copy data
	init(other);

	return *this;
}	//End: Camera::operator=()


//--------------------------------------------------------------------------------
//	@	Camera::Set)
//--------------------------------------------------------------------------------
//		Set up the camera
//--------------------------------------------------------------------------------
void Camera::Set(VQS new_vqs,
	float hfov,
	float vfov,
	float _near_clip_z,
	float _far_clip_z)
{
	//Set Data
	near_clip_z = _near_clip_z;
	far_clip_z = _far_clip_z;

	//Set screen
	SetFOV(hfov, vfov);

	//Set VQS
	SetVQS(new_vqs);

}	//End: Camera::Set()


//--------------------------------------------------------------------------------
//	@	Camera::SetFrustum()
//--------------------------------------------------------------------------------
//		Build a frustum off this camera
//--------------------------------------------------------------------------------
void Camera::SetFrustum(Frustum& frustum) const
{
	frustum.Build(origin, basis, near_clip_z, far_clip_z, wd2, hd2);
}	//End: Camera::SetFrustum()


//--------------------------------------------------------------------------------
//	@	Camera::GetBaseFrustum()
//--------------------------------------------------------------------------------
//		Returns a camera-space frustum
//--------------------------------------------------------------------------------
Frustum Camera::BaseFrustum() const
{
	Frustum temp;
	temp.Build(Point4::origin, basis_base, near_clip_z, far_clip_z, wd2, hd2);
	return temp;

}	//End: Camera::GetBaseFrustum()


//--------------------------------------------------------------------------------
//	@	Camera::SetVQS()
//--------------------------------------------------------------------------------
//		Set orientation
//--------------------------------------------------------------------------------
void Camera::SetVQS(const VQS& vqs)
{
	//Set origin
	origin = Point4::origin;
	origin *= vqs;

	//Set basis
	basis = basis_base;
	basis.TransformSelfQuick(vqs);

}	//End: Entity_OLD::SetVQS()


//--------------------------------------------------------------------------------
//	@	Camera::UpdateVQS()
//--------------------------------------------------------------------------------
//		Update orientation
//--------------------------------------------------------------------------------
void Camera::UpdateVQS(const VQS& vqs)
{
	//update origin
	origin *= vqs;

	//update basis
	basis.TransformSelf(vqs);

}	//End: Entity_OLD::UpdateVQS()


//--------------------------------------------------------------------------------
//	@	Camera::SetViewport()
//--------------------------------------------------------------------------------
//		Set the vertical and horizontal fov 
//--------------------------------------------------------------------------------
void Camera::SetFOV(float hfov, float vfov)
{
	//Check input
	if (hfov < 0.0f)
		wd2 = 0.0f;
	if (vfov < 0.0f)
		hd2 = 0.0f;

	if (hfov > PI - EPSILON)
		hfov = PI - EPSILON;
	if (vfov > PI - EPSILON)
		vfov = PI - EPSILON;

	wd2 = DgTan(hfov / 2.0f);
	hd2 = DgTan(vfov / 2.0f);

}	//End: Camera::SetViewport()


//--------------------------------------------------------------------------------
//	@	Camera::SetNearFar()
//--------------------------------------------------------------------------------
//		Set the near and far clipping planes
//--------------------------------------------------------------------------------
void Camera::SetNearFar(float near, float far)
{
	near_clip_z = near;
	far_clip_z = far;

}	//End: Camera::SetVNearFar()


//--------------------------------------------------------------------------------
//	@	Camera::SetViewplane()
//--------------------------------------------------------------------------------
//		Set the dimentions of the viewplane. 
//--------------------------------------------------------------------------------
void Camera::SetViewplane(float _wd2, float _hd2)
{
	//Check input
	if (_wd2 < 0.0f)
		wd2 = 0.0f;
	else
		wd2 = _wd2;

	if (_hd2 < 0.0f)
		hd2 = 0.0f;
	else
		hd2 = _hd2;

}	//End: Camera::SetViewplane()


//--------------------------------------------------------------------------------
//	@	Camera::HFOV()
//--------------------------------------------------------------------------------
//		Return the horizontal field of view
//--------------------------------------------------------------------------------
float Camera::HFOV() const
{
	return DgATan(wd2) * 2.0f;

}	//End: Camera::HFOV()


//--------------------------------------------------------------------------------
//	@	Camera::VFOV()
//--------------------------------------------------------------------------------
//		Return the vertical field of view
//--------------------------------------------------------------------------------
float Camera::VFOV() const
{
	return DgATan(hd2) * 2.0f;

}	//End: Camera::VFOV()