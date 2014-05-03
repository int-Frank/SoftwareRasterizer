#include "Systems.h"
#include "GameDatabase.h"
#include "Clipper.h"
#include "Mesh.h"
#include "Texture.h"
#include "Matrix44.h"
#include "Viewport.h"

#include "Debugger.h"


//--------------------------------------------------------------------------------
//	@	ResetData(GameDatabase& data)
//--------------------------------------------------------------------------------
//		Resets all data after a draw
//--------------------------------------------------------------------------------
static void ResetData(GameDatabase& data)
{
	for (int li = 0; li < data.LightsAffecting.size(); ++li)
	{
		//Clear lights list
		data.LightsAffecting[li].pointlights.clear();
		data.LightsAffecting[li].spotlights.clear();
	}
}	//End: ResetData(GameDatabase& data)

/*!
 * Send all aspects through the pipeline
 *
 * @pre [pre condition]
 * @post [post condition]
 */
static void AddAspects(GameDatabase& data, uint32 clock_time, Component_CAMERA& camera)
{

	Point4 camera_origin(camera.cameraSystem.CameraOrigin());
	Viewport* camera_view = camera.cameraSystem.GetViewport();

	//--------------------------------------------------------------------------------
	//		Loop through all aspects
	//--------------------------------------------------------------------------------
	int asp_pi = 0;
	int asp_li = 0;
	for (int ai = 0; ai < data.Aspects.size(); ++ai)
	{
		//--------------------------------------------------------------------------------
		//		Check if object is outside frustum
		//--------------------------------------------------------------------------------
		if (data.Aspects[ai].intersects == Frustum::OUTSIDE)
			continue;

		const entityID asp_id = data.Aspects.ID(ai);

		//--------------------------------------------------------------------------------
		//		Try to find the aspect position component
		//--------------------------------------------------------------------------------
		if (!data.Positions.find(asp_id, asp_pi, asp_pi))
			continue;

		Component_ASPECT& aspect = data.Aspects[ai];
		Component_POSITION& aspect_position = data.Positions[asp_pi];


		//--------------------------------------------------------------------------------
		//		Find the World to aspect transform, T_OBJ_WLD
		//--------------------------------------------------------------------------------
		VQS vqs_temp(Inverse(aspect_position.T_WLD_OBJ));


		//--------------------------------------------------------------------------------
		//		Backcull polygons
		//--------------------------------------------------------------------------------

		//Only backcull if not colorkeyed
		if (!aspect.materials.IsDoubleSided())
		{
			//Transform camera position to Object BASE cordinates
			Point4 camera_origin_obj(vqs_temp * camera_origin);

			//Backcull
			aspect.mesh->BackCull(camera_origin_obj);
		}
		else
		{
			//Activate all vertices
			aspect.mesh->ActivateAll();
		}


		//--------------------------------------------------------------------------------
		//		Lighting calculations
		//--------------------------------------------------------------------------------

		if (aspect.materials.IsMasterOn())
		{
			if (aspect.materials.IsReflective() &&
				data.LightsAffecting.find(asp_id, asp_li, asp_li))
			{
				Component_LIGHTS_AFFECTING& affectinglights = data.LightsAffecting[asp_li];

				//Add ambient light
				data.ambientLight.AddToMesh(*aspect.mesh, VQS(), aspect.materials);

				//Add directional lights
				for (int32 i = 0; i < data.directionalLights.size(); ++i)
				{
					data.directionalLights[i].AddToMesh(*aspect.mesh, vqs_temp, aspect.materials);
				}

				//Add points lights
				int pli = 0;
				for (int32 i = 0; i < affectinglights.pointlights.size(); ++i)
				{
					//Find light
					if (!data.PointLights.find(affectinglights.pointlights[i], pli, pli))
						continue;

					data.PointLights[pli].light.current.AddToMesh(*aspect.mesh, vqs_temp, aspect.materials);
				}

				//Add spot lights
				int sli = 0;
				for (int32 i = 0; i < affectinglights.spotlights.size(); ++i)
				{
					//Find light
					if (!data.SpotLights.find(affectinglights.spotlights[i], sli, sli))
						continue;

					data.SpotLights[sli].light.current.AddToMesh(*aspect.mesh, vqs_temp, aspect.materials);
				}

			}

			//Adjust material lighting to each vertex in the object
			aspect.materials.AdjustMesh(*(aspect.mesh));

		}



		//--------------------------------------------------------------------------------
		//		Transform object to camera space
		//--------------------------------------------------------------------------------

		//Mesh to camera space transform
		//T: camera_object  =  T:camera_world * T:world_object
		vqs_temp = camera.T_OBJ_WLD * aspect_position.T_WLD_OBJ;

		//Transform active vertices in the object to camera space
		aspect.mesh->TransformActiveVertices(vqs_temp);



		//--------------------------------------------------------------------------------
		//		Send Polygons through to the viewport
		//--------------------------------------------------------------------------------

		const Mipmap *mm_temp(NULL);
		if (aspect.texture != NULL)
			mm_temp = aspect.texture->GetMipmap(clock_time);

		camera_view->AddObject(aspect.mesh,
			aspect.materials,
			mm_temp,
			aspect.intersects);



		//--------------------------------------------------------------------------------
		//		Finish up
		//--------------------------------------------------------------------------------

		//Reset base
		aspect.mesh->ResetStates();

	}
}


/*!
 * Send all particle emitters through the pipeline
 *
 * @pre [pre condition]
 * @post [post condition]
 */
static void AddParticleEmitters(GameDatabase& data, Viewport* view, VQS& T_OBJ_WLD)
{
	for (int i = 0; i < data.ParticleEmitters.size(); ++i)
	{
		data.ParticleEmitters[i].particleEmitter->Render(view, T_OBJ_WLD);
	}
}


//--------------------------------------------------------------------------------
/*
		Sends entity down the pipeline. Assumes culling has occured and clip 
		planes have been set in the clipper.
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_Add_Entities(GameDatabase& data, uint32 clock_time)
{
	//Draw for each camera
	int cam_pi = 0;
	for (int ci = 0; ci < data.Cameras.size(); ++ci)
	{
		entityID cam_id = data.Cameras.ID(ci);

		//--------------------------------------------------------------------------------
		//		Check camera is valid
		//--------------------------------------------------------------------------------
		if (!data.Cameras[ci].cameraSystem.IsActive() ||
			!data.Positions.find(cam_id, cam_pi, cam_pi))
			continue;


		//--------------------------------------------------------------------------------
		//		Test all Aspects against the camera
		//--------------------------------------------------------------------------------
		SYSTEM_FrustumCull(data, cam_id);


		//--------------------------------------------------------------------------------
		//		Process all aspects
		//--------------------------------------------------------------------------------
		AddAspects(data, clock_time, data.Cameras[ci]);


		//--------------------------------------------------------------------------------
		//		Process all particle emitters
		//--------------------------------------------------------------------------------
		AddParticleEmitters(data, data.Cameras[ci].cameraSystem.GetViewport(),
			data.Cameras[ci].T_OBJ_WLD);
	}

	//After all rendering is done, reset data
	ResetData(data);

}