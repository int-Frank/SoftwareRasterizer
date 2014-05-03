#include "Systems.h"
#include "VQS.h"
#include "GameDatabase.h"
#include "DgTypes.h"

//--------------------------------------------------------------------------------
/*
		* Moves an entity
        * precondition: Components: MOVEMENT, POSITION
        * postcondition: Adds Movement elements to Position
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_Move(GameDatabase& data, float dt)
{
	int index = 0;
	for (int i = 0; i < data.Movements.size(); ++i)
	{
		//Extract current movement object reference
		Component_MOVEMENT& cur_mov = data.Movements[i];

		//Try to find corresponding position
		if ( !data.Positions.find(data.Movements.ID(i), index, index) )
			continue;

		//Set quaternion
		Quaternion q;
		q.Set( cur_mov.pitch * dt, 
			   cur_mov.yaw * dt,
			   cur_mov.roll * dt);

		//Set new position
		data.Positions[index].T_PAR_OBJ *= VQS(cur_mov.direction * dt, q, 1.0f);
	}
	
}	//End: SYSTEM_Move()