#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <string>

#include "DgMap.h"
#include "DgOrderedArray.h"
#include "DgTypes.h"
#include "XMLValidator.h"

#include "Component_Meta.h"
#include "Component_Aspect.h"
#include "Component_Physics.h"
#include "Component_Camera.h"
#include "Component_Lights_Affecting.h"
#include "Component_PointLight.h"
#include "Component_SpotLight.h"
#include "Component_Movement.h"
#include "Component_Position.h"
#include "Component_ParticleEmitter.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "Skybox.h"
#include "pugixml.hpp"

#define GetID(classID, instanceID) entityID((classID << 16) | instanceID)

/*!
 * @ingroup entity_component
 *
 * @class GameDatabase
 *
 * @brief A database to store all objects and level data.
 *
 * Entity IDs are constructed like so:
 * 00000000   00000000   00000000   00000000
 *       class id      |    instance id
 * Child IDs must NOT be 00; this is parent ID.
 * 
 * @author Frank Hart
 * @date 2/01/2014
 */
class GameDatabase
{
public:

	//Constructor / destructor
	GameDatabase() {}
	~GameDatabase() {}

    //! @brief Sets class schema
    static bool GlobalInit();
    static void GlobalShutDown();

	//--------------------------------------------------------------------------------
	//		Read from xml_node
	//--------------------------------------------------------------------------------
	//friend void operator>>(pugi::xml_node&, GameDatabase&);
    //friend bool LoadEntity(pugi::xml_node_iterator&, GameDatabase&);
    //friend bool LoadDirectionalLight(pugi::xml_node_iterator&, GameDatabase&);
    //friend entityID ParseClassNode(uint32 classID, entityID parent, GameDatabase& dest);

    //! @Load class template file
    bool LoadClassFile(const std::string&);

    //! @Load map file
    bool LoadDataFile(const std::string&);

	//Remove all components with this entityID
	void RemoveEntity(entityID);

	void SetID(const std::string& str) {id = str;}
	std::string ID() const {return id;}

	//Adding items into the lists should be done through these functions
    bool AddMeta(const Component_META&, entityID);
    bool AddPosition(const Component_POSITION&, entityID);
	bool AddPhysics(const Component_PHYSICS&, entityID);
	bool AddPointLight(const Component_POINTLIGHT&, entityID);
	bool AddSpotLight(const Component_SPOTLIGHT&, entityID);
	bool AddLightsAffecting(const Component_LIGHTS_AFFECTING&, entityID);
	bool AddMovement(const Component_MOVEMENT&, entityID);
	bool AddAspect(const Component_ASPECT&, entityID);
	bool AddCamera(const Component_CAMERA&, entityID);
	bool AddParticleEmitter(const Component_PARTICLEEMITTER&, entityID);

	bool SetSkybox(const Skybox& s) { skybox = s; }


public:
	//--------------------------------------------------------------------------------
	//		Data Lists. Adding/removing entities should be done through
	//		the interface.
	//--------------------------------------------------------------------------------
	DgOrderedArray<entityID>						EntityIDs;
    DgMap<entityID, Component_META>                 Metas;
	DgMap<entityID, Component_POSITION>				Positions;
	DgMap<entityID, Component_MOVEMENT>				Movements;
	DgMap<entityID, Component_PHYSICS>				Physics;
	DgMap<entityID, Component_LIGHTS_AFFECTING>		LightsAffecting;
	DgMap<entityID, Component_POINTLIGHT>			PointLights;
	DgMap<entityID, Component_SPOTLIGHT>			SpotLights;
	DgMap<entityID, Component_ASPECT>				Aspects;
	DgMap<entityID, Component_CAMERA>				Cameras;
	DgMap<entityID, Component_PARTICLEEMITTER>		ParticleEmitters;

	//--------------------------------------------------------------------------------
	//		Other level data
	//--------------------------------------------------------------------------------
	entityID							player;
	Skybox								skybox;
	AmbientLight						ambientLight;
	DgMap<entityID, DirectionalLight>	directionalLights;


private:    //Data

	std::string id;
	pugi::xml_document classDocument;

private:    //Functions

    // Functions that help validate class files.
    bool IsIDValid(pugi::xml_node& parent, uint32 id);
    bool IsIDInRoot(uint32);
    bool GetIDValue(pugi::xml_node&, uint32&);

private:    //Statics

    static XMLValidator *classValidator;
    static XMLValidator *DBValidator;
};

inline bool IsIDOK(entityID id)
{
    return (id & ENTITYID::CLASS_MASK) > ENTITYID::CUTOFF;
}

// Find the global velocity vector from a child in the family tree.
// Note: The family heirarcy of positions must be established before this
//       function should be called.
bool GetWorldSpaceVelocity(GameDatabase&, Vector4&, entityID);

#endif