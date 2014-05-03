#include "Systems.h"
#include "GameDatabase.h"
#include "ObjectController.h"
#include "DgTypes.h"

//--------------------------------------------------------------------------------
/*
		* Moves entity from some object controller
        * precondition: Components: MOVEMENT
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_CameraControl(GameDatabase& data, 
						  const ObjectController& controller, float dt)
{
	//Try to find camera
	int index;
	if (!data.Movements.find(data.player, index))
		return;

	Component_MOVEMENT& movement = data.Movements[index];

	//Set vector
	movement.direction = controller.Get_dp(dt);

	//Set angles
	controller.GetRPY(movement.roll, 
					  movement.pitch, 
					  movement.yaw, dt);
	

}	//End: SYSTEM_Move()



//--------------------------------------------------------------------------------
/*
		* Updates any info needed in the camera entity before rendering
        * precondition: Components: CAMERA, POSITION
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_CameraPost(GameDatabase& data)
{
	//Try to find components
	int pos = 0;
	for (int i = 0; i < data.Cameras.size(); ++i)
	{
		if (!data.Cameras[i].cameraSystem.IsActive())
			continue;

		if (!data.Positions.find(data.Cameras.ID(i), pos))
			continue;

		Component_CAMERA& camera = data.Cameras[i];
		Component_POSITION& position = data.Positions[pos];

		//Get inverse VQS
		camera.T_OBJ_WLD = position.T_WLD_OBJ;
		camera.T_OBJ_WLD.Inverse();

		//Set camera vqs data
		camera.cameraSystem.SetVQS(position.T_WLD_OBJ);
	}


}	//End: SYSTEM_Move()