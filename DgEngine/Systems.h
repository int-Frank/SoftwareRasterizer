#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "DgTypes.h"
#include "DgArray.h"
#include "ViewportEvent.h"

class GameDatabase;

class Component_CAMERA;
class ObjectController;
class Light;
class Viewport;

//--------------------------------------------------------------------------------
/*
		* Processes a freshly built entity
        * precondition: The entity has just been constructed from an xml file
        * postcondition: Tidies up the data, makes connections etc...
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_PostProcess(GameDatabase&);

//--------------------------------------------------------------------------------
/*
		* Moves an entity
        * precondition: Components: MOVEMENT, POSITION
        * postcondition: Adds Movement to Position
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_Move(GameDatabase&, float dt);


//--------------------------------------------------------------------------------
/*
		* Updates position hierarchy trees
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_UpdatePositionHierarchies(GameDatabase&);


//--------------------------------------------------------------------------------
/*
		* Updates BV
        * precondition: Components: BV, POSITION
        * postcondition: Generates new BV->current
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_UpdatePhysics(GameDatabase&);


//--------------------------------------------------------------------------------
/*
		* Processes viewport events
		* precondition: Components: CAMERA
		* postcondition: Switches camera viewport
*/
//--------------------------------------------------------------------------------
void SYSTEM_AssignViewports(GameDatabase&, DgArray<ViewportEvent>&);

/*!
* Updates particle emitters in the database
*
* @pre [pre condition]
* @post [post condition]
*/
void SYSTEM_UpdateParticleEmitters(GameDatabase& data, float dt);

//--------------------------------------------------------------------------------
/*
		* Sends object to the master polygon list
        * precondition: Components: BV, POSITION, ASPECT
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_Add_Entities(GameDatabase&, uint32 clock_time);
void SYSTEM_Add_Skyboxes(GameDatabase&);
void SYSTEM_Render(GameDatabase&);
void SYSTEM_FrustumCull(GameDatabase&, entityID camera_id);


//--------------------------------------------------------------------------------
/*
		* Adds player input to the camera entity
        * precondition: Components: CAMERA, MOVEMENT
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_CameraControl(GameDatabase& data,
						  const ObjectController& controller, float dt);
void SYSTEM_CameraPost(GameDatabase& data);


//--------------------------------------------------------------------------------
/*
		* Tests all lights in the input array against the entity
        * precondition: Components: LIGHTS_ATTACHED
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_AddLights(GameDatabase&);


//--------------------------------------------------------------------------------
/*
		* Moves an entity's lights
        * precondition: Components: LIGHTS_ATTACHED, POSITION
        * postcondition: Transforms lights to new vqs
*/ 
//--------------------------------------------------------------------------------
void SYSTEM_UpdateLights(GameDatabase&);

#endif