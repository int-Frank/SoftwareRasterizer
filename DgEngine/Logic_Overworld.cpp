#include "Overworld.h"
#include "MasterPList.h"
#include "DgTypes.h"
#include "Component_MOVEMENT.h"
#include "Systems.h"

//--------------------------------------------------------------------------------
//		Logic
//--------------------------------------------------------------------------------
void Overworld::Logic()
{
   // while (timer.Time() % 200) {  }

    //Get dt
    dt = timer.Lap();

	float dtf = float(dt) / 1000.0f;

	//Assign/Deassign cameras
	//TODO Remove this. Deal with this as requests are made.
	//No need to check every frame. Or perhaps this can be some
	//general event handler system.
	SYSTEM_AssignViewports(gameData, viewport_events);

	//Update the player movement
	SYSTEM_CameraControl(gameData, cameraControls, dtf);

	//Update data
	SYSTEM_Move(gameData, dtf);
	SYSTEM_UpdatePositionHierarchies(gameData);
	SYSTEM_UpdatePhysics(gameData);
	SYSTEM_UpdateLights(gameData);
	SYSTEM_AddLights(gameData);
	SYSTEM_UpdateParticleEmitters(gameData, dtf);

	//This is the logic ^
	//Should this below be in Overworld::Render()?

	//Update cameras
	SYSTEM_CameraPost(gameData);

	//Add objects to the render lists
	SYSTEM_Add_Entities(gameData, timer.Time());
	SYSTEM_Add_Skyboxes(gameData);

	//Render the render lists
	SYSTEM_Render(gameData);

}	//End: Overworld::Logic()