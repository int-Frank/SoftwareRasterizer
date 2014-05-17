#include "GameDatabase.h"
#include "DgError.h"
#include "Dg_io.h"
#include "SimpleRNG.h"

//--------------------------------------------------------------------------------
//		@GetWorldSpaceVelocity()
//--------------------------------------------------------------------------------
bool GetWorldSpaceVelocity(GameDatabase& data, Vector4& output, entityID id)
{
    //Try to find position component
    int pos = 0;
    if (!data.Positions.find(id, pos))
        return false;

    //Try to find movement component
    int mov = 0;
    if (data.Movements.find(id, mov))
    {
        //Add the world space velocity vector to the output.
        output += (data.Positions[pos].T_WLD_OBJ * data.Movements[mov].direction);
    }

    entityID parent = data.Positions[pos].parent;

    //If we've reached the root, stop the chain.
    if (parent == ENTITYID_ROOT)
    {
        return true;
    }

    //Add in the parents velocity
    return GetWorldSpaceVelocity(data, output, parent);
}


//--------------------------------------------------------------------------------
//		Completely removes all entity components and children from the database
//--------------------------------------------------------------------------------
void GameDatabase::RemoveEntity(entityID id)
{
    //Erase all children
    int ind = 0;
    if (Positions.find(id, ind))
    {
        int ind1 = 0;
        for (int i = 0; i < Positions[ind].children.size(); ++i)
        {
            //Find child
            if (Positions.find(Positions[ind].children[i], ind1, ind1))
            {
                //Erase this child
                RemoveEntity(Positions.ID(ind1));
            }
        }
    }

	EntityIDs.erase(id);
    Metas.erase_c(id);
	Positions.erase_c(id);
	Movements.erase_c(id);
	Physics.erase_c(id);
	LightsAffecting.erase_c(id);
	PointLights.erase_c(id);
	SpotLights.erase_c(id);
	Aspects.erase_c(id);
	Cameras.erase_c(id);
	ParticleEmitters.erase_c(id);

}	//End: GameDatabase::RemoveEntity()


//--------------------------------------------------------------------------------
//		Adds a meta data component
//--------------------------------------------------------------------------------
bool GameDatabase::AddMeta(const Component_META& obj, entityID id)
{
    //Try to find the object.
    int32 ref;
    if (Metas.find(id, ref))
    {
        Metas[ref] = obj;
        return true;
    }

    //Object not foud, try to insert object
    if (!Metas.insert(obj, id))
    {
        ERROR_OUT("GameDatabase::AddMeta() -> Failed to insert component.");
        return false;
    }

    //Add Entity id if need be
    int index;
    if (!EntityIDs.find(id, index))
        EntityIDs.insert(id);

    //All good
    return true;
}	//End: GameDatabase::AddMeta()


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
entityID ParseClassNode(uint32 classID, entityID parent, GameDatabase& dest)
{
    //Need to convert classID to string to find in the DOM
    DgString ss;
    ss << classID;
    std::string classID_str = ss.str();

    //Build the combined id.
    SimpleRNG rng;
    uint32 instanceID = rng.GetUint(0, 0xFFFF);
    entityID id = GetID(classID, instanceID);

    //Does the id already exist in the database?
    int32 ind;
    if (dest.EntityIDs.find(id, ind))
    {
        for (uint32 i = 0; i < 0xFFFF; ++i)
        {
            //Adjust id
            instanceID += (65535 + (i & 1) * 2);
            instanceID &= 0xFFFF;
            id = GetID(classID, instanceID);

            if (!dest.EntityIDs.find(id, ind))
            {
                break;
            }
        }
        ERROR_OUT("LoadEntity() -> All instance IDs for class " +
            classID_str + " taken.");
        return ENTITYID_ERROR;
    }

    //Find the class in the class document
    pugi::xml_node classNode = dest.entityClassesRoot.find_child_by_attribute(
        "class", "id", classID_str.c_str());

    //Does the class exist?
    if (!classNode)
    {
        ERROR_OUT("LoadEntity() -> Class doen not exist: " + classID_str);
        return ENTITYID_ERROR;
    }

	//Update any values from the input node.
	for (pugi::xml_node_iterator it = classNode.begin(); it != classNode.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "position")
		{
			Component_POSITION obj;
			Read(*it, obj, parent);
			dest.AddPosition(obj, id);
        }
        else if (tag == "meta")
        {
            Component_META obj;
            *it >> obj;
            dest.AddMeta(obj, id);
        }
        else if (tag == "movement")
        {
            Component_MOVEMENT obj;
            *it >> obj;
            dest.AddMovement(obj, id);
        }
		else if (tag == "lights_affecting")
		{
			Component_LIGHTS_AFFECTING obj;
			*it >> obj;
			dest.AddLightsAffecting(obj, id);
		}
		else if (tag == "pointlight")
		{
			Component_POINTLIGHT obj;
			*it >> obj;
			dest.AddPointLight(obj, id);
		}
		else if (tag == "spotlight")
		{
			Component_SPOTLIGHT obj;
			*it >> obj;
			dest.AddSpotLight(obj, id);
		}
		else if (tag == "physics")
		{
			Component_PHYSICS obj;
			*it >> obj;
			dest.AddPhysics(obj, id);
		}
		else if (tag == "aspect")
		{
			Component_ASPECT obj;
			*it >> obj;
			dest.AddAspect(obj, id);
		}
		else if (tag == "camera")
		{
			Component_CAMERA obj;
			*it >> obj;
			dest.AddCamera(obj, id);
		}
		else if (tag == "particle_emitter")
		{
			Component_PARTICLEEMITTER obj;
			*it >> obj;
			dest.AddParticleEmitter(obj, id);
		}
		else if (tag == "class")
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

            if ((childID == classID) || 
                !IsIDOK(ParseClassNode(childID, classID, dest)))
            {
                //Clean up 
                dest.RemoveEntity(id);
                return ENTITYID_ERROR;
            }
		}
	}

    return id;
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
	//Get the class id
	pugi::xml_attribute classAtt = node->attribute("class");

	if (!classAtt)
	{
		ERROR_OUT("LoadEntity() -> No class ID.");
		return false;
	}

	//Read the class id.
	uint32 classID;
	if (!StringToNumber(classID, classAtt.value(), std::hex))
	{
		ERROR_OUT("LoadEntity() -> Failed to read class ID.");
		return false;
	}

	//Add in the default class
    entityID id = ParseClassNode(classID, ENTITYID_ROOT, dest);
    if (!IsIDOK(id))
    {
        return false;
    }

    //Overwrite components from the input
    if (!ParseInstanceNode(*node, dest, id, ROOT_ID))
    {
        dest.RemoveEntity(id);
        return false;
    }

	return true;
}	//End: void operator>>(pugi::xml__node_iterator& doc, GameDatabase& dest)


