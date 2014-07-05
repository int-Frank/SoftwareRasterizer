#include "GameDatabase.h"
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
    if (parent == ENTITYID::ROOT)
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
        std::cerr << "GameDatabase::AddMeta() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddPosition() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddMovement() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddBV() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddPointLight() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddSpotLight() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddLightsAffecting() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddAspect() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddCamera() -> Failed to insert component." << std::endl;
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
		std::cerr << "GameDatabase::AddParticleEmitter() -> Failed to insert component." << std::endl;
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
entityID GameDatabase::LoadClass(uint32 classID, entityID parent)
{
    //Need to convert classID to string to find in the DOM
    DgString ss;
    ss << classID;
    std::string classID_str = ss.str();

    //Find the class in the class document
    pugi::xml_node root = classDocument.document_element();
    pugi::xml_node classNode = root.find_child_by_attribute(
        "class", "id", classID_str.c_str());

    //Does the class exist?
    if (!classNode)
    {
        std::cerr << "@GameDatabase::LoadEntity() -> Class does not exist: " << classID_str << std::endl;
        return ENTITYID::E_INV_CLASS;
    }

    //Build the combined id.
    SimpleRNG rng;
    uint32 instanceID = rng.GetUint(0, 0xFFFF);
    entityID thisID = GetID(classID, instanceID);

    //Does the id already exist in the database?
    int32 ind;
    if (EntityIDs.find(thisID, ind))
    {
        bool success = false;
        for (uint32 i = 0; i < 0xFFFF; ++i)
        {
            //Adjust id
            instanceID += (65535 + (i & 1) * 2);
            instanceID &= 0xFFFF;
            thisID = GetID(classID, instanceID);

            if (!EntityIDs.find(thisID, ind))
            {
                success = true;
                break;
            }
        }
        if (!success)
        {
            std::cerr << "@GameDatabase::LoadEntity() -> All instance IDs for class "  <<
                classID_str << " taken." << std::endl;
            return ENTITYID::E_INS_FULL;
        }
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
            AddPosition(obj, thisID);
        }
        else if (tag == "meta")
        {
            Component_META obj;
            *it >> obj;
            AddMeta(obj, thisID);
        }
        else if (tag == "movement")
        {
            Component_MOVEMENT obj;
            *it >> obj;
            AddMovement(obj, thisID);
        }
		else if (tag == "lights_affecting")
		{
			Component_LIGHTS_AFFECTING obj;
			*it >> obj;
            AddLightsAffecting(obj, thisID);
		}
		else if (tag == "pointlight")
		{
			Component_POINTLIGHT obj;
			*it >> obj;
            AddPointLight(obj, thisID);
		}
		else if (tag == "spotlight")
		{
			Component_SPOTLIGHT obj;
			*it >> obj;
            AddSpotLight(obj, thisID);
		}
		else if (tag == "physics")
		{
			Component_PHYSICS obj;
			*it >> obj;
            AddPhysics(obj, thisID);
		}
		else if (tag == "aspect")
		{
			Component_ASPECT obj;
			*it >> obj;
            AddAspect(obj, thisID);
		}
		else if (tag == "camera")
		{
			Component_CAMERA obj;
			*it >> obj;
            AddCamera(obj, thisID);
		}
		else if (tag == "particle_emitter")
		{
			Component_PARTICLEEMITTER obj;
			*it >> obj;
            AddParticleEmitter(obj, thisID);
		}
		else if (tag == "class")
		{
			uint32 childClass;
            if (!GetIDValue(*it, childClass))
            {
                return ENTITYID::E_INV_CLASS;
            }

            entityID result = LoadClass(childClass, thisID);
            if (!IsIDOK(result))
            {
                //Clean up 
                RemoveEntity(thisID);
                return result;
            }

		}
	}

    return thisID;
}


//--------------------------------------------------------------------------------
//		Builds the database from a xml file
//--------------------------------------------------------------------------------
bool GameDatabase::LoadDataFile(const std::string& file)
{
    //Does class file conform to schema?
    if (!DBValidator->ValidateXML(file))
    {
        return false;
    }

    //Load file
    pugi::xml_document dbDoc;
    pugi::xml_parse_result result = dbDoc.load_file(file.c_str());

    //Make sure the data file opened
    if (!result)
    {
        std::cerr << "@GameDtabase::LoadDataFile() -> Failed to open file: " << file << std::endl;
        return false;
    }

    //Get the root node
    pugi::xml_node root = dbDoc.document_element();

    //Get the class file
    pugi::xml_node classFile = root.child("classFile");
    if (!classFile)
    {
        std::cerr << "@GameDtabase::LoadDataFile() -> No classFile element: " << file << std::endl;
        return false;
    }

    //Try to load the class file
    if (!LoadClassFile(classFile.child_value()))
    {
        std::cerr << "@GameDtabase::LoadDataFile() -> Failed to load class file: " << file << std::endl;
        return false;
    }

    //iterate through all nodes
    for (pugi::xml_node_iterator it = root.begin(); it != root.end(); ++it)
    {
        //Get the name of the node
        std::string tag = it->name();

        if (tag == "entity")
        {
            if (!LoadEntity(*it))
            {
                std::cerr << "@GameDtabase::LoadDataFile() -> Error while loading entity." <<
                    std::endl;
            }
        }
        else if (tag == "skybox")
        {
            *it >> skybox;
        }
        else if (tag == "ambientLight")
        {
            *it >> ambientLight;
        }
        else if (tag == "directionalLight")
        {
            uint32 thisID;
            if (!GetIDValue(*it, thisID))
            {
                std::cerr << "@GameDtabase::LoadDataFile() -> Error while loading directional light id." <<
                    std::endl;
            }
            
            DirectionalLight temp;
            *it >> temp;
            directionalLights.insert(temp, thisID);
        }
    }

}	//End: GameDatabase::Build()


//--------------------------------------------------------------------------------
//		Get component
//--------------------------------------------------------------------------------
bool GameDatabase::LoadEntity(pugi::xml_node& node)
{
    //Get class ID
    uint32 classID;
    if (!GetIDValue(node, classID))
    {
        return false;
    }

	//Add in the default class template
    entityID thisID = LoadClass(classID, ENTITYID::ROOT);
    if (!IsIDOK(thisID))
    {
        return false;
    }

    //Overwrite components from the input
    if (!AmmendEntity(node, thisID))
    {
        RemoveEntity(thisID);
        return false;
    }

	return true;
}	//End: void operator>>(pugi::xml__node_iterator& doc, GameDatabase& dest)


//--------------------------------------------------------------------------------
//		@GameDtabase::LoadClassFile()
//--------------------------------------------------------------------------------
bool GameDatabase::LoadClassFile(const std::string& file)
{
    //Is this file already loaded?
    if (file == currentClassDoc)
    {
        return true;
    }

    //Does class file conform to the schema?
    if (!classValidator->ValidateXML(file))
    {
        return false;
    }

    //Load class file
    classDocument.reset();
    pugi::xml_parse_result result = classDocument.load_file(file.c_str());

    //Make sure it opened
    if (!result)
    {
        std::cerr << "@GameDtabase::LoadClassFile() -> Failed to open file: " << file << std::endl;
        return false;
    }

    //Get the root node
    pugi::xml_node root = classDocument.document_element();

    //iterate through all nodes
    for (pugi::xml_node_iterator it = root.begin(); it != root.end(); ++it)
    {
        //Get the name of the node
        std::string tag = it->name();

        if (tag == "class")
        {
            //Get the class ID
            uint32 thisID;
            if (!GetIDValue(*it, thisID))
            {
                return false;
            }

            //Ensure Class structure is valid
            if (!IsIDValid(*it, thisID))
            {
                return false;
            }
        }
    }

    //Set the new class doc name
    currentClassDoc = file;

    return true;
}


//--------------------------------------------------------------------------------
//		@GameDtabase::IsIDValid()
//--------------------------------------------------------------------------------
bool GameDatabase::IsIDValid(pugi::xml_node& parent, uint32 thatID)
{
    //Must have a position component if has children
    bool hasPosition = false;
    if (parent.child("position"))
    {
        hasPosition = true;
    }

    //iterate through all child nodes
    for (pugi::xml_node_iterator it = parent.begin(); it != parent.end(); ++it)
    {
        //Get the name of the node
        std::string tag = it->name();

        if (tag == "class")
        {
            uint32 thisID;
            if (!hasPosition || !GetIDValue(*it, thisID))
            {
                return false;
            }

            if ((thatID ==  thisID) ||          //Check this ID against input ID
                (!IsIDValid(*it, thatID)) ||    //Check all children against input ID
                (!IsIDInRoot(thisID)) ||        //Check this ID is in the root
                (!IsIDValid(*it, thisID)))      //Check all children against this ID
            {
                return false;
            }
        }
    }
    return true;
}


//--------------------------------------------------------------------------------
//		@GameDtabase::IsIDInRoot()
//--------------------------------------------------------------------------------
bool GameDatabase::IsIDInRoot(uint32 thatID)
{
    //Get the root node
    pugi::xml_node root = classDocument.document_element();

    //iterate through all nodes
    for (pugi::xml_node_iterator it = root.begin(); it != root.end(); ++it)
    {
        //Get the name of the node
        std::string tag = it->name();

        if (tag == "class")
        {
            //Read the class id.
            uint32 thisID;
            if (!GetIDValue(*it, thisID))
            {
                return false;
            }

            if (thisID == thatID)      //Check all children against this ID
            {
                return true;
            }
        }
    }
    return false;
}


//--------------------------------------------------------------------------------
//		@GameDtabase::IsIDInRoot()
//--------------------------------------------------------------------------------
bool GameDatabase::GetIDValue(pugi::xml_node& node, uint32& out)
{
    //Get the class id
    pugi::xml_attribute classAtt = node.attribute("id");

    if (!classAtt)
    {
        std::cerr << "@GameDtabase::GetIDValue() -> No class ID." << std::endl;
        return false;
    }

    //Read the class id.
    if (!StringToNumber(out, classAtt.value(), std::hex))
    {
        std::cerr << "@GameDtabase::GetIDValue() -> Failed to read class ID." << std::endl;
        return false;
    }
    return true;
}


//--------------------------------------------------------------------------------
//		@GameDtabase::GlobalInit
//--------------------------------------------------------------------------------
bool GameDatabase::GlobalInit()
{
    std::string classFile = "class_format.xsd";
    std::string dbFile = "gamedb_format.xsd";
    classValidator = new XMLValidator();
    DBValidator = new XMLValidator();
    return (classValidator->SetSchema(classFile) &&
            DBValidator->SetSchema(dbFile));
}


//--------------------------------------------------------------------------------
//		@GameDtabase::LoadClassFile
//--------------------------------------------------------------------------------
void GameDatabase::GlobalShutDown()
{
    delete classValidator;
    delete DBValidator;
    classValidator = NULL;
    DBValidator = NULL;
}