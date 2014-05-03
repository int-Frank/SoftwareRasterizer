#include "Systems.h"
#include "VQS.h"
#include "GameDatabase.h"
#include "Dg_io.h"
#include "DgTypes.h"


/*!
 * Updates particle emitters in the database
 *
 * @pre [pre condition]
 * @post [post condition]
 */
void SYSTEM_UpdateParticleEmitters(GameDatabase& data, float dt)
{
	//add current skybox to each camera
	int index = 0;
	for (int i = 0; i < data.ParticleEmitters.size(); ++i)
	{
		//Try to find corresponding position
		if (!data.Positions.find(data.ParticleEmitters.ID(i), index, index))
			continue;

		if (data.ParticleEmitters[i].particleEmitter == NULL)
			continue;

		if (!data.ParticleEmitters[i].particleEmitter->IsActive())
			continue;

		//Set the VQS
		data.ParticleEmitters[i].particleEmitter->SetVQS(data.Positions[index].T_WLD_OBJ);

		////Update particles
		data.ParticleEmitters[i].particleEmitter->Update(dt);
	}
}