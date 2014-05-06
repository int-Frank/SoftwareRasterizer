/*!
 * @file ParticleEmitter.h
 *
 * @author Frank Hart
 * @date 10/01/2014
 *
 * class declaration: ParticleEmitter
 */

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "NormalDistributionBounded.h"
#include "Particle.h"
#include "ParticleAlphaTemplate.h"
#include "DgLinkedList.h"
#include "DgArray.h"
#include "DgTypes.h"
#include "Color.h"
#include "VQS.h"
#include "Dg_io.h"
#include "ParticleEngineDefaults.inl"
#include "FastPoisson.h"
#include "BaseClass.h"
#include "Sphere.h"
#include "ObjectPair.h"

namespace pugi{ class xml_node; }
class Viewport;
class Frustum;

/*!
 * @ingroup particle_systems
 *
 * @class ParticleEmitter
 *
 * @brief Base particle emitter.
 *
 * This base emitter is essentially a point emitter where
 * particles are ejected at random trajectories.
 *
 * @author Frank Hart
 * @date 10/01/2014
 */
class ParticleEmitter : public BaseClass<ParticleEmitter>
{
private:

	struct Flags;

public:

	//! Constructs with default paramaters.
	ParticleEmitter();
	virtual ~ParticleEmitter() {}

	ParticleEmitter(const ParticleEmitter&);
	ParticleEmitter& operator=(const ParticleEmitter&);

	//! Create a deep copy of the object
	virtual ParticleEmitter* clone() const { return new ParticleEmitter(*this); }

	//! Build from xml_node
	friend pugi::xml_node& operator>>(pugi::xml_node&, ParticleEmitter& dest);

	/*!
	 * Update all particles
	 *
	 * @param dt The change in time since the last update.
	 * @return Return code, not yet implemented
	 */
	uint32 Update(float dt);

	//! The class-specific particle generator.
	//! Must be call to affect any changes.
	//! @return Return code not yet implemented
	virtual uint32 Initiate();

	//! Sets the maximum number of particles allowed. Note,
	//! this clears all current particles.
	void SetMaxNumber(uint32);

	//! Sets the alpha template for each particle.
	void SetAlphaTemplate(double exp, double master);

	//! Flags new values but but does not commit changes. New particles 
	//! are only generated with the GenerateParticles() function. 
	void SetRadii(const ndb_f_param& start, const ndb_f_param& end);

	//! Flags new values but but does not commit changes. New particles 
	//! are only generated with the GenerateParticles() function. 
	void SetEntrySpeed(const ndb_f_param& speed);

	//! Accelerating or retarding force added to each particle.
	//! This function updates all particles.
	void SetForce(const ndb_f_param& a);

	//! Flags new values but but does not commit changes. New particles 
	//! are only generated with the GenerateParticles() function. 
	void SetLifeTime(const ndb_f_param& alive);

	//! Flags new values but but does not commit changes. New particles 
	//! are only generated with the GenerateParticles() function. 
	void SetColor(const ndb_f_param& red_s, const ndb_f_param& green_s,
		const ndb_f_param& blue_s, const ndb_f_param& alpha_s,
		const ndb_f_param& red_e, const ndb_f_param& green_e,
		const ndb_f_param& blue_e, const ndb_f_param& alpha_e);

	/*!
	 * The vqs affects the following:
	 *      - size, but only on initial construction of particles
	 *      - speed, but only on newly emitted particles
	 *      - position, but only on newly emitted particles.
	 */
	virtual void SetVQS(const VQS&);

	//! Culls particles against a frustum
	void FrustumCull(const Frustum&);

    //! Set the global velocity and acceleration for this emitter
    void SetGlobal(const Vector4& v, const Vector4& a);

    //! Set the global velocity for this emitter
    void SetGlobalVelocity(const Vector4& v);

	//! Turns an emitter on/off.
	void SetActive(bool b){ flags.isActive = b; }

	bool IsActive() const { return flags.isActive; }

	//! Toggles an emitter on/off.
	void ToggleActive(bool){ flags.isActive = !flags.isActive; }

	//! Particles are ejected at a constant rate
	void SetSpawnMethod_LINEAR(float birthRate);

	//! All particles are ejected as soon as an empty slot becomes available.
	void SetSpawnMethod_INSTANT();

	//! Sets the number of particles ejected per second. 
	//! maxEjection is the number of standard deviations above the rate.
	void SetSpawnMethod_POISON(float rate);

	//! Send particles down the pipeline
	void Render(Viewport*, VQS& T_CAM_WLD);

protected:

	//Build base data from xml node
	void BuildBase(pugi::xml_node&);

	//Current vqs
	VQS vqs;

	ndb_f entry_speed;

	//Velocity and acceleration applied to all particles
	Vector4 velocity_global;
	Vector4 acceleration_global;

    struct FixedPoint
    {
        Point4 point;
        float pull;
    };

    DgArray<FixedPoint> FixedPoints;

private:

	//--------------------------------------------------------------------------------
	//		Data
	//--------------------------------------------------------------------------------

	//The rate at which particles are ejected.
	enum EmissionType{ LINEAR, POISSON, INSTANT } emissionType;
	FastPoisson<float> emissionRate_Poisson;
	float linearBirthTime;
	float dtLinear;

	//The alpha template to use
	ParticleAlphaTemplate alphaTemplate;

	//The list of particles
	DgLinkedList<Particle> particles;

	//How the size of a particle changes over its life. This is set
	//once 
	ndb_f radius_start;
	ndb_f radius_end;

	//Entry speed of the particles
	ndb_f force;	//Either accelerates or retards the velocity vector.

	//Lifetime of particles (ms)
	ndb_f lifeTime;

	//How the color (or transparency) of a particle changes 
	//over its life time.
	ndb_f red_start;
	ndb_f red_end;
	ndb_f green_start;
	ndb_f green_end;
	ndb_f blue_start;
	ndb_f blue_end;
	ndb_f alpha_start;
	ndb_f alpha_end;

	ObjectPair<Sphere> boundingSphere;

	struct Flags
	{
		Flags() : isActive(PE_DEFAULT_FLAG_ACTIVE){}

		bool isActive;

	} flags;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void init(const ParticleEmitter& other);
	uint32 GetNumberToAdd(float dt);
    virtual void SetNewParticle(Particle*);

    //May decrement the iterator if a particle is removed
    void UpdateParticle(DgLinkedList<Particle>::iterator&, float dt);

};



#endif