#include "Systems.h"
#include "GameDatabase.h"
#include "DgTypes.h"

//--------------------------------------------------------------------------------
/*
		* Processes viewport events
		* precondition: Components: CAMERA
		* postcondition: Switches camera viewport
*/
//--------------------------------------------------------------------------------
void SYSTEM_AssignViewports(GameDatabase& data, DgArray<ViewportEvent>& events)
{
	for (int i = events.size() - 1; i >= 0; --i)
	{
		ViewportEvent e = events[i];

		//Try to find camera in data
		int cam_ind = 0;
		if (!data.Cameras.find(events[i].entity_id, cam_ind))
			continue;

		CameraSystem& cameraSystem = data.Cameras[cam_ind].cameraSystem;

		//Try to attach new viewport
		if (!cameraSystem.SetViewport(events[i].viewport_id))
			continue;

		//Try to activate
		if (events[i].active)
		{
			cameraSystem.Activate();
		}
		else
		{
			cameraSystem.Deactivate();
		}

	}

	//Clear event list
	events.clear();

}	//End: SYSTEM_AssignViewports()