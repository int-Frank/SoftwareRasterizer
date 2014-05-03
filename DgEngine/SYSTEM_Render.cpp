#include "Systems.h"
#include "GameDatabase.h"
#include "Clipper.h"
#include "Mesh.h"
#include "Texture.h"
#include "Matrix44.h"
#include "Viewport.h"


//--------------------------------------------------------------------------------
/*
Sends entity down the pipeline. Assumes culling has occured and clip
planes have been set in the clipper.
*/
//--------------------------------------------------------------------------------
void SYSTEM_Render(GameDatabase& data)
{
	//Draw for each camera
	int cam_pi = 0;
	for (int ci = 0; ci < data.Cameras.size(); ++ci)
	{
		entityID cam_id = data.Cameras.ID(ci);

		//--------------------------------------------------------------------------------
		//		Render active cameras
		//--------------------------------------------------------------------------------
		if (data.Cameras[ci].cameraSystem.IsActive())
			data.Cameras[ci].cameraSystem.Render();

		
	}


}