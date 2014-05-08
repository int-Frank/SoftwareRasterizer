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
    int mov_ind = 0;
	for (int i = 0; i < data.ParticleEmitters.size(); ++i)
	{
        entityID id = data.ParticleEmitters.ID(i);
		//Try to find corresponding position
		if (!data.Positions.find(id, index, index))
			continue;

		if (data.ParticleEmitters[i].particleEmitter == NULL)
			continue;

		if (!data.ParticleEmitters[i].particleEmitter->IsActive())
			continue;

		//Set the VQS
		data.ParticleEmitters[i].particleEmitter->SetVQS(data.Positions[index].T_WLD_OBJ);

        /// Try to find global velocity
        Vector4 mov(0.0f, 0.0f, 0.0f);
        if (GetWorldSpaceVelocity(data, mov, id))
        {
            data.ParticleEmitters[i].particleEmitter->SetGlobalVelocity(mov);
        }

		////Update particles
		data.ParticleEmitters[i].particleEmitter->Update(dt);
	}
}