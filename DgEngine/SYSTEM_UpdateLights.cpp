#include "Systems.h"
#include "GameDatabase.h"
#include "DgTypes.h"

//--------------------------------------------------------------------------------
/*
		* Moves an entity's lights
        * postcondition: Transforms lights to new vqs
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_UpdateLights(GameDatabase& data)
{
	int index = 0;
	for (int i = 0; i < data.PointLights.size(); ++i)
	{
		//Find corresponding position 
		if (data.Positions.find(data.PointLights.ID(i), index, index))
		{
			//Transform the lights
			data.PointLights[i].light.TransformQuick(data.Positions[index].T_WLD_OBJ);
		}
	}

	index = 0;
	for (int i = 0; i < data.SpotLights.size(); ++i)
	{
		//Find corresponding position 
		if (data.Positions.find(data.SpotLights.ID(i), index, index))
		{
			//Transform the lights
			data.SpotLights[i].light.TransformQuick(data.Positions[index].T_WLD_OBJ);
		}
	}
}	//End: SYSTEM_Move()