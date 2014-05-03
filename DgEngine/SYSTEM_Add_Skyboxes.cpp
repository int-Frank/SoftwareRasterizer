#include "Systems.h"
#include "GameDatabase.h"
#include "Viewport.h"


//--------------------------------------------------------------------------------
/*
Sends entity down the pipeline. Assumes culling has occured and clip
planes have been set in the clipper.
*/
//--------------------------------------------------------------------------------
void SYSTEM_Add_Skyboxes(GameDatabase& data)
{
	//add current skybox to each camera
	for (int cam_ind = 0; cam_ind < data.Cameras.size(); ++cam_ind)
	{
		if (!data.Cameras[cam_ind].cameraSystem.IsActive())
			continue;

		//Process skybox
		data.skybox.OrientateCubeToCamera(data.Cameras[cam_ind].T_OBJ_WLD.Q());
		data.skybox.SendToRenderer(data.Cameras[cam_ind].cameraSystem.GetViewport());
	}
	
}