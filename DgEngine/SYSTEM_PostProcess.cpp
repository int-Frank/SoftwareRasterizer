#include "Systems.h"
#include "GameDatabase.h"
#include "Dg_io.h"
#include "Mesh.h"

//--------------------------------------------------------------------------------
/*
		* Processes a freshly built entity
        * precondition: The entity has just been constructed from an xml file
        * postcondition: Tidies up the data, makes connections etc...
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_PostProcess(GameDatabase& data)
{
	int index = 0;

	//Set Physics from Aspects
	for (int i = 0; i < data.Aspects.size(); ++i)
	{
		//Get id
		entityID id = data.Aspects.ID(i);

		//Look for corresponding id in Physics
		if (!data.Physics.find(id, index, index))
			continue;

		Component_PHYSICS& bv = data.Physics[index];

		//Assign new geometry
		data.Aspects[i].mesh->SetBV(bv.sphere.base, 
									bv.box.base);
		bv.box.Update();
		bv.sphere.Update();

	}

	//Build VQS heirachy
	for (int i = 0; i < data.Positions.size(); ++i)
	{
		//If current position has no parent, continue
		if (data.Positions[i].parent == ENTITYID::ROOT)
			continue;

		//Get id current component
		entityID id = data.Positions.ID(i);

		//Try to find parent
		int par_ind = 0;
		if (!data.Positions.find(data.Positions[i].parent, par_ind))
		{
			std::cerr << "@SYSTEM_PostProcess() -> Parent " 
				<< std::hex << std::to_string(data.Positions[i].parent)
				<< " not found for id: " << std:: hex
				<< std::to_string(id) << std::endl;
			continue;
		}

		//Add child to parent
		data.Positions[par_ind].children.insert(id);

	}

	//Update all positions / orientations
	SYSTEM_UpdatePositionHierarchies(data);

	//Initiate all particle emitters
	int pos = 0;
	for (int i = 0; i < data.ParticleEmitters.size(); ++i)
	{
		//Try to find position component
		if (!data.Positions.find(data.ParticleEmitters.ID(i), pos, pos))
			continue;

		data.ParticleEmitters[i].particleEmitter->SetVQS(
			data.Positions[pos].T_WLD_OBJ);

		data.ParticleEmitters[i].particleEmitter->Initiate();
	}

}