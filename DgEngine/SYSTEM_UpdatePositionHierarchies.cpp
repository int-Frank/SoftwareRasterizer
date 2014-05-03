#include "Systems.h"
#include "VQS.h"
#include "GameDatabase.h"
#include "Dg_io.h"
#include "DgTypes.h"

static void Update(Component_POSITION& pos, DgMap<entityID, Component_POSITION>& data)
{
	//Update all children 
	int index = 0;
	for (int i = 0; i < pos.children.size(); ++i)
	{
		//Find child
		if (!data.find(pos.children[i], index, index))
			continue;

		//Update vqs
		data[index].T_WLD_OBJ = pos.T_WLD_OBJ * data[index].T_PAR_OBJ;

		//Call Update on this child
		Update(data[index], data);
	}

	//Call Update on all children
}

//--------------------------------------------------------------------------------
/*
		* Updates position hierarchies
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_UpdatePositionHierarchies(GameDatabase& data)
{
	for (int i = 0; i < data.Positions.size(); ++i)
	{
		//Extract current position object reference
		Component_POSITION& cur_pos = data.Positions[i];

		//If this is a root node
		if ( cur_pos.parent == ROOT_ID )
		{
			//Update world vqs
			cur_pos.T_WLD_OBJ = cur_pos.T_PAR_OBJ;

			//If it has children
			if ( cur_pos.children.size() > 0  )
			{
				//Begin update chain, starting at the root
				Update(cur_pos, data.Positions);
			}
		}
	}
	
}	//End: SYSTEM_Move()