#include "GameDatabase.h"
#include "DgError.h"
#include "Dg_io.h"


//--------------------------------------------------------------------------------
//		Statics
//--------------------------------------------------------------------------------
static bool LoadDirectionalLight(pugi::xml_node_iterator&, GameDatabase&);
static bool LoadSkybox(pugi::xml_node&, GameDatabase&);

//--------------------------------------------------------------------------------
//		@GetWorldSpaceVelocity()
//--------------------------------------------------------------------------------
bool GetWorldSpaceVelocity(GameDatabase& data, Vector4& output, entityID id)
{
    //Try to find position component
    int pos = 0;
    if (!data.Positions.find(id, pos))
        return false;

    //Try to find movement position component
    int mov = 0;
    if (data.Movements.find(id, mov))
    {
        //Add the world space velocity vector to the output.
        output += (data.Positions[pos].T_WLD_OBJ * data.Movements[mov].direction);
    }

    entityID parent = data.Positions[pos].parent;

    //If we've reached the root, stop the chain.
    if (parent == ROOT_ID)
    {
        return true;
    }

    //Add in the parents velocity
    return GetWorldSpaceVelocity(data, output, parent);
}


//--------------------------------------------------------------------------------
//		Completely removes all entity components from the database
//--------------------------------------------------------------------------------
void GameDatabase::RemoveEntity(entityID id)
{
	EntityIDs.erase(id);
	Positions.erase(id);
	Movements.erase(id);
	Physics.erase(id);
	LightsAffecting.erase(id);
	PointLights.erase(id);
	SpotLights.erase(id);
	Aspects.erase(id);
	Cameras.erase(id);
	ParticleEmitters.erase(id);

}	//End: GameDatabase::RemoveEntity()


//--------------------------------------------------------------------------------
//		Adds a position component
//--------------------------------------------------------------------------------
bool GameDatabase::AddPosition(const Component_POSITION& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (Positions.find(id, ref))
	{
		Positions[ref] = obj;
		return true;
	}

	//Object not foud, try to insert object
	if (!Positions.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddPosition() -> Failed to insert component.");
		return false;
	}

	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddPosition()


//--------------------------------------------------------------------------------
//		Adds a Movement component
//--------------------------------------------------------------------------------
bool GameDatabase::AddMovement(const Component_MOVEMENT& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (Movements.find(id, ref))
	{
		Movements[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!Movements.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddMovement() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddMovement()


//--------------------------------------------------------------------------------
//		Adds a BV component
//--------------------------------------------------------------------------------
bool GameDatabase::AddPhysics(const Component_PHYSICS& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (Physics.find(id, ref))
	{
		Physics[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!Physics.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddBV() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddBV()


//--------------------------------------------------------------------------------
//		Adds a PointLight component
//--------------------------------------------------------------------------------
bool GameDatabase::AddPointLight(const Component_POINTLIGHT& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (PointLights.find(id, ref))
	{
		PointLights[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!PointLights.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddPointLight() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddPointLight()


//--------------------------------------------------------------------------------
//		Adds a SpotLight component
//--------------------------------------------------------------------------------
bool GameDatabase::AddSpotLight(const Component_SPOTLIGHT& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (SpotLights.find(id, ref))
	{
		SpotLights[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!SpotLights.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddSpotLight() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddSpotLight()


//--------------------------------------------------------------------------------
//		Adds a LightsAffecting component
//--------------------------------------------------------------------------------
bool GameDatabase::AddLightsAffecting(const Component_LIGHTS_AFFECTING& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (LightsAffecting.find(id, ref))
	{
		LightsAffecting[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!LightsAffecting.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddLightsAffecting() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddLightsAffecting()


//--------------------------------------------------------------------------------
//		Adds an Aspect component
//--------------------------------------------------------------------------------
bool GameDatabase::AddAspect(const Component_ASPECT& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (Aspects.find(id, ref))
	{
		Aspects[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!Aspects.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddAspect() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddAspect()


//--------------------------------------------------------------------------------
//		Adds a Camera component
//--------------------------------------------------------------------------------
bool GameDatabase::AddCamera(const Component_CAMERA& obj,  entityID id)
{
	//Try to find the object.
	int32 ref;
	if (Cameras.find(id, ref))
	{
		Cameras[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!Cameras.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddCamera() -> Failed to insert component.");
		return false;
	}
	
	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddCamera()


//--------------------------------------------------------------------------------
//		Adds a Camera component
//--------------------------------------------------------------------------------
bool GameDatabase::AddParticleEmitter(const Component_PARTICLEEMITTER& obj, entityID id)
{
	//Try to find the object.
	int32 ref;
	if (ParticleEmitters.find(id, ref))
	{
		ParticleEmitters[ref] = obj;
		return true;
	}

	//Try to insert object
	if (!ParticleEmitters.insert(obj, id))
	{
		ERROR_OUT("GameDatabase::AddParticleEmitter() -> Failed to insert component.");
		return false;
	}

	//Add Entity id if need be
	int index;
	if (!EntityIDs.find(id, index))
		EntityIDs.insert(id);

	//All good
	return true;
}	//End: GameDatabase::AddParticleEmitter()


//--------------------------------------------------------------------------------
//		Parse a node into the database.
//--------------------------------------------------------------------------------
void ParseNode(	const pugi::xml_node& node, GameDatabase& dest, 
				entityID id, entityID parent)
{
	//Update any values from the input node.
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "position")
		{
			//Activate component
			Component_POSITION obj;
			Read(*it, obj, parent);
			dest.AddPosition(obj, id);
		}
		else if (tag == "movement")
		{
			//Activate component
			Component_MOVEMENT obj;
			*it >> obj;
			dest.AddMovement(obj, id);
		}
		else if (tag == "lights_affecting")
		{
			//Activate component
			Component_LIGHTS_AFFECTING obj;
			*it >> obj;
			dest.AddLightsAffecting(obj, id);
		}
		else if (tag == "pointlight")
		{
			//Activate component
			Component_POINTLIGHT obj;
			*it >> obj;
			dest.AddPointLight(obj, id);
		}
		else if (tag == "spotlight")
		{
			//Activate component
			Component_SPOTLIGHT obj;
			*it >> obj;
			dest.AddSpotLight(obj, id);
		}
		else if (tag == "physics")
		{
			//Activate component
			Component_PHYSICS obj;
			*it >> obj;
			dest.AddPhysics(obj, id);
		}
		else if (tag == "aspect")
		{
			//Activate component
			Component_ASPECT obj;
			*it >> obj;
			dest.AddAspect(obj, id);
		}
		else if (tag == "camera")
		{
			//Activate component
			Component_CAMERA obj;
			*it >> obj;
			dest.AddCamera(obj, id);
		}
		else if (tag == "particle_emitter")
		{
			//Activate component
			Component_PARTICLEEMITTER obj;
			*it >> obj;
			dest.AddParticleEmitter(obj, id);
		}
		else if (tag == "player_controlled")
		{
			//Flag this entity as the player controlled
			dest.player = id;
		}
		else if (tag == "child")
		{
			//Get id
			pugi::xml_attribute idAtt = it->attribute("id");

			if (!idAtt)
			{
				ERROR_OUT("ParseNode() -> No child ID.");
				continue;
			}

			uint32 childID;
			if (!StringToNumber(childID, idAtt.value(), std::hex))
			{
				ERROR_OUT("ParseNode() -> Failed to read child ID.");
				continue;
			}

			//Get the child ID
			childID = (id | (childID << 16));

			//Parse the child
			ParseNode(*it, dest, childID, id);
		}
	}
}


//--------------------------------------------------------------------------------
//		Builds the database from a xml file
//--------------------------------------------------------------------------------
void operator>>(pugi::xml_node& tool, GameDatabase& dest)
{
	//Load the class document first
	dest.entityClasses.reset();
	pugi::xml_node classFileNode = tool.child("base_class_file");

	//Ensure base class node exists
	if (!classFileNode)
	{
		ERROR_OUT("operator>>(GameDatabase) -> No class file is defined.");
		return;
	}

	//Open file
	dest.entityClasses.reset();
	std::string classFile = classFileNode.child_value();
	pugi::xml_parse_result result = dest.entityClasses.load_file(classFile.c_str());

	//Make sure the class file has loaded
	if (!result)
	{
		std::string message = "@operator>>(pugi::xml_node&, GameDatabase&) -> Failed to open class file: "
			+ classFile;
		ERROR_OUT(message);
		return;
	}

	//Set the class file root node
	dest.entityClassesRoot = dest.entityClasses.document_element();

	//iterate through all attributes
    for (	pugi::xml_attribute_iterator ait = tool.attributes_begin(); 
			ait != tool.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "id")
		{
			dest.SetID(ait->as_string());
		}
    }

	//iterate through all nodes
	for (pugi::xml_node_iterator it = tool.begin(); it != tool.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

		if (tag == "entity")
		{
			if (!LoadEntity(it, dest))
				ERROR_OUT("operator>>(GameDatabase) -> Error while loading entity.");
		}
		else if (tag == "skybox")
		{
			*it >> dest.skybox;
		}
		else if (tag == "ambientlight")
		{
			*it >> dest.ambientLight;
		}
		else if (tag == "directional_light")
		{
			if (!LoadDirectionalLight(it, dest))
				ERROR_OUT("operator>>(GameDatabase) -> Error while loading ambient light.");
		}

    }

}	//End: GameDatabase::Build()


//--------------------------------------------------------------------------------
//		Load a directional light into the database
//--------------------------------------------------------------------------------
bool LoadDirectionalLight(pugi::xml_node_iterator& node, GameDatabase& dest)
{
	elementID id = 0;

	//iterate through all attributes
	for (pugi::xml_attribute_iterator ait = node->attributes_begin();
		ait != node->attributes_end(); ++ait)
	{
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "id")
		{
			//Read id
			uint32 attempt;
			if (StringToNumber(attempt, ait->as_string(), std::hex))
				id = entityID(attempt);

			//Check is unique
			int index;
			if (dest.directionalLights.find(id, index))
			{
				ERROR_OUT("LoadDirectionalLight() -> Duplicate id");
				return false;
			}
		}
	}

	//Load the light
	DirectionalLight temp;
	*node >> temp;

	//Add to the database.
	dest.directionalLights.insert(temp, id);

	return true;

}	//End: LoadDirectionalLight()


//--------------------------------------------------------------------------------
//		Get component
//--------------------------------------------------------------------------------
bool LoadEntity(pugi::xml_node_iterator& node, GameDatabase& dest)
{
	//Get the class and instance ids
	pugi::xml_attribute classAtt = node->attribute("class");
	pugi::xml_attribute idAtt = node->attribute("id");

	if (!classAtt || !idAtt)
	{
		ERROR_OUT("LoadEntity() -> Invalid ID names.");
		return false;
	}

	//Read the class id.
	uint32 classID;
	if (!StringToNumber(classID, classAtt.value(), std::hex))
	{
		ERROR_OUT("LoadEntity() -> Failed to read class ID.");
		return false;
	}

	//Read the instance id.
	uint32 idID;
	if (!StringToNumber(idID, idAtt.value(), std::hex))
	{
		ERROR_OUT("LoadEntity() -> Failed to read instance ID.");
		return false;
	}
	
	//Build the final id.
	entityID id = GetID(classID, idID);

	//Does the id already exist in the database?
	for (int32 i = 0; i < dest.EntityIDs.size(); ++i)
	{
		if ((dest.EntityIDs[i] & 0xFF00FFFF) == id)
		{
			ERROR_OUT("LoadEntity() -> Entity already exists.");
			return false;
		}
	}

	//Find the class in the class document
	pugi::xml_node classNode = dest.entityClassesRoot.find_child_by_attribute(
		"class", "id", classAtt.value());

	//Does the class exist?
	if (!classNode)
	{
		std::string missingClass = classAtt.value();
		ERROR_OUT("LoadEntity() -> Class doen not exist: " + missingClass);
		return false;
	}

	//Add in the default class
	ParseNode(classNode, dest, id, ROOT_ID);

	//Overwrite components from the input
	ParseNode(*node, dest, id, ROOT_ID);

	return true;
}	//End: void operator>>(pugi::xml__node_iterator& doc, GameDatabase& dest)


