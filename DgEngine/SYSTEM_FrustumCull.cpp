#include "Systems.h"
#include "GameDatabase.h"

//--------------------------------------------------------------------------------
/*
		Test a Bounding Volume against a frustum.
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_FrustumCull(GameDatabase& data, entityID camera_id)
{
	//Find camera
	int ci;
	if (!data.Cameras.find(camera_id, ci))
		return;

	//Test aspects
	for (int  i = 0; i < data.Aspects.size(); ++i)
	{
		data.Aspects[i].intersects = TestFrustumSphere(
			data.Cameras[ci].cameraSystem.GetFrustum(), 
			data.Aspects[i].sphere.current);
	}

	//Test particles
	for (int i = 0; i < data.ParticleEmitters.size(); ++i)
	{
		data.ParticleEmitters[i].particleEmitter->
			FrustumCull(data.Cameras[ci].cameraSystem.GetFrustum());
	}

}