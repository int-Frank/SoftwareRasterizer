#include "Systems.h"
#include "VQS.h"
#include "GameDatabase.h"
#include "DgTypes.h"

//--------------------------------------------------------------------------------
/*
		* Updates BV
        * postcondition: Generates new BV->current
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_UpdatePhysics(GameDatabase& data)
{
	//Update Physics bounding volumes
	int index = 0;
	for (int i = 0; i < data.Physics.size(); ++i)
	{
		//Extract current bv object reference
		Component_PHYSICS& cur_bv = data.Physics[i];

		if (!data.Positions.find(data.Physics.ID(i), index, index))
			continue;
		
		//Extract current position object reference
		Component_POSITION& cur_pos = data.Positions[index];

		cur_bv.box.TransformQuick(cur_pos.T_WLD_OBJ);
		cur_bv.sphere.TransformQuick(cur_pos.T_WLD_OBJ);
	}
	
	//Update Aspect bounding volumes
	index = 0;
	for (int i = 0; i < data.Aspects.size(); ++i)
	{
		//Extract current bv object reference
		Component_ASPECT& aspect = data.Aspects[i];

		if (!data.Positions.find(data.Aspects.ID(i), index, index))
			continue;

		//Extract current position object reference
		Component_POSITION& cur_pos = data.Positions[index];

		aspect.box.TransformQuick(cur_pos.T_WLD_OBJ);
		aspect.sphere.TransformQuick(cur_pos.T_WLD_OBJ);
	}
}