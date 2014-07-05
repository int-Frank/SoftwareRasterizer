#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <string>

#include "dg_map.h"
#include "dg_map_sl.h"
#include "dg_map_s.h"
#include "dg_set.h"
#include "DgTypes.h"

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
	Dg::set<entityID>						EntityIDs;
  Dg::map_sl<entityID, Component_META>                 Metas;
  Dg::map_sl<entityID, Component_POSITION>				Positions;
  Dg::map_sl<entityID, Component_MOVEMENT>				Movements;
  Dg::map_sl<entityID, Component_PHYSICS>				Physics;
  Dg::map_sl<entityID, Component_LIGHTS_AFFECTING>		LightsAffecting;
  Dg::map_sl<entityID, Component_POINTLIGHT>			PointLights;
  Dg::map_sl<entityID, Component_SPOTLIGHT>			SpotLights;
  Dg::map_sl<entityID, Component_ASPECT>				Aspects;
  Dg::map_sl<entityID, Component_CAMERA>				Cameras;
  Dg::map_sl<entityID, Component_PARTICLEEMITTER>		ParticleEmitters;

	//--------------------------------------------------------------------------------
	//		Other level data
	//--------------------------------------------------------------------------------
	entityID							player;
	Skybox								skybox;
	AmbientLight						ambientLight;
  Dg::map_sl<entityID, DirectionalLight>	directionalLights;


private:    //Data

	std::string id;

private:    //Functions

    //! @Load class template file
    bool LoadClassFile(const std::string&);

    // Functions that help validate class files.
    bool IsIDValid(pugi::xml_node& parent, uint32 id);
    bool IsIDInRoot(uint32);
    bool GetIDValue(pugi::xml_node&, uint32&);

    // Load functions
    entityID LoadClass(uint32 classID, entityID parent);
    bool LoadEntity(pugi::xml_node& node);
    bool AmmendEntity(pugi::xml_node& node, entityID id);

};

inline bool IsIDOK(entityID id)
{
    return (id & ENTITYID::CLASS_MASK) > ENTITYID::CUTOFF;
}

// Find the global velocity vector from a child in the family tree.
// Note: The family heirarcy of positions must be established before this
//       function should be called.
bool GetWorldSpaceVelocity(GameDatabase&, Vector4&, entityID);

inline entityID GetID(uint32 classID, uint32 instanceID)
{
    return entityID((classID << 16) | instanceID);
}

#endif