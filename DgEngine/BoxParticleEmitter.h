/*!
 * @file BoxParticleEmitter.h
 *
 * @author Frank Hart
 * @date 16/01/2014
 *
 * class declaration: BoxParticleEmitter
 */

#ifndef BOXPARTICLEEMITTER_H
#define BOXPARTICLEEMITTER_H

#include "ParticleEmitter.h"
#include "OBB.h"
#include "ObjectPair.h"
#include "SimpleRNG.h"

/*!
 * @ingroup particle_systems
 *
 * @class BoxParticleEmitter
 *
 * @brief A particle emitter which emits particles in a defined OBB
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date 16/01/2014
 */
class BoxParticleEmitter : public ParticleEmitter
{
public:

	BoxParticleEmitter();
	~BoxParticleEmitter() {}

	BoxParticleEmitter(const BoxParticleEmitter&);
	BoxParticleEmitter& operator= (const BoxParticleEmitter&);

	//! Create a deep copy of the object
	BoxParticleEmitter* clone() const { return new BoxParticleEmitter(*this); }

	//! Build from xml_node
	friend pugi::xml_node& operator>>(pugi::xml_node&, BoxParticleEmitter& dest);

	/*!
	* The vqs affects the following:
	*      - size, but only on initial construction of particles
	*      - speed, but only on newly emitted particles
	*      - position, but only on newly emitted particles.
	*/
	void SetVQS(const VQS&);


	//!Set the bounding box particles with appear in.
	void SetOBB(const OBB& b);

	//!Adds noise to a particles initial direction
	void SetDirection(const Vector4&, const ndb_f_param&);

private:

	SimpleRNG generator;
	ObjectPair<OBB> box;

	//The direction the particles will initially move
	Vector4 direction_BASE;
	Vector4 direction;
	ndb_f theta;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
    void SetNewParticle(Particle*);

};

#endif