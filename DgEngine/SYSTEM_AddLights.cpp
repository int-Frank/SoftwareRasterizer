#include "Systems.h"
#include "GameDatabase.h"
#include "DgTypes.h"
#include "Light.h"

//--------------------------------------------------------------------------------
/*
		Tests all lights in the input array against the entity.
		Maybe restrict testing to only those entities in the frustum.
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_AddLights(GameDatabase& data)
{
	int ai = 0;
	for (int li = 0; li < data.LightsAffecting.size(); ++li)
	{
		//Clear current list
		Component_LIGHTS_AFFECTING& lightsAffecting = data.LightsAffecting[li];
		lightsAffecting.pointlights.clear();
		lightsAffecting.spotlights.clear();

		//find BV
		if (!data.Aspects.find(data.LightsAffecting.ID(li), ai, ai))
			continue;

		Component_ASPECT& aspect = data.Aspects[ai];

		//For all pointlights in the database
		for (int pi = 0; pi < data.PointLights.size(); ++pi)
		{
			if (data.PointLights[pi].light.current.Test(aspect.sphere.current) == 1)
				lightsAffecting.pointlights.insert(data.PointLights.ID(pi));
		}

		//For all spotlights in the database
		for (int si = 0; si < data.SpotLights.size(); ++si)
		{
			if (data.SpotLights[si].light.current.Test(aspect.sphere.current) == 1)
				lightsAffecting.spotlights.insert(data.SpotLights.ID(si));
		}

	}

}	//End: SYSTEM_Move()